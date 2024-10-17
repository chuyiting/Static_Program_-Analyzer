//
// Created by Eddy Chu on 2022/9/10.
//

#ifndef SPA_QPSCONSTRAINT_H
#define SPA_QPSCONSTRAINT_H

#include "PKB/PKBGetter.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include <unordered_map>
#include <vector>
#include <string>

typedef std::unordered_set<std::string> domain_t;
typedef std::unordered_map<synonym_t, domain_t> orderDomain_t;
typedef std::unordered_map<synonym_t, std::string> assign_t;
typedef std::vector<assign_t> assigns_t;

class QPSConstraint {

protected:
  bool hasExactOneVarUnassigned(assign_t &assign);

public:
  virtual bool isSatisfied(std::shared_ptr<PKBGetter> pkb,
                           assign_t &assign) = 0;

  virtual std::vector<synonym_t> getVariables() = 0;

  /**
   *
   * @param pkb
   * @param assign
   * @param orderDomain
   * @param queryResult
   * @return
   */
  virtual bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                           orderDomain_t &orderDomain,
                           orderDomain_t &queryResult);

  virtual int priority();
};

#endif // SPA_QPSCONSTRAINT_H
