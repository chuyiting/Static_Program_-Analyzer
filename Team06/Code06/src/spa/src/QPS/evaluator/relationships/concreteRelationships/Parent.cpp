//
// Created by Eddy Chu on 2022/9/1.
//

#include "Parent.h"
#include "QPS/evaluator/CSPUtils.h"
#include <string>

Parent::Parent(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

bool Parent::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                 std::string lhs, std::string rhs) {
  return pkb->isParent(lhs, rhs);
}

bool Parent::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                             domain_t &result) {
  result = pkb->getParentOf(rhs, false);
  return true;
}

bool Parent::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                              domain_t &result) {
  result = pkb->getParentOf(lhs, true);
  return true;
}
