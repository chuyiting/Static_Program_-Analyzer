//
// Created by Eddy Chu on 2022/10/11.
//

#ifndef SPA_ENTENTRELATIONSHIP_H
#define SPA_ENTENTRELATIONSHIP_H

#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"
#include "Relationship.h"

class EntEntRelationship : public Relationship {

protected:
  EntRef lhs;
  EntRef rhs;

public:
  bool isSatisfied(std::shared_ptr<PKBGetter> pkb, assign_t &assign) override;

  bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                   orderDomain_t &orderDomain,
                   orderDomain_t &queryResult) override;

  std::vector<synonym_t> getSynonyms() override;

  bool operator==(const EntEntRelationship &other);

  bool operator!=(const EntEntRelationship &other);

  bool operator==(const Relationship &other) override;

  bool operator!=(const Relationship &other) override;
};

#endif // SPA_ENTENTRELATIONSHIP_H
