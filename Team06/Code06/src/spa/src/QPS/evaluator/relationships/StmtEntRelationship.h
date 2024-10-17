//
// Created by Eddy Chu on 2022/10/11.
//

#ifndef SPA_STMTENTRELATIONSHIP_H
#define SPA_STMTENTRELATIONSHIP_H

#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"
#include "QPS/evaluator/relationships/relationshipStructs/StmtRef.h"
#include "Relationship.h"
#include <vector>
#include <memory>

class StmtEntRelationship : public Relationship {

protected:
  StmtRef lhs;
  EntRef rhs;

public:
  bool isSatisfied(std::shared_ptr<PKBGetter> pkb, assign_t &assign) override;

  bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                   orderDomain_t &orderDomain,
                   orderDomain_t &queryResult) override;


  std::vector<synonym_t> getSynonyms() override;

  bool operator==(const StmtEntRelationship &other);

  bool operator!=(const StmtEntRelationship &other);

  bool operator==(const Relationship &other) override;

  bool operator!=(const Relationship &other) override;
};

#endif // SPA_STMTENTRELATIONSHIP_H
