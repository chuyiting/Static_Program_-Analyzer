//
// Created by Eddy Chu on 2022/9/10.
//

#ifndef SPA_QUERYEVALUATOR_H
#define SPA_QUERYEVALUATOR_H

#include "CSPUtils.h"
#include "PKB/PKBGetter.h"
#include "QPS/parser/QPSQuery.h"
#include "QPS/parser/QPSQueryEvaluatorAPI.h"
#include "QPSConstraint.h"
#include "QueryResult.h"

class QPSQuery;
class QPSQueryEvaluatorAPI;

class QueryEvaluator {
public:
  explicit QueryEvaluator(std::shared_ptr<PKBGetter> pkb);

  std::shared_ptr<QueryResult>
  evaluate(std::shared_ptr<QPSQueryEvaluatorAPI> query);

private:
  std::shared_ptr<PKBGetter> pkb;
  std::shared_ptr<QPSQueryEvaluatorAPI> query;
  std::unordered_map<synonym_t, int> priorityMap;
  constraints_t constraints;
  assigns_t assigns;
  orderDomain_t orderDomain;
  std::unordered_set<std::shared_ptr<QPSConstraint>> satisfiedConstraints;
  int currGroupId = 0;

  /**
   * Initialize the constraints map for each synonym and check satisfaction of
   * constraints without variable
   * @param constraints
   * @param query
   * @return false if there exists unsatisfied constraints
   */
  bool initConstraints();

  void initAssign();

  void initPriorityMap();

  bool initOrderDomain(assign_t &assign);

  bool updateOrderDomain(assign_t &assign);

  bool isValConsistentWithAssignment(std::string value, const synonym_t &syn,
                                     assign_t &assign);

  bool hasSelectCombinationTested(assign_t &assign,
                                  std::shared_ptr<QueryResult> result);

  bool backtrackingSearch(assign_t &assign, std::shared_ptr<QueryResult> result,
                          bool &hasSol, bool isBoolean);

  bool containSelectedSynonym(assign_t &assign);

  bool infer(inference_t &inference, assign_t &assign, const synonym_t &var,
             std::vector<std::shared_ptr<QPSConstraint>> &newlySatisfied);

  synonym_t pickUnassignedVariable(assign_t &assign);

  void revertConstraints(std::vector<std::shared_ptr<QPSConstraint>> &toRevert);

  void addNewlySatisfiedConstraint(
      std::shared_ptr<QPSConstraint> c,
      std::vector<std::shared_ptr<QPSConstraint>> &toAdd);

  bool allConstraintsSatisfied(synonym_t var);
};

#endif // SPA_QUERYEVALUATOR_H
