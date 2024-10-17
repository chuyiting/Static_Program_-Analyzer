//
// Created by Stephen Tan  Hin Khai on 15/9/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_RESOLVER_H_

#include "ResolutionEnums.h"
#include "utils/string_utils.h"
#include <regex>
#include <vector>

/**
 * Resolver generalises different variations of function calls and is extended
 * upon to support the different calls eg: follows('1', '2'), follows(_, '2'),
 * follows(1, _), (follows(_, _) Is a interface.
 */
class Resolver {

private:
  static inline std::vector<std::pair<std::regex, ArgumentType>> _regexRules = {
      {std::regex("0|([1-9][0-9]*)"), ArgumentType::CONCRETE_NUMERIC_REF},
      {std::regex("^\\s*[A-Za-z][A-Za-z0-9\\s]*"),
       ArgumentType::CONCRETE_ENTITY_REF},
      {std::regex("^\\s*[A-Za-z0-9*+\\-/%()\\s]*"), ArgumentType::DIRECTEXPR},
      {std::regex("^\\s*_\\s*"), ArgumentType::WILDCARD},
      {std::regex("^\\s*_[A-Za-z0-9*+\\-/%()\\s]*_\\s*"),
       ArgumentType::SUBEXPR}};

protected:
  Entity _lhs_value;
  Entity _rhs_value;
  ArgumentType _lhs_type;
  ArgumentType _rhs_type;
  ResolutionStrategy _resolution_strategy;

  // functions
  void resolve(const Entity &lhs, const Entity &rhs);

  virtual void assignStrategy() = 0;

public:
  virtual bool run(const Entity &lhs, const Entity &rhs,
                   const Statement &s) = 0;
  Resolver();
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_RESOLVER_H_
