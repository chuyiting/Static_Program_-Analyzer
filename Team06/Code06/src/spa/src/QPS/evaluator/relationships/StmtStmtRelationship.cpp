//
// Created by Eddy Chu on 2022/10/11.
//

#include "StmtStmtRelationship.h"

bool StmtStmtRelationship::operator==(const Relationship &other) {
  try {
    auto &derived = dynamic_cast<const StmtStmtRelationship &>(other);
    return (*this) == derived;
  } catch (std::bad_cast &err) {
    return false;
  }
}

bool StmtStmtRelationship::operator!=(const Relationship &other) {
  return !((*this) == other);
}

bool StmtStmtRelationship::operator==(const StmtStmtRelationship &other) {
  return this->lhs == other.lhs && this->rhs == other.rhs;
}

bool StmtStmtRelationship::operator!=(const StmtStmtRelationship &other) {
  return !((*this) == other);
}

std::vector<synonym_t> StmtStmtRelationship::getSynonyms() {
  std::vector<synonym_t> result;
  if (lhs.getType() == StmtRef::SYNONYM) {
    result.push_back(lhs.getSynonym().getSynonymName());
  }

  if (rhs.getType() == StmtRef::SYNONYM) {
    result.push_back(rhs.getSynonym().getSynonymName());
  }

  return result;
}

bool StmtStmtRelationship::isSatisfied(std::shared_ptr<PKBGetter> pkb,
                                       assign_t &assign) {
  return isSatisfiedFromRefVal(pkb, lhs.getStmtValue(assign),
                               rhs.getStmtValue(assign));
}

bool StmtStmtRelationship::queryDomain(std::shared_ptr<PKBGetter> pkb,
                                       assign_t &assign,
                                       orderDomain_t &orderDomain,
                                       orderDomain_t &queryResult) {
#ifdef DEBUG
  assert(hasExactOneVarUnassigned(assign));
#endif
  if (rhs.isWildcard() || lhs.isWildcard())
    return false;

  if (lhs.isSynonym() && assign[lhs.getStmtString()] == CSPUtils::UNASSIGNED) {
    auto rhsVal = rhs.getStmtValue(assign);

    domain_t stmts;
    if (!queryLeftDomain(pkb, rhsVal, stmts))
      return false;
    queryResult.insert({lhs.getStmtString(), stmts});

  } else {
    auto lhsVal = lhs.getStmtValue(assign);

    domain_t vars;
    if (!queryRightDomain(pkb, lhsVal, vars))
      return false;
    queryResult.insert({rhs.getStmtString(), vars});
  }

  CSPUtils::intersectOrderDomains(queryResult, orderDomain);
  return true;
}
