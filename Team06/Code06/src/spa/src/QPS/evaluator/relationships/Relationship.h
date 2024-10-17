#pragma once

#include "PKB/PKBGetter.h"
#include "QPS/evaluator/QPSConstraint.h"
#include "QPS/evaluator/QueryEvaluator.h"
#include <memory>
#include <vector>

class QPSQueryEvaluatorAPI;

class Relationship {

public:
  virtual bool isSatisfied(std::shared_ptr<PKBGetter> pkb,
                           assign_t &assign) = 0;

  virtual bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                           orderDomain_t &orderDomain,
                           orderDomain_t &queryResult);

  virtual std::vector<synonym_t> getSynonyms() = 0;

  virtual int priority();

  virtual bool operator==(const Relationship &) = 0;

  virtual bool operator!=(const Relationship &) = 0;

protected:
  virtual bool queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                               domain_t &result);

  virtual bool queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                                domain_t &result);

  virtual bool isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb,
                                   std::string lhs, std::string rhs) = 0;

  bool isSatisfiedFromRefVal(std::shared_ptr<PKBGetter> pkb,
                                     std::string lhs, std::string rhs);
};
