//
// Created by Eddy Chu on 2022/10/13.
//

#include "ClauseParser.h"

QPSAbstractParser::TokenIterator
ClauseParser::tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                       QPSAbstractParser::TokenIterator begin,
                       QPSAbstractParser::TokenIterator end) {
  while (begin != end) {
    begin = tryParseClause(result, begin, end);
  }
  return begin;
}

QPSAbstractParser::TokenIterator
ClauseParser::tryParseClause(std::shared_ptr<QPSQueryParserAPI> result,
                             QPSAbstractParser::TokenIterator begin,
                             QPSAbstractParser::TokenIterator end) {
  auto i = begin;
  if (i == end) {
    return end;
  }

  if (*i == KEYWORD_PATTERN ||
      (*i == KEYWORD_AND && this->currentCtx == KEYWORD_PATTERN)) {
    this->currentCtx = KEYWORD_PATTERN;
    return pcp.tryParse(result, i + 1, end);
  }

  if (*i == KEYWORD_SUCHTHAT ||
      (*i == KEYWORD_AND && this->currentCtx == KEYWORD_SUCHTHAT)) {
    this->currentCtx = KEYWORD_SUCHTHAT;
    return stcp.tryParse(result, i + 1, end);
  }

  if (*i == KEYWORD_WITH) {
    return wcp.tryParse(result, i + 1, end);
  }

  throw QPSException(QPSParseExceptionMessage::QPS_INVALID_CLAUSE);
}
