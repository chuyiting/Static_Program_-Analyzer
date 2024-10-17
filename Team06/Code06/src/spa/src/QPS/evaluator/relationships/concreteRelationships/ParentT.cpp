//
// Created by Eddy Chu on 2022/9/1.
//

#include "ParentT.h"
#include "QPS/evaluator/CSPUtils.h"

ParentT::ParentT(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

bool ParentT::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                  std::string lhs, std::string rhs) {
  return pkb->isParentT(lhs, rhs);
}

bool ParentT::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                              domain_t &result) {
  result = pkb->getParentTOf(rhs, false);
  return true;
}

bool ParentT::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                               domain_t &result) {
  result = pkb->getParentTOf(lhs, true);
  return true;
}
