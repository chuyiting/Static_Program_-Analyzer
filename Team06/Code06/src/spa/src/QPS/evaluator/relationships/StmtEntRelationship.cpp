//
// Created by Eddy Chu on 2022/10/11.
//

#include "StmtEntRelationship.h"

std::vector<synonym_t> StmtEntRelationship::getSynonyms() {
  std::vector<synonym_t> result;
  if (lhs.getType() == StmtRef::SYNONYM) {
    result.push_back(lhs.getSynonym().getSynonymName());
  }

  if (rhs.getType() == EntRef::SYNONYM) {
    result.push_back(rhs.getSynonym().getSynonymName());
  }

  return result;
}

bool StmtEntRelationship::operator==(const Relationship &other) {
  try {
    auto &derived = dynamic_cast<const StmtEntRelationship &>(other);
    return (*this) == derived;
  } catch (std::bad_cast &err) {
    return false;
  }
}

bool StmtEntRelationship::operator!=(const Relationship &other) {
  return !((*this) == other);
}

bool StmtEntRelationship::operator==(const StmtEntRelationship &other) {
  return this->lhs == other.lhs && this->rhs == other.rhs;
}

bool StmtEntRelationship::operator!=(const StmtEntRelationship &other) {
  return !((*this) == other);
}

bool StmtEntRelationship::isSatisfied(std::shared_ptr<PKBGetter> pkb,
                                      assign_t &assign) {
  return isSatisfiedFromRefVal(pkb, lhs.getStmtValue(assign),
                               rhs.getEntValue(assign));
}
bool StmtEntRelationship::queryDomain(std::shared_ptr<PKBGetter> pkb,
                                      assign_t &assign,
                                      orderDomain_t &orderDomain,
                                      orderDomain_t &queryResult) {

#ifdef DEBUG
  assert(hasExactOneVarUnassigned(assign));
#endif
  if (rhs.isWildcard() || lhs.isWildcard())
    return false;

  if (lhs.isSynonym() && assign[lhs.getStmtString()] == CSPUtils::UNASSIGNED) {
    auto rhsVal = rhs.getEntValue(assign);

    domain_t stmts;
    if (!queryLeftDomain(pkb, rhsVal, stmts))
      return false;
    queryResult.insert({lhs.getStmtString(), stmts});

  } else {
    auto lhsVal = lhs.getStmtValue(assign);

    domain_t vars;
    if (!queryRightDomain(pkb, lhsVal, vars))
      return false;
    queryResult.insert({rhs.getEntString(), vars});
  }

  CSPUtils::intersectOrderDomains(queryResult, orderDomain);
  return true;
}
