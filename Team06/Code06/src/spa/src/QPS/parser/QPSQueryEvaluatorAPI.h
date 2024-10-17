//
// Created by Eddy Chu on 2022/10/15.
//

#ifndef SPA_QPSQUERYEVALUATORAPI_H
#define SPA_QPSQUERYEVALUATORAPI_H

#include "QPS/evaluator/QueryResult.h"
#include "QPS/parser/querystructs/patternclause/PatternClause.h"
#include "QPS/parser/querystructs/selectclause/SelectClause.h"
#include "QPS/parser/querystructs/suchthatclause/SuchThatClause.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/parser/querystructs/withclause/AttrCompare.h"

class Synonym;
class SuchThatClause;
class PatternClause;
class SelectClause;
class AttrCompare;

class QPSQueryEvaluatorAPI {
public:
  [[nodiscard]] virtual std::vector<std::shared_ptr<QPSConstraint>>
  getAllConstraints() const = 0;

  virtual EntityType getType(const synonym_t &synonym) const = 0;

  virtual QueryResultType getQueryResultType() const = 0;

  virtual bool isVarSelected(synonym_t var) = 0;

  virtual bool isBoolean() = 0;

  [[nodiscard]] virtual const std::unordered_set<synonym_t> &
  extractSelectedSynonyms() const = 0;

  [[nodiscard]] virtual const SelectClause &getSelectClause() const = 0;
};

#endif // SPA_QPSQUERYEVALUATORAPI_H
