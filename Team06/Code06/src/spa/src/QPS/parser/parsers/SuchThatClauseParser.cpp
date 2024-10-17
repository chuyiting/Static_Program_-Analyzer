//
// Created by Eddy Chu on 2022/10/13.
//

#include "SuchThatClauseParser.h"
#include "QPS/evaluator/relationships/RelationshipFactory.h"
#include "QPS/parser/qps_parse_utils.h"

inline void throwSuchThatException() {
  throw QPSException(
      QPSParseExceptionMessage::QPS_PARSER_INVALID_SUCH_THAT_CLAUSE_SYNTAX);
}

QPSAbstractParser::TokenIterator
SuchThatClauseParser::tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                               QPSAbstractParser::TokenIterator begin,
                               QPSAbstractParser::TokenIterator end) {
  auto i = begin;
  if (begin == end) {
    throwSuchThatException();
  }

  QueryToken rel = *i;
  i++;
  if (i == end || !(*i == "(")) {
    throwSuchThatException();
  }

  i++;
  QueryToken lhsToken = *i;
  auto lhs = qps_parse_utils::tokenToRef(lhsToken, result);

  i++;
  if (i == end || !(*i == ",")) {
    throwSuchThatException();
  }

  i++;
  if (i == end) {
    throwSuchThatException();
  }

  QueryToken rhsToken = *i;
  auto rhs = qps_parse_utils::tokenToRef(rhsToken, result);
  i++;
  if (i == end || !(*i == ")")) {
    throwSuchThatException();
  }
  i++;

  std::shared_ptr<Relationship> relationship =
      RelationshipFactory::createRelationship(rel, lhs, rhs);
  auto stc = std::make_shared<SuchThatClause>();
  stc->setRelationship(relationship);
  result->addSuchThatClause(stc);

  return i;
}
