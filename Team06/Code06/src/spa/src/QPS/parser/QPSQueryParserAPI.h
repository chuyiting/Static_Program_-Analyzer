//
// Created by Eddy Chu on 2022/10/15.
//

#ifndef SPA_QPSQUERYPARSERAPI_H
#define SPA_QPSQUERYPARSERAPI_H

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

class QPSQueryParserAPI {
public:
  virtual void addSynonymDecl(Synonym &synonymDecl) = 0;

  virtual void addSuchThatClause(std::shared_ptr<SuchThatClause> clause) = 0;

  virtual void addPatternClause(std::shared_ptr<PatternClause> clause) = 0;

  virtual void addAttrCompare(std::shared_ptr<AttrCompare> attrCompare) = 0;

  virtual void addSelectTarget(SelectTarget selectTarget) = 0;

  virtual bool isSynonymDefined(const synonym_t &synonym) const = 0;

  /**
   * Method used to extract Synonym from synonym declarations.
   * @param literal the string literal to be checked
   * @throws QPSException is Synonym not found
   * @return the pointer for the target synonym
   */
  virtual std::shared_ptr<Synonym>
  getSynonymFromLiteral(std::string literal) const = 0;
};

#endif // SPA_QPSQUERYPARSERAPI_H
