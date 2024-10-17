//
// Created by Eddy Chu on 2022/9/14.
//

#ifndef SPA_QPS_H
#define SPA_QPS_H

#include "PKB/PKB.h"
#include "QPS/evaluator/QueryEvaluator.h"
#include "QPS/parser/QPSParser.h"
#include "QPS/tokenizer/QueryTokenizer.h"
#include <string>

/**
 * The API for the whole QPS system
 */
class QPS {
  std::shared_ptr<PKBGetter> pkb;

public:
  QPS() = delete;

  /**
   * The constructor for QPS
   * @param pkb QPS execution requires PKB, pass in the shared pointer of
   * PKBGetter to construct QPS
   */
  explicit QPS(std::shared_ptr<PKBGetter> pkb);

  /**
   * The <b>ONE AND ONLY ONE API OF QPS</b>.
   * It takes in PQL string and return QueryResult
   * @param query PQL query in {@code string} format
   * @return shared pointer pointing to the QueryResult
   */
  [[nodiscard]] std::shared_ptr<QueryResult> run(std::string query) const;
};

#endif // SPA_QPS_H

