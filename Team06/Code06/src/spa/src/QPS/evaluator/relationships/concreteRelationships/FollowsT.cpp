//
// Created by Eddy Chu on 2022/9/1.
//

#include "FollowsT.h"
#include "QPS/evaluator/CSPUtils.h"

FollowsT::FollowsT(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool FollowsT::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                   std::string lhs, std::string rhs) {
  return pkb->isFollowsT(lhs, rhs);
}

bool FollowsT::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                               domain_t &result) {
  result = pkb->getFollowsT(rhs, false);
  return true;
}
bool FollowsT::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                                domain_t &result) {
  result = pkb->getFollowsT(lhs, true);
  return true;
}
