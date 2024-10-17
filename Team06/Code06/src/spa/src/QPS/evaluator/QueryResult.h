#pragma once

#include "CSPUtils.h"
#include "PKB/type/pkb_types.h"
#include "QPS/parser/querystructs/selectclause/SelectClause.h"
#include "QPSConstraint.h"
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace pkb_types;

enum class QueryResultType { SINGLE, TUPLE, BOOLEAN };

typedef std::vector<assign_t> interRes_t;
typedef std::vector<interRes_t> interResGroup_t;

class QueryResult {
  std::unordered_set<std::string> visitedCombs;
  interResGroup_t intermediateResultGroup;

  std::unordered_set<std::string> result;
  QueryResultType type = QueryResultType::SINGLE;

  inline static const std::string RETURN_LITERAL_NONE = "none";
  inline static const std::string RETURN_LITERAL_FALSE = "FALSE";
  inline static const std::string RETURN_LITERAL_TRUE = "TRUE";
  inline static const std::string FAILURE_PLACEHOLDER = "FAILURE";
  inline static const std::unordered_set<std::string> FAILURE_RESULT = {
      FAILURE_PLACEHOLDER};

  inline static int randomSeed = 0;

public:
  QueryResult();

  explicit QueryResult(QueryResultType type);

  // for testing
  explicit QueryResult(const std::unordered_set<std::string> &result,
                       QueryResultType type);

  ~QueryResult() = default;

  void addResult(std::string res);

  void addResult(const SelectClause &selectClause, assign_t &assign,
                 orderDomain_t &orderDomain, int groupId);

  void addFailureResult(const SelectClause &selectClause);

  void addErrorResult(QPSException &e);

  // Should clear visited combinations after each query group
  void clearVisitedCombs();
  /**
   * Util function for QPS evaluator to check if a given assignment has already
   * been passed. Designed for optimization purpose
   * @param selectClause The select clause of the current PQL
   * @param assign The current assignment for the backtracking algorithm
   * @return true if all selected synonyms have already been assigned and the
   * combination can be found in the current result list.
   */
  bool hasCombinationTested(std::shared_ptr<PKBGetter> pkb,
                            const SelectClause &selectClause, assign_t &assign);

  [[maybe_unused]] void printResult();

  [[nodiscard]] std::string toString() const;

  bool operator==(const QueryResult &other) const;

  bool operator!=(const QueryResult &other) const;

  friend std::ostream &operator<<(std::ostream &out,
                                  QueryResult const &queryResult) {
    out << queryResult.toString();
    return out;
  }

  /**
   * Method used to turn intermediate result to final results
   * @param selectClause
   */
  void generateResults(std::shared_ptr<PKBGetter> pkb,
                       const SelectClause &selectClause);

  const std::unordered_set<std::string> &getResult();

private:
  std::string generateResultString(std::shared_ptr<PKBGetter> pkb,
                                   const SelectClause &selectClause,
                                   assign_t assignMap);

  std::string generateCombString(const SelectClause &selectClause,
                                 assign_t &assign);

  void cartesianProduct(interRes_t &interRes, interRes_t &interRes1,
                        interRes_t &interRes2);

  void cartesianProduct(const SelectClause &selectClause, interRes_t &interRes,
                        assign_t &assign, orderDomain_t &orderDomain);

  void addSingleAssign(interRes_t &interRes, const std::string &syn,
                       const domain_t &vals);
};

