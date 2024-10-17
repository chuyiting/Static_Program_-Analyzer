//
// Created by Eddy Chu on 2022/9/1.
//
#include <memory>
#include <string>
#include <utility>

#include "QPS/evaluator/CSPUtils.h"
#include "QPS/evaluator/relationships/Relationship.h"
#include "UsesS.h"

UsesS::UsesS(StmtRef lhs, EntRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

bool UsesS::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                                std::string rhs) {
  return pkb->isStatementUses(lhs, rhs);
}

bool UsesS::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                            domain_t &result) {
  result = pkb->getUsesS(rhs, false);
  return true;
}

bool UsesS::queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                             domain_t &result) {
  result = pkb->getUsesS(lhs, true);
  return true;
}
