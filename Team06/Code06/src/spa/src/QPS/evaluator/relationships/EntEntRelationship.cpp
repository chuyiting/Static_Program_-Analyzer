//
// Created by Eddy Chu on 2022/10/11.
//

#include "EntEntRelationship.h"

std::vector<synonym_t> EntEntRelationship::getSynonyms() {
  std::vector<synonym_t> result;
  if (lhs.getType() == EntRef::SYNONYM) {
    result.push_back(lhs.getSynonym().getSynonymName());
  }

  if (rhs.getType() == EntRef::SYNONYM) {
    result.push_back(rhs.getSynonym().getSynonymName());
  }

  return result;
}

bool EntEntRelationship::operator==(const Relationship &other) {
  try {
    auto &derived = dynamic_cast<const EntEntRelationship &>(other);
    return (*this) == derived;
  } catch (std::bad_cast &err) {
    return false;
  }
}

bool EntEntRelationship::operator!=(const Relationship &other) {
  return !((*this) == other);
}

bool EntEntRelationship::operator==(const EntEntRelationship &other) {
  return this->lhs == other.lhs && this->rhs == other.rhs;
}

bool EntEntRelationship::operator!=(const EntEntRelationship &other) {
  return !((*this) == other);
}

bool EntEntRelationship::isSatisfied(std::shared_ptr<PKBGetter> pkb,
                                     assign_t &assign) {
  return isSatisfiedFromRefVal(pkb, lhs.getEntValue(assign),
                               rhs.getEntValue(assign));
}

bool EntEntRelationship::queryDomain(std::shared_ptr<PKBGetter> pkb,
                                     assign_t &assign,
                                     orderDomain_t &orderDomain,
                                     orderDomain_t &queryResult) {
#ifdef DEBUG
  assert(hasExactOneVarUnassigned(assign));
#endif
  if (rhs.isWildcard() || lhs.isWildcard())
    return false;

  if (lhs.isSynonym() && assign[lhs.getEntString()] == CSPUtils::UNASSIGNED) {
    auto rhsVal = rhs.getEntValue(assign);

    domain_t stmts;
    if (!queryLeftDomain(pkb, rhsVal, stmts))
      return false;
    queryResult.insert({lhs.getEntString(), stmts});

  } else {
    auto lhsVal = lhs.getEntValue(assign);

    domain_t vars;
    if (!queryRightDomain(pkb, lhsVal, vars))
      return false;
    queryResult.insert({rhs.getEntString(), vars});
  }

  CSPUtils::intersectOrderDomains(queryResult, orderDomain);
  return true;
}
