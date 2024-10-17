#include "Affects.h"
#include "QPS/evaluator/CSPUtils.h"

Affects::Affects(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool Affects::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                  std::string lhs, std::string rhs) {
  return pkb->isAffects(lhs, rhs);
}

bool Affects::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                              domain_t &result) {
  return false;
}
bool Affects::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                               domain_t &result) {
  result = pkb->getAffects(lhs, true);
  return true;
}
