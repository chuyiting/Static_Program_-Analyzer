#include "AffectsT.h"
#include "QPS/evaluator/CSPUtils.h"

AffectsT::AffectsT(StmtRef lhs, StmtRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

bool AffectsT::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                   std::string lhs, std::string rhs) {
  return pkb->isAffectsT(lhs, rhs);
}

bool AffectsT::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                               domain_t &result) {
  result = pkb->getAffectsT(rhs, false);
  return true;
}

bool AffectsT::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                                domain_t &result) {
  result = pkb->getAffectsT(lhs, true);
  return true;
}
