//
// Created by Eddy Chu on 2022/9/1.
//

#include "UsesP.h"
#include "QPS/evaluator/CSPUtils.h"
#include "QPS/evaluator/relationships/Relationship.h"
#include <memory>
#include <utility>

UsesP::UsesP(EntRef lhs, EntRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

bool UsesP::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                                std::string rhs) {
  return pkb->isProcedureUses(lhs, rhs);
}

bool UsesP::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                            domain_t &result) {
  result = pkb->getUsesP(rhs, false);
  return true;
}

bool UsesP::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                             domain_t &result) {
  result = pkb->getUsesP(lhs, true);
  return true;
}
