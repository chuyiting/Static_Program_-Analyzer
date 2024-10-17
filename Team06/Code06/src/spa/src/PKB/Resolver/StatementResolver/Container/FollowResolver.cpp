#include "FollowResolver.h"

FollowResolver::FollowResolver(FollowStorage &follow_storage)
    : StatementResolver(), follow_storage(follow_storage) {}

bool FollowResolver::isDirect() {
  return follow_storage.isFollowDirect(_lhs_value, _rhs_value);
}

bool FollowResolver::isAnyRight() {
  return follow_storage.isFollowRight(_lhs_value);
}

bool FollowResolver::isAnyLeft() {
  return follow_storage.isFollowLeft(_rhs_value);
}

bool FollowResolver::isAny() { return follow_storage.isFollowExists(); }
