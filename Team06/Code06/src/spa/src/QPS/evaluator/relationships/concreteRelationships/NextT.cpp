#include "NextT.h"
#include "QPS/evaluator/CSPUtils.h"

NextT::NextT(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool NextT::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                                std::string rhs) {
  return pkb->isNextT(lhs, rhs);
}

bool NextT::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                            domain_t &result) {
  result = pkb->getNextT(rhs, false);
  return true;
}
bool NextT::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                             domain_t &result) {
  result = pkb->getNextT(lhs, true);
  return true;
}
