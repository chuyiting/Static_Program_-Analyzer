//
// Created by Eddy Chu on 2022/10/13.
//

#ifndef SPA_PATTERNCLAUSEPARSER_H
#define SPA_PATTERNCLAUSEPARSER_H

#include "QPSAbstractParser.h"

class PatternClauseParser : public QPSAbstractParser {
  static TokenIterator
  tryParseAssignPatternClause(std::shared_ptr<Synonym> syn,
                              std::shared_ptr<EntRef> lhs,
                              std::shared_ptr<QPSQueryParserAPI> result,
                              TokenIterator begin, TokenIterator end);

  static TokenIterator
  tryParseWhilePatternClause(std::shared_ptr<Synonym> syn,
                             std::shared_ptr<EntRef> lhs,
                             std::shared_ptr<QPSQueryParserAPI> result,
                             TokenIterator begin, TokenIterator end);

  static TokenIterator
  tryParseIfPatternClause(std::shared_ptr<Synonym> syn,
                          std::shared_ptr<EntRef> lhs,
                          std::shared_ptr<QPSQueryParserAPI> result,
                          TokenIterator begin, TokenIterator end);

  static void checkExpression(std::string exp);

  inline static const std::string KEYWORD_LEFT_PARAM = "(";
  inline static const std::string KEYWORD_RIGHT_PARAM = ")";
  inline static const std::string KEYWORD_COMMA = ",";

public:
  TokenIterator tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                         TokenIterator begin, TokenIterator end) override;
};

#endif // SPA_PATTERNCLAUSEPARSER_H
