#pragma once
#include "QPSConstraint.h"
#include "CSPUtils.h"

bool QPSConstraint::queryDomain(std::shared_ptr<PKBGetter> pkb,
                                assign_t &assign, orderDomain_t &orderDomain,
                                orderDomain_t &queryResult) {
  return false;
}

bool QPSConstraint::hasExactOneVarUnassigned(assign_t &assign) {
  auto vars = getVariables();
  if (vars.size() == 0) {
    return false;
  } else if (vars.size() == 1) {
    return assign[vars[0]] == CSPUtils::UNASSIGNED;
  }

  if (assign[vars[0]] == CSPUtils::UNASSIGNED &&
      assign[vars[1]] == CSPUtils::UNASSIGNED) {
    return false;
  }

  if (assign[vars[0]] != CSPUtils::UNASSIGNED &&
      assign[vars[1]] != CSPUtils::UNASSIGNED) {
    return false;
  }

  return true;
}
int QPSConstraint::priority() { return 0; }
