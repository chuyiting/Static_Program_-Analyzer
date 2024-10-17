//
// Created by Eddy Chu on 2022/9/8.
//

#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#include "ExpressionSpec.h"
#include "QPS/evaluator/QPSConstraint.h"
#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"
#include "QPS/evaluator/relationships/relationshipStructs/StmtRef.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include <string>
#include <memory>

class PatternClause : public QPSConstraint {
  Synonym syn;
  EntRef lhs;
  ExpressionSpec rhs;

  const std::unordered_set<EntityType> validPatternSyn = {
      EntityType::ASSIGN, EntityType::IF, EntityType::WHILE};

  void checkSynonym(const Synonym &syn);
  void checkLhs(const EntRef &ent);

public:
  PatternClause(const Synonym &syn, const EntRef &lhs,
                const ExpressionSpec &rhs);

  bool operator==(const PatternClause &other) const {
    return syn == other.syn && lhs == other.lhs && rhs == other.rhs;
  };

  bool operator!=(const PatternClause &other) const {
    return !((*this) == other);
  };

  bool isSatisfied(std::shared_ptr<PKBGetter> pkb, assign_t &assign) override;

  std::vector<synonym_t> getVariables() override;

  bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                   orderDomain_t &orderDomain,
                   orderDomain_t &queryResult) override;

  int priority() override;
};

#endif // SPA_PATTERNCLAUSE_H
