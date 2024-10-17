//
// Created by Eddy Chu on 2022/10/13.
//

#ifndef SPA_CLAUSEPARSER_H
#define SPA_CLAUSEPARSER_H

#include "PatternClauseParser.h"
#include "QPSAbstractParser.h"
#include "SuchThatClauseParser.h"
#include "WithClauseParser.h"

class ClauseParser : public QPSAbstractParser {

  SuchThatClauseParser stcp;
  PatternClauseParser pcp;
  WithClauseParser wcp;
  std::string currentCtx;

  inline static const std::string KEYWORD_PATTERN = "pattern";
  inline static const std::string KEYWORD_SUCHTHAT = "such that";
  inline static const std::string KEYWORD_AND = "and";
  inline static const std::string KEYWORD_WITH = "with";

  TokenIterator tryParseClause(std::shared_ptr<QPSQueryParserAPI> result,
                               TokenIterator begin, TokenIterator end);

public:
  TokenIterator tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                         TokenIterator begin, TokenIterator end) override;
};

#endif // SPA_CLAUSEPARSER_H
