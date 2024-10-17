//
// Created by Eddy Chu on 2022/9/1.
//
#pragma once

#include "QPS/evaluator/relationships/EntEntRelationship.h"
#include "QPS/evaluator/relationships/Relationship.h"
#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"
#include <unordered_set>
#include <memory>

class ModifiesP : public EntEntRelationship {
public:
  ModifiesP(EntRef lhs, EntRef rhs);

protected:
  bool isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                           std::string rhs) override;

  bool queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                       domain_t &result) override;

  bool queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                        domain_t &result) override;
};
