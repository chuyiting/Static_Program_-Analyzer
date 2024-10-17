//
// Created by Eddy Chu on 2022/9/14.
//

#include "QPS.h"
#include "QPS/evaluator/QueryEvaluator.h"
#include "QPS/exceptions/QPSException.h"

std::shared_ptr<QueryResult> QPS::run(std::string query) const {
  QueryEvaluator evaluator = QueryEvaluator(pkb);
  auto result = std::make_shared<QueryResult>();

  try {
    auto tokens = QueryTokenizer::tokenize(query);
    auto queryData = QPSParser::parse(tokens);
    result = evaluator.evaluate(queryData);
  } catch (QPSException &e) {
    result->addErrorResult(e);
  }

  return result;
}

QPS::QPS(std::shared_ptr<PKBGetter> pkb) : pkb(std::move(pkb)) {}
