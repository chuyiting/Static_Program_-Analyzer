//
// Created by Eddy Chu on 2022/9/10.
//

#include "QueryEvaluator.h"
#include "CSPUtils.h"
#include <algorithm>
#include <cassert>
#include <queue>
#include <utility>

QueryEvaluator::QueryEvaluator(std::shared_ptr<PKBGetter> pkb)
    : pkb(std::move(pkb)) {}

std::shared_ptr<QueryResult>
QueryEvaluator::evaluate(std::shared_ptr<QPSQueryEvaluatorAPI> query) {
  this->query = query;

  auto result = std::make_shared<QueryResult>(query->getQueryResultType());

  // note that the sequence is not interchangeable
  bool initConstResult = initConstraints();
  if (!initConstResult) {
    result->addFailureResult(query->getSelectClause());
    result->generateResults(pkb, query->getSelectClause());
    query = NULL;
    return result;
  }

  initAssign();
  initPriorityMap();

  for (auto &assign : assigns) {
    initOrderDomain(assign);
    if (!updateOrderDomain(assign)) {
      result->addFailureResult(query->getSelectClause());
      break;
    }
    bool hasSol = false;
    backtrackingSearch(assign, result, hasSol, !containSelectedSynonym(assign));
    if (!hasSol) {
      result->addFailureResult(query->getSelectClause());
      break;
    }
    result->clearVisitedCombs();
    currGroupId++;
  }

  result->generateResults(pkb, query->getSelectClause());

  query = NULL;
  return result;
}

bool QueryEvaluator::initConstraints() {
  if (query->getAllConstraints().empty())
    return true;

  for (const auto &constraint : query->getAllConstraints()) {
    auto variables = constraint->getVariables();
    assign_t emptyAssign;
    if (variables.empty() && !constraint->isSatisfied(pkb, emptyAssign)) {
      return false;
    } else if (variables.empty()) {
      continue;
    }

    for (const auto &var : variables) {
      if (constraints.find(var) == constraints.end()) {
        std::vector<std::shared_ptr<QPSConstraint>> constraintList = {
            constraint};
        constraints.insert(std::make_pair(var, constraintList));
      } else {
        constraints.find(var)->second.push_back(constraint);
      }
    }
  }

  return true;
}

void QueryEvaluator::initAssign() {

  std::unordered_set<synonym_t> visited;
  std::queue<synonym_t> queue;
  for (const auto &[syn, _] : constraints) {
    if (visited.find(syn) != visited.end()) {
      continue;
    }

    // breadth first search
    assign_t currAssign;
    queue.push(syn);

    while (!queue.empty()) {
      auto curr = queue.front();
      queue.pop();

      currAssign.insert(std::make_pair(curr, CSPUtils::UNASSIGNED));
      visited.insert(curr);

      for (auto &constraint : constraints[curr]) {
        auto vars = constraint->getVariables();
        for (auto &var : vars) {
          if (visited.find(var) != visited.end()) {
            continue;
          }

          queue.push(var);
        }
      }
    }

    assigns.push_back(currAssign);
  }

  for (const auto &syn : query->extractSelectedSynonyms()) {
    if (visited.find(syn) != visited.end()) {
      continue;
    }
    assigns.push_back({{syn, CSPUtils::UNASSIGNED}});
  }
}

void QueryEvaluator::initPriorityMap() {
  for (auto &[syn, cs] : constraints) {
    int priority = 0;
    for (auto &c : cs) {
      priority += c->priority();
    }
    priorityMap[syn] = priority;
  }
}

bool QueryEvaluator::initOrderDomain(assign_t &assign) {
  orderDomain.clear();
  for (auto const &[synonym, _] : assign) {

    switch (query->getType(synonym)) {
    case EntityType::PROCEDURE:
      orderDomain.insert(std::make_pair(synonym, pkb->getProcedures()));
      continue;
    case EntityType::STMT:
      orderDomain.insert(std::make_pair(synonym, pkb->getStatements()));
      continue;
    case EntityType::READ:
      orderDomain.insert(std::make_pair(synonym, pkb->getReadStmts()));
      continue;
    case EntityType::PRINT:
      orderDomain.insert(std::make_pair(synonym, pkb->getPrintStmts()));
      continue;
    case EntityType::ASSIGN:
      orderDomain.insert(std::make_pair(synonym, pkb->getAssignStmts()));
      continue;
    case EntityType::VARIABLE:
      orderDomain.insert(std::make_pair(synonym, pkb->getVariables()));
      continue;
    case EntityType::CONSTANT:
      orderDomain.insert(std::make_pair(synonym, pkb->getConstants()));
      continue;
    case EntityType::IF:
      orderDomain.insert(std::make_pair(synonym, pkb->getIfStmts()));
      continue;
    case EntityType::WHILE:
      orderDomain.insert(std::make_pair(synonym, pkb->getWhileStmts()));
      continue;
    case EntityType::CALL:
      orderDomain.insert(std::make_pair(synonym, pkb->getCallStmts()));
      continue;
    default: {
#ifdef DEBUG
      assert(false && "you should never reach here...");
#endif
    }
    }
  }

  return true;
}

bool QueryEvaluator::backtrackingSearch(assign_t &assign,
                                        std::shared_ptr<QueryResult> result,
                                        bool &hasSol, bool isBoolean) {

  if (CSPUtils::isAllVariableAssigned(assign)) {
    result->addResult(query->getSelectClause(), assign, orderDomain,
                      currGroupId);
    hasSol = true;
    return true;
  }

  std::vector<std::shared_ptr<QPSConstraint>> newlySatisfiedC;
  synonym_t var = pickUnassignedVariable(assign);

  if (allConstraintsSatisfied(var)) {
    assign[var] = CSPUtils::ASSIGNED;
    bool res = backtrackingSearch(assign, result, hasSol, isBoolean);
    assign[var] = CSPUtils::UNASSIGNED;
    return res;
  }

  // first copy the old domain
  domain_t oldDomain = domain_t(orderDomain[var]);

  // loop through constraints, and make domain smaller
  bool allSatisfied = true;
  for (const auto &c : constraints[var]) {
    orderDomain_t queryDomain;
    // skip already satisfied constraints
    if (satisfiedConstraints.find(c) != satisfiedConstraints.end())
      continue;

    if (!c->queryDomain(pkb, assign, orderDomain, queryDomain)) {
      allSatisfied = false;
    } else {
      CSPUtils::intersectOrderDomains(orderDomain, queryDomain);
      if (orderDomain[var].size() == 0) {
        revertConstraints(newlySatisfiedC);
        orderDomain[var] = oldDomain;
        return false;
      }

      addNewlySatisfiedConstraint(c, newlySatisfiedC);
    }
  }

  if (allSatisfied) {
    assign[var] = CSPUtils::ASSIGNED;
    bool searchResult = backtrackingSearch(assign, result, hasSol, isBoolean);
    if (isBoolean && searchResult) {
      return true;
    }
    orderDomain[var] = oldDomain;       // revert orderDomain
    assign[var] = CSPUtils::UNASSIGNED; // revert assign
    revertConstraints(newlySatisfiedC); // revert satisfied constraints
    return searchResult;
  }

  for (auto &val : oldDomain) {
    // short circuit if the combination is already tested
    // an optimization to avoid unnecessary searching
    if (hasSelectCombinationTested(assign, result)) {
      assign[var] = CSPUtils::UNASSIGNED;
      break;
    }

    if (!isValConsistentWithAssignment(val, var, assign))
      continue;
    assign[var] = val;

    orderDomain[var] = {val};
    inference_t inference;
    CSPUtils::initInference(inference, assign);

    bool inferResult = infer(inference, assign, var, newlySatisfiedC);
    if (inferResult) {
      CSPUtils::unionOrderDomainWithInference(orderDomain, inference);
      bool res = backtrackingSearch(assign, result, hasSol, isBoolean);
      if (isBoolean && res) {
        return true;
      }
      CSPUtils::differenceOrderDomainWithInference(orderDomain, inference);
    }

    // restore the orderDomain and assign
    orderDomain[var] = oldDomain;
    assign[var] = CSPUtils::UNASSIGNED;
    revertConstraints(newlySatisfiedC);
  }

  return false;
}

bool QueryEvaluator::hasSelectCombinationTested(
    assign_t &assign, std::shared_ptr<QueryResult> result) {
  return result->hasCombinationTested(pkb, query->getSelectClause(), assign);
}

bool QueryEvaluator::isValConsistentWithAssignment(std::string value,
                                                   const synonym_t &syn,
                                                   assign_t &assign) {
  if (constraints[syn].empty())
    return true;
  assign[syn] = std::move(value);
  for (auto &c : constraints[syn]) {
    if (!c->isSatisfied(pkb, assign)) {
      assign[syn] = CSPUtils::UNASSIGNED;
      return false;
    }
  }
  assign[syn] = CSPUtils::UNASSIGNED;
  return true;
}

bool QueryEvaluator::infer(
    inference_t &inference, assign_t &assign, const synonym_t &var,
    std::vector<std::shared_ptr<QPSConstraint>> &newlySatisfied) {

  std::queue<synonym_t> varQueue;
  varQueue.push(var);

  while (!varQueue.empty()) {
    synonym_t y = varQueue.front();
    varQueue.pop();
    if (constraints.find(y) == constraints.end())
      continue;

    for (const auto &constraint : constraints.find(y)->second) {
      // skip already satisfied constraints
      if (satisfiedConstraints.find(constraint) != satisfiedConstraints.end())
        continue;

      for (const auto &x : constraint->getVariables()) {
        if (x == y) {
          continue;
        }

        domain_t s;
        CSPUtils::computeDomain(x, s, orderDomain, inference);

        orderDomain_t queryDomain;
        if (constraint->queryDomain(pkb, assign, orderDomain, queryDomain)) {
          if (queryDomain[x].size() == 0) {
            return false;
          }

          domain_t inferDomain = orderDomain[x];
          CSPUtils::differenceDomains(inferDomain, queryDomain[x]);
          inference[x] = inferDomain;
          addNewlySatisfiedConstraint(constraint, newlySatisfied);
          return true;
        }

        // This for loop can be replaced by set implementation in pkb
        for (const auto &v : s) {
          std::string oldVal = assign[x];

          assign[x] = v;
          // our constraints have maximum of 2 variables for basic SPA
          if (!constraint->isSatisfied(pkb, assign)) {
            inference[x].insert(v);
          }
          assign[x] = oldVal;
        }

        domain_t t;
        CSPUtils::computeDomain(x, t, orderDomain, inference);
        if (t.empty()) {
          return false;
        }

        // forward checking
        addNewlySatisfiedConstraint(constraint, newlySatisfied);
      }
    }
  }
  return true;
}

synonym_t QueryEvaluator::pickUnassignedVariable(assign_t &assign) {
  synonym_t var;
  int currMin = INT16_MAX;
  // implement minimum value heuristic
  for (auto const &[synonym, val] : assign) {
    if (val != CSPUtils::UNASSIGNED)
      continue;

    // immediate return
    if (priorityMap[synonym] >= CSPUtils::PRIORITY_THRESHOLD) {
      return synonym;
    }

    if (orderDomain[synonym].size() < currMin) {
      currMin = orderDomain[synonym].size();
      var = synonym;
    }

    if (currMin <= 1)
      return var;
  }

  if (var.empty()) {
#ifdef DEBUG
    assert(false && "you should never reach here...");
#endif
  }
  return var;
}

bool QueryEvaluator::updateOrderDomain(assign_t &assign) {
  for (auto const &[synonym, _] : assign) {
    auto it = constraints[synonym].begin();
    while (it != constraints[synonym].end()) {
      orderDomain_t queryDomain;
      if (!(*it)->queryDomain(pkb, assign, orderDomain, queryDomain)) {
        it++;
        continue;
      }

      CSPUtils::intersectOrderDomains(orderDomain, queryDomain);
      if (orderDomain[synonym].empty()) {
        return false;
      }

      it = constraints[synonym].erase(it);
    }
  }

  return true;
}

void QueryEvaluator::revertConstraints(
    std::vector<std::shared_ptr<QPSConstraint>> &toRevert) {
  for (auto &c : toRevert) {
    satisfiedConstraints.erase(c);

    // revert priority map
    for (auto &var : c->getVariables()) {
      priorityMap[var] += c->priority();
    }
  }
}

void QueryEvaluator::addNewlySatisfiedConstraint(
    std::shared_ptr<QPSConstraint> c,
    std::vector<std::shared_ptr<QPSConstraint>> &toAdd) {

  toAdd.push_back(c);
  satisfiedConstraints.insert(c);

  for (auto &var : c->getVariables()) {
    priorityMap[var] -= c->priority();
  }
}
bool QueryEvaluator::containSelectedSynonym(assign_t &assign) {
  for (auto &[syn, _] : assign) {
    if (query->isVarSelected(syn)) {
      return true;
    }
  }
  return false;
}

bool QueryEvaluator::allConstraintsSatisfied(synonym_t var) {
  for (auto &c: constraints[var]) {
    if (satisfiedConstraints.find(c) == satisfiedConstraints.end()) {
      return false;
    }
  }
  return true;
}
