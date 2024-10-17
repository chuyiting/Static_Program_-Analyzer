//
// Created by Stephen Tan  Hin Khai on 2/11/22.
//

#include "StatementResolver.h"

StatementResolver::StatementResolver() : Resolver(){};

bool StatementResolver::run(const Entity &lhs, const Entity &rhs,
                            const Statement &s) {
  resolve(lhs, rhs);
  assignStrategy();
  return (this->*fmap[_resolution_strategy])();
}

void StatementResolver::assignStrategy() {
  _resolution_strategy = ResolutionStrategy::UNINITIALISED;
  if (_lhs_type == ArgumentType::CONCRETE_NUMERIC_REF &&
      _rhs_type == ArgumentType::CONCRETE_NUMERIC_REF) {
    _resolution_strategy = ResolutionStrategy::DIRECT;
  } else if (_lhs_type == ArgumentType::CONCRETE_NUMERIC_REF &&
             _rhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_FORWARDS;
  } else if (_lhs_type == ArgumentType::WILDCARD &&
             _rhs_type == ArgumentType::CONCRETE_NUMERIC_REF) {
    _resolution_strategy = ResolutionStrategy::ANY_BACKWARDS;
  } else if (_lhs_type == ArgumentType::WILDCARD &&
             _rhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_BIDIRECT;
  }
}
