#include "CallsResolver.h"

CallsResolver::CallsResolver(CallStorage &call_storage)
    : Resolver(), call_storage(call_storage) {}

bool CallsResolver::run(const Entity &lhs, const Entity &rhs,
                        const Statement &s) {
  resolve(lhs, rhs);
  assignStrategy();
  return (this->*fmap[_resolution_strategy])();
}

void CallsResolver::assignStrategy() {
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

bool CallsResolver::isDirect() {
  return call_storage.isCallsDirect(_lhs_value, _rhs_value);
}

bool CallsResolver::isAnyRight() {
  return call_storage.isCallsRight(_lhs_value);
}

bool CallsResolver::isAnyLeft() { return call_storage.isCallsLeft(_rhs_value); }

bool CallsResolver::isAny() { return call_storage.isCallsExists(); }
