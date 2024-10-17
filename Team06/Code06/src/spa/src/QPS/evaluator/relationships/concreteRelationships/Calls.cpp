#include "Calls.h"
#include "QPS/evaluator/CSPUtils.h"

Calls::Calls(EntRef lhs, EntRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool Calls::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                                std::string rhs) {
  return pkb->isCalls(lhs, rhs);
}

bool Calls::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                            domain_t &result) {
  result =  pkb->getCalls(rhs, false);
  return true;
}

bool Calls::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                             domain_t &result) {
  result =  pkb->getCalls(lhs, true);
  return true;
}
