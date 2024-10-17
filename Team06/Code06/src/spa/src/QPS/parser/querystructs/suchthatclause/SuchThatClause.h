#pragma once

#include "QPS/evaluator/QPSConstraint.h"
#include "QPS/evaluator/relationships/Relationship.h"

class Relationship;

class SuchThatClause : public QPSConstraint {

  std::shared_ptr<Relationship> relationship{};

public:
  SuchThatClause() = default;

  explicit SuchThatClause(std::shared_ptr<Relationship> rel)
      : relationship(std::move(rel)) {}

  void setRelationship(std::shared_ptr<Relationship> rel);

  bool isSatisfied(std::shared_ptr<PKBGetter> pkb, assign_t &assign) override;

  std::vector<synonym_t> getVariables() override;

  bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                   orderDomain_t &orderDomain,
                   orderDomain_t &queryResult) override;

  int priority() override;

  bool operator==(const SuchThatClause &other) const;

  bool operator!=(const SuchThatClause &other) const;
};