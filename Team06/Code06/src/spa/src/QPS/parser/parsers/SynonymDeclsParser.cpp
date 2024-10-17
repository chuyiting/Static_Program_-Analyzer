//
// Created by Eddy Chu on 2022/10/13.
//

#include "SynonymDeclsParser.h"
#include "../qps_parse_utils.h"

QPSAbstractParser::TokenIterator
SynonymDeclsParser::tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                             TokenIterator begin, TokenIterator end) {
  auto next = begin;
  while (true) {
    try {
      next = tryParseSynonymDecl(result, next, end);
    } catch (QPSException &e) {
      break;
    }
  }

  return next;
}

QPSAbstractParser::TokenIterator SynonymDeclsParser::tryParseSynonymDecl(
    std::shared_ptr<QPSQueryParserAPI> result, TokenIterator begin,
    TokenIterator end) {

  auto i = begin;
  qps_parse_utils::checkNotEnd(i, end);
  EntityType entityType = getEntityType(i);

  i = qps_parse_utils::nextToken(i, end);
  Synonym synonym = Synonym{entityType, i->getValue()};
  checkNoRepeatedSynonym(synonym, result);
  result->addSynonymDecl(synonym);

  i = qps_parse_utils::nextToken(i, end);
  while (true) {
    if (*i == ";") {
      i++;
      break;
    }

    if (i == end || *i != "," || (i + 1) == end) {
      throw QPSException(QPSParseExceptionMessage::
                             QPS_PARSER_SYNONYM_INVALID_SYNONYM_DECLARATION);
    }

    i++;
    synonym = Synonym{entityType, i->getValue()};

    if (result->isSynonymDefined(synonym.getSynonymName())) {
      throw QPSException(
          QPSParseExceptionMessage::QPS_PARSER_SYNONYM_REDECLARED,
          QpsExceptionType::SEMANTIC);
    }
    result->addSynonymDecl(synonym);
    i++;
  }

  return i;
}

EntityType
SynonymDeclsParser::getEntityType(QPSAbstractParser::TokenIterator it) {
  return toEntityType(it->getValue());
}

void SynonymDeclsParser::checkNoRepeatedSynonym(
    Synonym &synonym, std::shared_ptr<QPSQueryParserAPI> result) {
  if (result->isSynonymDefined(synonym.getSynonymName())) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_REPEATED_SYNONYM_DECLARE);
  }
}
