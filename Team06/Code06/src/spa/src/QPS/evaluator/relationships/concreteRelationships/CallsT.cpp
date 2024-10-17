#include "CallsT.h"
#include "QPS/evaluator/CSPUtils.h"

CallsT::CallsT(EntRef lhs, EntRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool CallsT::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                 std::string lhs, std::string rhs) {
  return pkb->isCallsT(lhs, rhs);
}

bool CallsT::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                             domain_t &result) {
  result = pkb->getCallsT(rhs, false);
  return true;
}

bool CallsT::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                              domain_t &result) {
  result = pkb->getCallsT(lhs, true);
  return true;
}
