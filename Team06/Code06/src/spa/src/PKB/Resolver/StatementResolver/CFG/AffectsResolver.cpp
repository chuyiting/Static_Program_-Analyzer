#include "AffectsResolver.h"

AffectsResolver::AffectsResolver(AffectsStorage &affects_storage)
    : StatementResolver(), affects_storage(affects_storage) {}

bool AffectsResolver::isDirect() {
  return affects_storage.isAffectsDirect(_lhs_value, _rhs_value);
}

bool AffectsResolver::isAnyRight() {
  return affects_storage.isAffectsRight(_lhs_value);
}

bool AffectsResolver::isAnyLeft() {
  return affects_storage.isAffectsLeft(_rhs_value);
}

bool AffectsResolver::isAny() { return affects_storage.isAffectsExists(); }
