//
// Created by Eddy Chu on 2022/9/1.
//

#include "ModifiesP.h"
#include "QPS/evaluator/CSPUtils.h"

#include <utility>

ModifiesP::ModifiesP(EntRef lhs, EntRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}
bool ModifiesP::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                    std::string lhs, std::string rhs) {
  return pkb->isProcedureModifies(lhs, rhs);
}

bool ModifiesP::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                                domain_t &result) {
  auto res = pkb->getModifiesP(rhs, false);
  result = res;
  return true;
}
bool ModifiesP::queryRightDomain(std::shared_ptr<PKBGetter> pkb,
                                 std::string lhs, domain_t &result) {
  result = pkb->getModifiesP(lhs, true);
  return true;
}
