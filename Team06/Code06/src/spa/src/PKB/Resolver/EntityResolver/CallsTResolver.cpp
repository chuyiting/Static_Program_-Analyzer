#include "CallsTResolver.h"

CallsTResolver::CallsTResolver(CallStorage &call_storage)
    : Resolver(), call_storage(call_storage) {}

bool CallsTResolver::run(const Entity &lhs, const Entity &rhs,
                         const Statement &s) {
  resolve(lhs, rhs);
  assignStrategy();
  return (this->*fmap[_resolution_strategy])();
}

void CallsTResolver::assignStrategy() {
  _resolution_strategy = ResolutionStrategy::UNINITIALISED;
  if (_lhs_type == ArgumentType::WILDCARD &&
      _rhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_BIDIRECT; // (_,_)
  } else if (_lhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_BACKWARDS; // (_,literal)
  } else if (_rhs_type == ArgumentType::WILDCARD) {
    _resolution_strategy = ResolutionStrategy::ANY_FORWARDS; // (literal,_)
  } else {
    _resolution_strategy = ResolutionStrategy::DIRECT; // (literal,literal)
  }
}

bool CallsTResolver::isDirect() {
  return call_storage.isCallsTDirect(_lhs_value, _rhs_value);
}

bool CallsTResolver::isAnyRight() {
  return call_storage.isCallsTRight(_lhs_value);
}

bool CallsTResolver::isAnyLeft() {
  return call_storage.isCallsTLeft(_rhs_value);
}

bool CallsTResolver::isAny() { return call_storage.isCallsExists(); }
