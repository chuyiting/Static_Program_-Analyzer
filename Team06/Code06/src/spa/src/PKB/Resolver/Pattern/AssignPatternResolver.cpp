#include "AssignPatternResolver.h"
#include "utils/ExpressionHandler/ExpressionHandler.h"

AssignPatternResolver::AssignPatternResolver(PatternStorage &pattern_storage)
    : Resolver(), pattern_storage(pattern_storage) {}

bool AssignPatternResolver::run(const Entity &lhs, const Entity &rhs,
                                const Statement &s) {
  _statement = s;
  resolve(lhs, rhs);
  assignStrategy();
  return (this->*fmap[_resolution_strategy])();
}

void AssignPatternResolver::assignStrategy() {
  _resolution_strategy = ResolutionStrategy::UNINITIALISED;
  _lhs_value = string_utils::trimWhitespace(_lhs_value);
  _rhs_value = string_utils::trimWhitespace(_rhs_value);
  // pattern('a', 'b')
  if (_lhs_type == ArgumentType::CONCRETE_ENTITY_REF &&
      (_rhs_type == ArgumentType::CONCRETE_ENTITY_REF ||
       _rhs_type == ArgumentType::DIRECTEXPR ||
       _rhs_type == ArgumentType::CONCRETE_NUMERIC_REF)) {
    _resolution_strategy = ResolutionStrategy::DIRECT;
    ExpressionHandler e =
        ExpressionHandler(ExpressionType::EXPRESSION, _rhs_value);
    e.parse();
    _rhsArr = e.getInfixNotation();
    // pattern('a', '_')
  } else if (_lhs_type == ArgumentType::CONCRETE_ENTITY_REF &&
             _rhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::DIRECT_ANY;
    // pattern(a, '_a + b_')
  } else if (_lhs_type == ArgumentType::CONCRETE_ENTITY_REF &&
             _rhs_type == ArgumentType::SUBEXPR) {
    _resolution_strategy = ResolutionStrategy::DIRECT_SUBTREE;
    _rhs_value =
        string_utils::trim(string_utils::trim(_rhs_value), string_utils::ws);
    ExpressionHandler e =
        ExpressionHandler(ExpressionType::EXPRESSION, _rhs_value);
    e.parse();
    _rhsArr = e.getInfixNotation();
    // pattern('_', 'a')
  } else if (_rhs_type == ArgumentType::CONCRETE_ENTITY_REF ||
             _rhs_type == ArgumentType::DIRECTEXPR ||
             _rhs_type == ArgumentType::CONCRETE_NUMERIC_REF) {
    _resolution_strategy = ResolutionStrategy::ANY_DIRECT;
    ExpressionHandler e =
        ExpressionHandler(ExpressionType::EXPRESSION, _rhs_value);
    e.parse();
    _rhsArr = e.getInfixNotation();
    // pattern( '_', '_' )
  } else if (_rhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_ANY;
    // pattern( _ , '_a + b_')
  } else if (_rhs_type == ArgumentType::SUBEXPR) {
    _resolution_strategy = ResolutionStrategy::ANY_SUBTREE;
    _rhs_value =
        string_utils::trim(string_utils::trim(_rhs_value), string_utils::ws);
    ExpressionHandler e =
        ExpressionHandler(ExpressionType::EXPRESSION, _rhs_value);
    e.parse();
    _rhsArr = e.getInfixNotation();
  }
}

bool AssignPatternResolver::isDirect() {
  return pattern_storage.isDirect(_lhs_value, _rhsArr, _statement);
}

bool AssignPatternResolver::isDirectAny() {
  return pattern_storage.isDirectAny(_lhs_value, _statement);
}

bool AssignPatternResolver::isDirectSubtree() {
  return pattern_storage.isDirectSubtree(_lhs_value, _rhsArr, _statement);
}

bool AssignPatternResolver::isAnyDirect() {
  return pattern_storage.isAnyDirect(_rhsArr, _statement);
}

bool AssignPatternResolver::isAnyAny() {
  return pattern_storage.isAnyAny(_statement);
}

bool AssignPatternResolver::isAnySubtree() {
  return pattern_storage.isAnySubtree(_rhsArr, _statement);
}
