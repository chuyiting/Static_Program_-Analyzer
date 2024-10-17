//
// Created by Chong Jun Wei on 14/10/22.
//

#include "WhilePatternResolver.h"

WhilePatternResolver::WhilePatternResolver(PatternStorage &pattern_storage)
    : Resolver(), pattern_storage(pattern_storage) {}

bool WhilePatternResolver::run(const Entity &lhs, const Entity &rhs,
                               const Statement &s) {
  _statement = s;
  resolve(lhs, rhs);
  assignStrategy();
  return (this->*fmap[_resolution_strategy])();
}

void WhilePatternResolver::assignStrategy() {
  _resolution_strategy = ResolutionStrategy::UNINITIALISED;
  _lhs_value = string_utils::trimWhitespace(_lhs_value);
  // pattern('a', '_')
  if (_lhs_type == ArgumentType::CONCRETE_ENTITY_REF) {
    _resolution_strategy = ResolutionStrategy::DIRECT_WHILE;
    // pattern('_', '_')
  } else if (_lhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_WHILE;
  }
}

bool WhilePatternResolver::isDirectWhile() {
  return pattern_storage.isDirectWhile(_lhs_value, _statement);
}

bool WhilePatternResolver::isAnyWhile() {
  return pattern_storage.isAnyWhile(_statement);
}
