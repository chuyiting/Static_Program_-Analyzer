//
// Created by Stephen Tan  Hin Khai on 2/11/22.
//

#include "EntityResolver.h"

EntityResolver::EntityResolver() : Resolver(){};

bool EntityResolver::run(const Entity &lhs, const Entity &rhs,
                         const Statement &s) {
  resolve(lhs, rhs);
  assignStrategy();
  return (this->*fmap[_resolution_strategy])();
}

void EntityResolver::assignStrategy() {
  _resolution_strategy = ResolutionStrategy::UNINITIALISED;
  if (_rhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::FORWARDS;
  } else {
    _resolution_strategy = ResolutionStrategy::DIRECT_VAR;
  }
}
