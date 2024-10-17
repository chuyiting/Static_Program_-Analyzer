#include "NextResolver.h"

NextResolver::NextResolver(NextStorage &next_storage)
    : StatementResolver(), next_storage(next_storage) {}

bool NextResolver::isDirect() {
  return next_storage.isNextDirect(_lhs_value, _rhs_value);
}

bool NextResolver::isAnyRight() { return next_storage.isNextRight(_lhs_value); }

bool NextResolver::isAnyLeft() { return next_storage.isNextLeft(_rhs_value); }

bool NextResolver::isAny() { return next_storage.isNextExists(); }
