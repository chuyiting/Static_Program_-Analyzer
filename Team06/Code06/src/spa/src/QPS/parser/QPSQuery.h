//
// Created by Eddy Chu on 2022/8/31.
//

#ifndef INC_22S1_CP_SPA_TEAM_06_QPSQUERY_H
#define INC_22S1_CP_SPA_TEAM_06_QPSQUERY_H

#include "QPS/parser/QPSQueryEvaluatorAPI.h"
#include "QPS/parser/QPSQueryParserAPI.h"
#include "QPS/parser/querystructs/patternclause/PatternClause.h"
#include "QPS/parser/querystructs/selectclause/SelectClause.h"
#include "QPS/parser/querystructs/suchthatclause/SuchThatClause.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/parser/querystructs/withclause/AttrCompare.h"
#include <string>
#include <unordered_map>
#include <vector>

class Synonym;
class SuchThatClause;
class PatternClause;
class SelectClause;
class AttrCompare;

class QPSQuery : public QPSQueryEvaluatorAPI, public QPSQueryParserAPI {
  std::unordered_map<synonym_t, Synonym> synonymDecls;
  std::vector<std::shared_ptr<SuchThatClause>> stcs;
  std::vector<std::shared_ptr<PatternClause>> pcs;
  std::vector<std::shared_ptr<AttrCompare>> acs;
  SelectClause selectClause;

public:
  QPSQuery() = default;

  ////////////////////////////////////////////////
  //////////// Parser Related APIs ////////////
  ////////////////////////////////////////////////

  void addSynonymDecl(Synonym &synonymDecl) override;

  void addSuchThatClause(std::shared_ptr<SuchThatClause> clause) override;

  void addPatternClause(std::shared_ptr<PatternClause> clause) override;

  void addAttrCompare(std::shared_ptr<AttrCompare> attrCompare) override;

  void addSelectTarget(SelectTarget selectTarget) override;

  bool isSynonymDefined(const synonym_t &synonym) const override;

  /**
   * Method used to extract Synonym from synonym declarations.
   * @param literal the string literal to be checked
   * @throws QPSException is Synonym not found
   * @return the pointer for the target synonym
   */
  std::shared_ptr<Synonym>
  getSynonymFromLiteral(std::string literal) const override;

  ////////////////////////////////////////////////
  //////////// Evaluator Related APIs ////////////
  ////////////////////////////////////////////////

  [[nodiscard]] const SelectClause &getSelectClause() const override;

  [[nodiscard]] std::vector<std::shared_ptr<QPSConstraint>>
  getAllConstraints() const override;

  EntityType getType(const synonym_t &synonym) const override;

  QueryResultType getQueryResultType() const override;

  /**
   * Check if a given input synonym is selected.
   */
  bool isVarSelected(synonym_t var) override;

  bool isBoolean() override;

  [[nodiscard]] const std::unordered_set<synonym_t> &
  extractSelectedSynonyms() const override;

  // for unit testing purpose
  QPSQuery(std::unordered_map<synonym_t, Synonym> synonymDecls,
           std::vector<std::shared_ptr<SuchThatClause>> stcs,
           std::vector<std::shared_ptr<PatternClause>> pcs,
           const SelectClause &selectClause);

  // for unit testing purpose
  void
  setSynonymDecls(const std::unordered_map<synonym_t, Synonym> &synonymDecls);

  // for unit testing purpose
  void setSelectClause(SelectClause &selectClause);

  // for unit testing purpose
  void setWithClause(const std::vector<std::shared_ptr<AttrCompare>> &acs);

  // for unit testing purpose
  [[nodiscard]] const std::unordered_map<synonym_t, Synonym> &
  getSynonymDecls() const;

  // for unit testing purpose
  const std::vector<std::shared_ptr<SuchThatClause>> &getStcs() const;

  bool operator==(const QPSQuery &other) const;

  bool operator!=(const QPSQuery &other) const;
};

#endif // INC_22S1_CP_SPA_TEAM_06_QPSQUERY_H
