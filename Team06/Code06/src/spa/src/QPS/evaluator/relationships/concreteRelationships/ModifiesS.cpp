#include "ModifiesS.h"
#include "QPS/evaluator/CSPUtils.h"
#include <string>
#include <memory>

ModifiesS::ModifiesS(StmtRef lhs, EntRef rhs) {
  this->lhs = std::move(lhs);
  this->rhs = std::move(rhs);
}

bool ModifiesS::isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                    std::string lhs, std::string rhs) {
  return pkb->isStatementModifies(lhs, rhs);
}

bool ModifiesS::queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                                domain_t &result) {
  result = pkb->getModifiesS(rhs, false);
  return true;
}

bool ModifiesS::queryRightDomain(std::shared_ptr<PKBGetter> pkb,
                                 std::string lhs, domain_t &result) {
  result = pkb->getModifiesS(lhs, true);
  return true;
}
