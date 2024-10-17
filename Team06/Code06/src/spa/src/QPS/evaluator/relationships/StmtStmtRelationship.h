//
// Created by Eddy Chu on 2022/10/11.
//

#ifndef SPA_STMTSTMTRELATIONSHIP_H
#define SPA_STMTSTMTRELATIONSHIP_H

#include "QPS/evaluator/relationships/relationshipStructs/StmtRef.h"
#include "Relationship.h"

class StmtStmtRelationship : public Relationship {
protected:
  StmtRef lhs;
  StmtRef rhs;

public:
  bool isSatisfied(std::shared_ptr<PKBGetter> pkb, assign_t &assign) override;

  bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                   orderDomain_t &orderDomain,
                   orderDomain_t &queryResult) override;

  std::vector<synonym_t> getSynonyms() override;

  bool operator==(const StmtStmtRelationship &other);

  bool operator!=(const StmtStmtRelationship &other);

  bool operator==(const Relationship &other) override;

  bool operator!=(const Relationship &other) override;
};

#endif // SPA_STMTSTMTRELATIONSHIP_H
