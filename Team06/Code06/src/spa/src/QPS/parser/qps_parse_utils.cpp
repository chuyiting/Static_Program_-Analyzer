//
// Created by Eddy Chu on 2022/10/2.
//

#include "qps_parse_utils.h"
#include <algorithm>
const std::string WHITESPACE = " \n\r\t\f\v";

std::regex IDENT("[A-Za-z][A-Za-z0-9]*");

bool qps_parse_utils::is_ident(std::string target) {
  return std::regex_match(target, IDENT);
}

bool qps_parse_utils::is_integer(std::string target) {
  return std::all_of(target.begin(), target.end(),
                     [](char c) { return isdigit(c) != 0; });
}

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string qps_parse_utils::trim(const std::string &s) {
  return rtrim(ltrim(s));
}

void qps_parse_utils::checkNotEnd(QPSAbstractParser::TokenIterator i,
                                  QPSAbstractParser::TokenIterator end) {
  if (i == end) {
    throw QPSException(QPSParseExceptionMessage::QPS_PARSER_SYNTAX_ERROR_PQL);
  }
}

void qps_parse_utils::expectHasNext(QPSAbstractParser::TokenIterator i,
                                    QPSAbstractParser::TokenIterator end) {
  checkNotEnd(i + 1, end);
}

QPSAbstractParser::TokenIterator
qps_parse_utils::nextToken(QPSAbstractParser::TokenIterator i,
                           QPSAbstractParser::TokenIterator end) {
  checkNotEnd(i + 1, end);
  return i + 1;
}

std::shared_ptr<RelRef>
qps_parse_utils::tokenToRef(const QueryToken &token,
                            std::shared_ptr<QPSQueryParserAPI> query) {
  if (token.getType() == WildcardToken) {
    return std::make_shared<Wildcard>();
  } else if (token.getType() != QuoteToken &&
             query->isSynonymDefined(
                 static_cast<const synonym_t &>(token.getValue()))) {
    return query->getSynonymFromLiteral(token.getValue());
  } else {
    return std::make_shared<Literal>(token);
  }
}
