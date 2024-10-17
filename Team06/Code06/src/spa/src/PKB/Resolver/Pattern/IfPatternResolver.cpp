//
// Created by Chong Jun Wei on 14/10/22.
//

#include "IfPatternResolver.h"

IfPatternResolver::IfPatternResolver(PatternStorage &pattern_storage)
    : Resolver(), pattern_storage(pattern_storage) {}

bool IfPatternResolver::run(const Entity &lhs, const Entity &rhs,
                            const Statement &s) {
  _statement = s;
  resolve(lhs, rhs);
  assignStrategy();
  return (this->*fmap[_resolution_strategy])();
}

void IfPatternResolver::assignStrategy() {
  _resolution_strategy = ResolutionStrategy::UNINITIALISED;
  _lhs_value = string_utils::trimWhitespace(_lhs_value);
  // pattern('a', '_')
  if (_lhs_type == ArgumentType::CONCRETE_ENTITY_REF) {
    _resolution_strategy = ResolutionStrategy::DIRECT_IF;
    // pattern('_', '_')
  } else if (_lhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_IF;
  }
}

bool IfPatternResolver::isDirectIf() {
  return pattern_storage.isDirectIf(_lhs_value, _statement);
}

bool IfPatternResolver::isAnyIf() {
  return pattern_storage.isAnyIf(_statement);
}
