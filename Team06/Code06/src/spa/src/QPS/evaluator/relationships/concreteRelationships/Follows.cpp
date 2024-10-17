//
// Created by Eddy Chu on 2022/9/1.
//

#include "Follows.h"
#include "QPS/evaluator/CSPUtils.h"

Follows::Follows(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool Follows::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                  std::string lhs, std::string rhs) {
  return pkb->isFollows(lhs, rhs);
}

bool Follows::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                              domain_t &result) {
  result = pkb->getFollows(rhs, false);
  return true;
}

bool Follows::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                               domain_t &result) {
  result = pkb->getFollows(lhs, true);
  return true;
}
