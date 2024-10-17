//
// Created by Eddy Chu on 2022/10/13.
//

#include "PatternClauseParser.h"
#include "QPS/parser/qps_parse_utils.h"
#include "utils/ExpressionHandler/ExpressionHandler.h"

QPSAbstractParser::TokenIterator
PatternClauseParser::tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                              QPSAbstractParser::TokenIterator begin,
                              QPSAbstractParser::TokenIterator end) {
  auto i = begin;
  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }

  auto synAssignToken = *i;
  auto synPattern = result->getSynonymFromLiteral(synAssignToken.getValue());

  if (synPattern->getSynonymType() != EntityType::ASSIGN &&
      synPattern->getSynonymType() != EntityType::IF &&
      synPattern->getSynonymType() != EntityType::WHILE) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX,
        QpsExceptionType::SEMANTIC);
  }

  i++;
  if (i == end || *i != KEYWORD_LEFT_PARAM || (i + 1) == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }

  i++;
  auto lhsToken = *i;
  auto lhsRef = qps_parse_utils::tokenToRef(lhsToken, result);
  auto lhs = std::make_shared<EntRef>(lhsRef);

  if ((i + 1) == end || *(i + 1) != KEYWORD_COMMA || (i + 2) == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }
  i = i + 2;

  if (synPattern->getSynonymType() == EntityType::ASSIGN) {
    i = tryParseAssignPatternClause(synPattern, lhs, result, i, end);
  } else if (synPattern->getSynonymType() == EntityType::WHILE) {
    i = tryParseWhilePatternClause(synPattern, lhs, result, i, end);
  } else if (synPattern->getSynonymType() == EntityType::IF) {
    i = tryParseIfPatternClause(synPattern, lhs, result, i, end);
  } else {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }

  return i;
}

QPSAbstractParser::TokenIterator
PatternClauseParser::tryParseAssignPatternClause(
    std::shared_ptr<Synonym> syn, std::shared_ptr<EntRef> lhs,
    std::shared_ptr<QPSQueryParserAPI> result, TokenIterator begin,
    TokenIterator end) {
  auto i = begin;
  ExpressionSpec rhs = ExpressionSpec();
  QueryToken rhsToken = *i;
  if (rhsToken.getType() == WildcardToken) {
    i++;

    if (i == end) {
      throw QPSException(
          QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
    }
    if (*i == KEYWORD_RIGHT_PARAM) {
      rhs = ExpressionSpec(); // default constructor is wildcard constructor
      i = i + 1;
    } else if (i->getType() == QuoteToken && (i + 1) != end &&
               (i + 1)->getType() == WildcardToken && (i + 2) != end &&
               *(i + 2) == KEYWORD_RIGHT_PARAM) {
      checkExpression(i->getValue());
      rhs = ExpressionSpec(true, i->getValue()); // partial match
      i = i + 3;
    } else {
      throw QPSException(
          QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
    }
  } else if (rhsToken.getType() == QuoteToken && (i + 1) != end &&
             *(i + 1) == KEYWORD_RIGHT_PARAM) {
    checkExpression(i->getValue());
    rhs = ExpressionSpec(false, i->getValue()); // exact match
    i = i + 2;
  } else {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }
  auto pc = std::make_shared<PatternClause>(*syn, *lhs, rhs);
  result->addPatternClause(pc);
  return i;
}

QPSAbstractParser::TokenIterator
PatternClauseParser::tryParseWhilePatternClause(
    std::shared_ptr<Synonym> syn, std::shared_ptr<EntRef> lhs,
    std::shared_ptr<QPSQueryParserAPI> result, TokenIterator begin,
    TokenIterator end) {
  auto i = begin;
  ExpressionSpec rhs = ExpressionSpec(false, "_");
  if (i->getType() != WildcardToken) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }
  i++;

  if (i == end) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }
  if (*i == KEYWORD_RIGHT_PARAM) {
    auto pc = std::make_shared<PatternClause>(*syn, *lhs, rhs);
    result->addPatternClause(pc);
    i = i + 1;
  }
  return i;
}

QPSAbstractParser::TokenIterator PatternClauseParser::tryParseIfPatternClause(
    std::shared_ptr<Synonym> syn, std::shared_ptr<EntRef> lhs,
    std::shared_ptr<QPSQueryParserAPI> result, TokenIterator begin,
    TokenIterator end) {
  auto i = begin;
  ExpressionSpec rhs = ExpressionSpec(false, "_");
  if (i->getType() != WildcardToken ||
      ((i + 1) == end || *(i + 1) != KEYWORD_COMMA || (i + 2) == end)) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }
  i = i + 2;
  if (i == end || i + 1 == end || i->getType() != WildcardToken ||
      *(i + 1) != KEYWORD_RIGHT_PARAM) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX);
  }
  auto pc = std::make_shared<PatternClause>(*syn, *lhs, rhs);
  result->addPatternClause(pc);
  i = i + 2;
  return i;
}

void PatternClauseParser::checkExpression(std::string exp) {
  try {
    ExpressionHandler e = ExpressionHandler(ExpressionType::EXPRESSION, exp);
    e.parse();
  } catch (ParserException e) {
    throw QPSException("");
  } catch (TokenizerException e) {
    throw QPSException("");
  }
}
