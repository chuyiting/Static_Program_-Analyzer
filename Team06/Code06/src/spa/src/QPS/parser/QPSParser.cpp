//
// Created by Eddy Chu on 2022/8/31.
//

#include "QPSParser.h"
#include "Exceptions/ParserException.h"
#include "QPS/evaluator/relationships/Relationship.h"
#include "QPS/parser/parsers/ClauseParser.h"
#include "QPS/parser/parsers/SelectClauseParser.h"
#include "QPS/parser/parsers/SynonymDeclsParser.h"
#include "QPS/parser/querystructs/suchthatclause/Literal.h"
#include "utils/ExpressionHandler/ExpressionHandler.h"

std::shared_ptr<QPSQuery>
QPSParser::parse(const std::vector<QueryToken> &tokens) {
  auto result = std::make_shared<QPSQuery>();
  auto begin = tokens.cbegin();
  auto end = tokens.cend();

  begin = SynonymDeclsParser().tryParse(result, begin, end);
  begin = SelectClauseParser().tryParse(result, begin, end);
  ClauseParser().tryParse(result, begin, end);
  return result;
}
