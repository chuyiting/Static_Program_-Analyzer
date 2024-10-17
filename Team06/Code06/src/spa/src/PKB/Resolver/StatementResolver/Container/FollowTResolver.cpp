#include "FollowTResolver.h"

FollowTResolver::FollowTResolver(FollowStorage &follow_storage)
    : StatementResolver(), follow_storage(follow_storage) {}

bool FollowTResolver::isDirect() {
  return follow_storage.isFollowTDirect(_lhs_value, _rhs_value);
}

bool FollowTResolver::isAnyRight() {
  return follow_storage.isFollowTRight(_lhs_value);
}

bool FollowTResolver::isAnyLeft() {
  return follow_storage.isFollowTLeft(_rhs_value);
}

bool FollowTResolver::isAny() { return follow_storage.isFollowExists(); }
