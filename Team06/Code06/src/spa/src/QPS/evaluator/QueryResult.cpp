#include "QueryResult.h"
#include "CSPUtils.h"
#include "iostream"
#include <set>

void QueryResult::addResult(std::string res) { result.insert(res); }

bool QueryResult::hasCombinationTested(std::shared_ptr<PKBGetter> pkb,
                                       const SelectClause &selectClause,
                                       assign_t &assign) {
  std::string combStr = generateCombString(selectClause, assign);
  return visitedCombs.find(combStr) != visitedCombs.end();
}

[[maybe_unused]] void QueryResult::printResult() {
  for (auto e = this->result.begin(); e != this->result.end(); e++) {
    if (e != this->result.begin()) {
      std::cout << ", ";
    }
    std::cout << *e;
  }
  std::cout << "\n";
}

std::string QueryResult::toString() const {
  if (this->result.size() == 0 && this->type == QueryResultType::BOOLEAN) {
    return RETURN_LITERAL_FALSE;
  } else if (this->result.size() == 0) {
    return RETURN_LITERAL_NONE;
  }

  std::string result = "";
  for (auto e = this->result.begin(); e != this->result.end(); e++) {
    if (e != this->result.begin()) {
      if (type == QueryResultType::TUPLE) {
        result += ",";
      }
      result += " ";
    }
    result += *e;
  }
  return result;
}

/**
 * This equal is only used for unit testing, performance is not an issue
 * @param other
 * @return
 */
bool QueryResult::operator==(const QueryResult &other) const {
  if (result.size() != other.result.size()) {
    return false;
  }
  std::set<std::string> resultSet(result.begin(), result.end());
  std::set<std::string> otherSet(other.result.begin(), other.result.end());

  return resultSet == otherSet;
}

bool QueryResult::operator!=(const QueryResult &other) const {
  return !((*this) == other);
}

void QueryResult::generateResults(std::shared_ptr<PKBGetter> pkb,
                                  const SelectClause &selectClause) {
  if (this->result.find(FAILURE_PLACEHOLDER) != this->result.end()) {
    result.clear();
    return;
  }

  if (this->result.size() != 0) {
    return;
  }

  if (selectClause.isBoolean()) {
    this->addResult(RETURN_LITERAL_TRUE);
    return;
  }

  interRes_t combinedRes;
  for (auto &intermediateResult : intermediateResultGroup) {
    if (combinedRes.empty()) {
      combinedRes = intermediateResult;
      continue;
    }
    cartesianProduct(combinedRes, combinedRes, intermediateResult);
  }

  for (const auto &resultMap : combinedRes) {
    addResult(generateResultString(pkb, selectClause, resultMap));
  }
}

const std::unordered_set<std::string> &QueryResult::getResult() {
  return result;
}

std::string QueryResult::generateResultString(std::shared_ptr<PKBGetter> pkb,
                                              const SelectClause &selectClause,
                                              assign_t assignMap) {

  std::string resStr = "";

  auto &targets = selectClause.getSelected();
  for (int i = 0; i < targets.size(); i++) {
    std::string synonym = targets[i].getSynonymName();
    EntityType synType = targets[i].getSynonymType();
    if ((synType == EntityType::CALL &&
         targets[i].getAttribute() ==
             AttributeType::PROCNAME) || // special cases
        (synType == EntityType::READ &&
         targets[i].getAttribute() == AttributeType::VARNAME) ||
        (synType == EntityType::PRINT &&
         targets[i].getAttribute() == AttributeType::VARNAME)) {
      resStr += pkb->getVarAt(assignMap[synonym]);
    } else {
      resStr += assignMap[synonym];
    }

    if (i != targets.size() - 1) {
      resStr += " ";
    }
  }

  return resStr;
}

std::string QueryResult::generateCombString(const SelectClause &selectClause,
                                            assign_t &assign) {

  std::string res;
  for (const auto &syn : selectClause.getSelectedSynonyms()) {
    if (assign.find(syn) != assign.end()) {
      if (assign[syn] == CSPUtils::ASSIGNED) {
        res +=  assign[syn] + std::to_string(randomSeed) + "_";
        randomSeed++;
      } else {
        res += assign[syn] + "_";
      }
    } else {
      res += CSPUtils::UNASSIGNED + "_";
    }
  }
  return res;
}

QueryResult::QueryResult(const std::unordered_set<std::string> &result,
                         QueryResultType type) {
  this->result = result;
  this->type = type;
}

QueryResult::QueryResult() {}

QueryResult::QueryResult(QueryResultType type) : type(type) {}

void QueryResult::addFailureResult(const SelectClause &selectClause) {
  if (selectClause.isBoolean()) {
    this->result.insert(RETURN_LITERAL_FALSE);
  } else {
    this->result.insert(FAILURE_PLACEHOLDER);
  }
}

void QueryResult::addErrorResult(QPSException &e) {
  this->addResult(std::string(e));
}

void QueryResult::clearVisitedCombs() {
  this->visitedCombs.clear();
}

void QueryResult::addResult(const SelectClause &selectClause, assign_t &assign,
                            orderDomain_t &orderDomain, int groupId) {
  
  std::string combStr = generateCombString(selectClause, assign);
  visitedCombs.insert(combStr);
  if (selectClause.isBoolean()) {
    return;
  }

  interRes_t currRes;
  cartesianProduct(selectClause, currRes, assign, orderDomain);

  if (intermediateResultGroup.size() <= groupId) {
    intermediateResultGroup.push_back({});
  }

  auto &targetGroup = intermediateResultGroup[groupId];
  targetGroup.insert(targetGroup.end(),
                     std::make_move_iterator(currRes.begin()),
                     std::make_move_iterator(currRes.end()));
}

void QueryResult::cartesianProduct(interRes_t &interRes, interRes_t &interRes1,
                                   interRes_t &interRes2) {
  interRes_t tmpInterRes;
  if (interRes1.size() == 0) {
    interRes = interRes2;
  }
  else if (interRes2.size() == 0) {
    interRes = interRes1;
  }
  else {
    for (auto& assign1 : interRes1) {
      for (auto& assign2 : interRes2) {
        assign_t combinedRes;
        combinedRes.insert(assign1.begin(), assign1.end());
        combinedRes.insert(assign2.begin(), assign2.end());
        tmpInterRes.emplace_back(combinedRes);
      }
    }
    interRes = tmpInterRes; // copy
  }

}

/**
 * Cartesian product for all the selected synonyms
 * @param selectClause
 * @param interRes
 * @param assign
 * @param orderDomain
 */
void QueryResult::cartesianProduct(const SelectClause &selectClause,
                                   interRes_t &interRes, assign_t &assign,
                                   orderDomain_t &orderDomain) {
  for (const auto &[syn, val] : assign) {
    if (!selectClause.isSynonymSelected(syn)) {
      continue;
    }

    domain_t vals = orderDomain[syn];
    addSingleAssign(interRes, syn, vals);
  }
}

void QueryResult::addSingleAssign(interRes_t &interRes, const std::string &syn,
                                  const domain_t &vals) {
  if (interRes.empty()) {
    for (const auto &value : vals) {
      interRes.push_back({{syn, value}});
    }
    return;
  }

  interRes_t tmpInterRes;
  for (auto &res : interRes) {
    for (const auto &value : vals) {
      assign_t assignCpy = res;
      assignCpy[syn] = value;
      tmpInterRes.emplace_back(assignCpy);
    }
  }

  interRes = tmpInterRes; // copy
}
