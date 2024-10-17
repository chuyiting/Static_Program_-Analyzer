//
// Created by Eddy Chu on 2022/10/2.
//

#ifndef SPA_ATTRCOMPARE_H
#define SPA_ATTRCOMPARE_H

#include "QPS/evaluator/QPSConstraint.h"
#include "QPS/evaluator/QueryEvaluator.h"
#include "WithRef.h"

class AttrCompare : public QPSConstraint {
  std::shared_ptr<WithRef> lhs;
  std::shared_ptr<WithRef> rhs;

public:
  /**
   * Constructor for AttrCompare, if lhs and rhs are not the same attribute type
   * (NAME, INTEGER), QPSException thrown
   * @param lhs
   * @param rhs
   */
  AttrCompare(std::shared_ptr<WithRef> lhs, std::shared_ptr<WithRef> rhs);

  bool isSatisfied(std::shared_ptr<PKBGetter> pkb, assign_t &assign) override;

  bool queryDomain(std::shared_ptr<PKBGetter> pkb, assign_t &assign,
                   orderDomain_t &orderDomain,
                   orderDomain_t &queryResult) override;

  std::vector<synonym_t> getVariables() override;

  int priority() override;

  bool operator==(const AttrCompare &other) const;

  bool operator!=(const AttrCompare &other) const;

private:
  std::string getWithRefValue(std::shared_ptr<PKBGetter> pkb,
                              std::shared_ptr<WithRef> ref, assign_t &assign);
};

#endif // SPA_ATTRCOMPARE_H
