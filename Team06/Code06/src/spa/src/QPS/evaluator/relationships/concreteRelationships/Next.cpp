#include "Next.h"

#include <string>
#include "QPS/evaluator/CSPUtils.h"

Next::Next(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

bool Next::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                               std::string rhs) {
  return pkb->isNext(lhs, rhs);
}

bool Next::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                           domain_t &result) {
  result = pkb->getNext(rhs, false);
  return true;
}
bool Next::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                            domain_t &result) {
  result = pkb->getNext(lhs, true);
  return true;
}
