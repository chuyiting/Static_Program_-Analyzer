#include "NextTResolver.h"

NextTResolver::NextTResolver(NextStorage &next_storage)
    : StatementResolver(), next_storage(next_storage) {}

bool NextTResolver::isDirect() {
  return next_storage.isNextTDirect(_lhs_value, _rhs_value);
}

bool NextTResolver::isAnyRight() {
  return next_storage.isNextRight(_lhs_value);
}

bool NextTResolver::isAnyLeft() { return next_storage.isNextLeft(_rhs_value); }

bool NextTResolver::isAny() { return next_storage.isNextExists(); }
