//
// Created by Eddy Chu on 2022/10/13.
//

#include "SelectClauseParser.h"

QPSAbstractParser::TokenIterator
SelectClauseParser::tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                             QPSAbstractParser::TokenIterator begin,
                             QPSAbstractParser::TokenIterator end) {

  auto i = begin;
  if (i == end || !((*i) == KEYWORD_SELECT)) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_MISSING_SELECT_STATEMENT);
  }

  i++;

  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_ILLEGAL_SELECT_STATEMENT);
  }

  if (*i == KEYWORD_BOOLEAN && !result->isSynonymDefined(i->getValue())) {
    return i + 1;
  }

  if (*i != "<") {
    // single synonym
    return tryParseSelectTarget(result, i, end);
  }

  // multiple synonyms
  i++;
  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_ILLEGAL_SELECT_STATEMENT);
  }
  i = tryParseSelectTarget(result, i, end);

  while (i != end && *i == ",") {
    if (i + 1 == end) {
      throw QPSException(
          QPSParseExceptionMessage::QPS_PARSER_ILLEGAL_SELECT_STATEMENT);
    }
    i = tryParseSelectTarget(result, i + 1, end);
  }

  if (i == end || *i != ">") {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_ILLEGAL_SELECT_STATEMENT);
  }

  return i + 1;
}

QPSAbstractParser::TokenIterator SelectClauseParser::tryParseSelectTarget(
    std::shared_ptr<QPSQueryParserAPI> result,
    QPSAbstractParser::TokenIterator begin,
    QPSAbstractParser::TokenIterator end) {
  auto i = begin;
  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_INVALID_SELECT_SYNONYM_NOT_DEFINED,
        QpsExceptionType::SYNTACTIC);
  }

  synonym_t synonymStr = i->getValue();
  if (!result->isSynonymDefined(synonymStr)) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_INVALID_SELECT_SYNONYM_NOT_DEFINED,
        QpsExceptionType::SEMANTIC);
  }

  auto synonym = result->getSynonymFromLiteral(synonymStr);
  SelectTarget selectTarget = SelectTarget(synonym);
  i++;

  // handle attribute
  if (i != end && i->getType() == DotToken) {
    if ((i + 1) == end || (i + 1)->getType() != AttributeToken) {
      throw QPSException(
          QPSParseExceptionMessage::QPS_PARSER_ILLEGAL_SELECT_STATEMENT);
    }

    AttributeType attribute = toAttributeType((i + 1)->getValue());
    if (attribute == AttributeType::UNKNOWN ||
        !isValidAttribute(synonym->getSynonymType(), attribute)) {
      throw QPSException(
          QPSParseExceptionMessage::QPS_PARSER_ILLEGAL_SELECT_STATEMENT);
    }

    selectTarget = SelectTarget(synonym, attribute);
    i += 2;
  }

  result->addSelectTarget(selectTarget);
  return i;
}
