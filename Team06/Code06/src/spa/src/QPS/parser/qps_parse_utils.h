//
// Created by Eddy Chu on 2022/9/7.
//

#ifndef SPA_QPS_PARSE_UTILS_H
#define SPA_QPS_PARSE_UTILS_H

#include "QPS/parser/parsers/QPSAbstractParser.h"
#include <regex>

namespace qps_parse_utils {

bool is_ident(std::string target);

bool is_integer(std::string target);

std::string trim(const std::string &s);

/**
 * Until function to check if the iterator is not end
 * @QPSException thrown if i is equal to end
 */
void checkNotEnd(QPSAbstractParser::TokenIterator i,
                 QPSAbstractParser::TokenIterator end);

/**
 * Util function to check if the next iterator is not end
 * @QPSException thrown if i+1 is equal to end
 */
void expectHasNext(QPSAbstractParser::TokenIterator i,
                   QPSAbstractParser::TokenIterator end);

QPSAbstractParser::TokenIterator
nextToken(QPSAbstractParser::TokenIterator i,
          QPSAbstractParser::TokenIterator end);

std::shared_ptr<RelRef> tokenToRef(const QueryToken &token,
                                   std::shared_ptr<QPSQueryParserAPI> query);

} // namespace qps_parse_utils

#endif // SPA_QPS_PARSE_UTILS_H
