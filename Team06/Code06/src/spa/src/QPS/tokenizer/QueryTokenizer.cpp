#include "QueryTokenizer.h"
#include "QPS/exceptions/QPSException.h"
#include "QPS/parser/qps_parse_utils.h"
#include "QPS/types/AttributeType.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <vector>

/**
 * Check if a character is defined punctuator: ';', '(', '), ','
 * @param c character to be checked
 * @return whether the character is a punctuator character or not
 */
inline bool isPunctuator(const char c) {
  return (c == ';' || c == '(' || c == ')' || c == ',' || c == '<' || c == '>');
}

/**
 * Check if the token is a valid string token(start with alphabet)
 * @param token string
 * @return whether a token is valid
 */
inline bool isValidStringToken(const std::string token) {
  return token.length() != 0 && isalpha(token[0]);
}

/**
 * Check whether the token is a valid digital token
 * @param token string token to be checked
 * @return
 */
inline bool isDigitalToken(const std::string token) {
  if (token.length() == 0) {
    return false;
  }

  bool isAllDigit = std::all_of(token.begin(), token.end(),
                                [](char c) { return std::isdigit(c); });

  return isAllDigit && int(token[0]) > 0;
}

/**
 * Check if the character is a valid character. Valid characters include
 * alphanum and '_' and '#'
 * @param c character to be checked
 * @return
 */
inline bool isValidCharacter(const char c) {
  return std::isalnum(c) || c == '_' || c == '#';
}

std::vector<QueryToken>
QueryTokenizer::tokenize(const std::string &queryString) {
  std::vector<QueryToken> tokenList;
  auto iter = queryString.cbegin();
  auto end = queryString.cend();

  // scan character by character
  while (iter != end) {
    // skip spaces
    if (std::isspace(*iter)) {
      iter++;
      continue;
    }

    auto [token, next] = next_token(iter, end);
    tokenList.push_back(token);
    iter = next;
  }

  return tokenList;
}

std::pair<QueryToken, QueryTokenizer::StrIter>
QueryTokenizer::next_token(StrIter begin, StrIter end) {
  // copy pointer
  auto iter = begin;
  std::string tmpStr;

  // handle dot token - namely '.'
  if (*iter == '.') {
    return {QueryToken(".", DotToken), iter + 1};
  }

  // handle equal token - namely '='
  if (*iter == '=') {
    return {QueryToken("=", EqualToken), iter + 1};
  }

  // handle wildcard token
  if (*iter == '_') {
    tmpStr.push_back(*iter);
    return {QueryToken(tmpStr, WildcardToken), iter + 1};
  }

  // handle punctuator - namely ';', '(',')',
  if (isPunctuator(*iter)) {
    tmpStr.push_back(*iter);
    return {QueryToken(tmpStr, PunctuatorToken), iter + 1};
  }

  // handle quote token
  if (*iter == '\"') {

    // loop until finding the right quotation
    iter++;
    if (iter == end) {
      throw QPSException(
          QPSTokenizerExceptionMessage::QPS_TOKENIZER_INVALID_TOKEN);
    }

    while (iter != end && *iter != '\"') {
      tmpStr.push_back(*iter);
      iter++;
    }

    // missing right quote token
    if (iter == end) {
      throw QPSException(
          QPSTokenizerExceptionMessage::QPS_TOKENIZER_INVALID_TOKEN);
    }

    tmpStr = qps_parse_utils::trim(tmpStr);
    // empty quote
    if (tmpStr.length() == 0) {
      throw QPSException(
          QPSTokenizerExceptionMessage::QPS_TOKENIZER_INVALID_TOKEN);
    }

    return {QueryToken(tmpStr, QuoteToken), iter + 1};
  }

  // handle general case
  while (iter != end && isValidCharacter(*iter)) {
    tmpStr.push_back(*iter);
    iter++;
  }

  // handle integer token
  if (isDigitalToken(tmpStr)) {
    return {QueryToken(tmpStr, IntegerToken), iter};
  }

  // handle attribute token
  if (isValidAttributeName(tmpStr)) {
    return {QueryToken(tmpStr, AttributeToken), iter};
  }

  // handle string token
  if (!isValidStringToken(tmpStr)) {
    throw QPSException(
        QPSTokenizerExceptionMessage::QPS_TOKENIZER_INVALID_TOKEN,
        QpsExceptionType::SEMANTIC);
  }

  // handle 'such that'
  if (tmpStr == "such") {
    auto ptrCopy = iter;
    bool hasSpace = false;
    while (std::isspace(*iter)) {
      hasSpace = true;
      iter++;
    }

    while (isalpha(*iter)) {
      if (!hasSpace)
        break;

      tmpStr.push_back(*iter);
      iter++;
    }

    if (tmpStr == "suchthat") {
      return {QueryToken("such that", StringToken), iter};
    } else if (hasSpace) {
      // convert back to such when tmpstr is not such that
      tmpStr = "such";
      iter = ptrCopy;
    }
  }

  // handle special postfix aka asterisk
  if (iter != end && *iter == '*' &&
      (tmpStr == "Follows" || tmpStr == "Parent" || tmpStr == "Calls" ||
       tmpStr == "Next" || tmpStr == "Affects")) {
    tmpStr.push_back(*iter);
    return {QueryToken(tmpStr, StringToken), iter + 1};
  } else if (iter != end && *iter == '*') {
    throw QPSException(
        QPSTokenizerExceptionMessage::QPS_TOKENIZER_INVALID_TOKEN);
  }

  return {QueryToken(tmpStr, StringToken), iter};
}
