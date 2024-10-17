//
// Created by Eddy Chu on 2022/10/24.
//
#include "Relationship.h"

bool Relationship::queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                               orderDomain_t &orderDomain,
                               orderDomain_t &queryResult) {
  return false;
}
bool Relationship::queryLeftDomain(std::shared_ptr<PKBGetter> pkb,
                                   std::string rhs, domain_t &result) {
  return false;
}
bool Relationship::queryRightDomain(std::shared_ptr<PKBGetter> pkb,
                                    std::string lhs, domain_t &result) {
  return false;
}
int Relationship::priority() { return 0; }

bool Relationship::isSatisfiedFromRefVal(std::shared_ptr<PKBGetter> pkb,
                                         std::string lhs, std::string rhs) {
  // value unassigned evaluates to true
  if (rhs == CSPUtils::UNASSIGNED || lhs == CSPUtils::UNASSIGNED) {
    return true;
  }

  return isSatisfiedConcrete(pkb, lhs, rhs);
}
