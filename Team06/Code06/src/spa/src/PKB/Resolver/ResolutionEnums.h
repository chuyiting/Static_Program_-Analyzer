//
// Created by Stephen Tan  Hin Khai on 14/9/22.
//
#ifndef SPA_SRC_SPA_SRC_PKB_TYPE_REGEXRULES_H_
#define SPA_SRC_SPA_SRC_PKB_TYPE_REGEXRULES_H_

/**
 * Defines Regex rules to resolve checking for concrete or wildcard cases
 */
#include "PKB/type/pkb_types.h"
#include "utils/func_utils.h"
#include <functional>
#include <regex>
#include <unordered_map>
#include <utility>

using namespace pkb_types;

// identify the different arguments used.
enum class ArgumentType {
  CONCRETE_NUMERIC_REF,
  CONCRETE_ENTITY_REF,
  WILDCARD,
  DIRECTEXPR,
  SUBEXPR,
  ATTRIBUTE_VALUE,
  UNINITIALISED // should not happen in basic spa
};

// identify the different strategies employed
enum class ResolutionStrategy {
  DIRECT,
  DIRECT_ANY,
  DIRECT_IF,
  DIRECT_WHILE,
  DIRECT_SUBTREE,
  ANY_DIRECT,
  ANY_ANY,
  ANY_IF,
  ANY_WHILE,
  ANY_SUBTREE,
  ANY_BACKWARDS,
  ANY_FORWARDS,
  ANY_BIDIRECT,
  DIRECT_VAR,
  DIRECT_C,
  FORWARDS,
  SUBTREE,
  UNINITIALISED,
};

#endif // SPA_SRC_SPA_SRC_PKB_TYPE_REGEXRULES_H_
