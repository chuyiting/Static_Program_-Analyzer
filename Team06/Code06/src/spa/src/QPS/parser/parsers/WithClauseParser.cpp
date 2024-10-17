//
// Created by Eddy Chu on 2022/10/13.
//

#include "WithClauseParser.h"

QPSAbstractParser::TokenIterator
WithClauseParser::tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                           QPSAbstractParser::TokenIterator begin,
                           QPSAbstractParser::TokenIterator end) {
  auto i = begin;
  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }

  do {
    i = tryParseAttrCompare(result, i, end);
  } while (i != end && (*i) == "and" && (++i) != end);

  return i;
}

QPSAbstractParser::TokenIterator
WithClauseParser::tryParseAttrCompare(std::shared_ptr<QPSQueryParserAPI> result,
                                      QPSAbstractParser::TokenIterator begin,
                                      QPSAbstractParser::TokenIterator end) {
  auto i = begin;
  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }

  auto [lhs, next] = tryParseWithRef(result, i, end);
  i = next;

  if (i == end || (*i) != "=" || (i + 1) == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }

  i = i + 1;
  auto [rhs, next2] = tryParseWithRef(result, i, end);

  auto ac = std::make_shared<AttrCompare>(lhs, rhs);
  result->addAttrCompare(ac);

  return next2;
}

std::pair<std::shared_ptr<WithRef>, QPSAbstractParser::TokenIterator>
WithClauseParser::tryParseWithRef(std::shared_ptr<QPSQueryParserAPI> result,
                                  QPSAbstractParser::TokenIterator begin,
                                  QPSAbstractParser::TokenIterator end) {
  auto i = begin;
  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }

  // IDENT
  if (i->getType() == QuoteToken) {
    return {std::make_shared<LiteralWithRef>((*i).getValue()), i + 1};
  }

  // Integer
  if (i->getType() == IntegerToken) {
    return {std::make_shared<IntegerWithRef>((*i).getValue()), i + 1};
  }

  // Attribute Reference
  auto synonym = result->getSynonymFromLiteral((*i).getValue());
  if ((i + 1) == end || (i + 1)->getType() != DotToken || (i + 2) == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WITH_CLAUSE_SYNTAX);
  }

  auto attrType = toAttributeType((i + 2)->getValue());
  if (attrType == AttributeType::UNKNOWN) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WITH_CLAUSE_SYNTAX);
  }

  return {std::make_shared<AttrWithRef>(*synonym, attrType), i + 3};
}
