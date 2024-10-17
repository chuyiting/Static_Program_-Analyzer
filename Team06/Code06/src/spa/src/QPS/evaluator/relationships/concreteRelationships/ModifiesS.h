#pragma once

#include "QPS/evaluator/relationships/StmtEntRelationship.h"
#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"
#include "QPS/evaluator/relationships/relationshipStructs/StmtRef.h"
#include <utility>
#include <vector>
#include <memory>

class ModifiesS : public StmtEntRelationship {

public:
  ModifiesS(StmtRef lhs, EntRef rhs);

protected:
  bool isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                           std::string rhs) override;

  bool queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                       domain_t &result) override;

  bool queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                        domain_t &result) override;
};
