#include "AffectsTResolver.h"

AffectsTResolver::AffectsTResolver(AffectsStorage &affects_storage)
    : StatementResolver(), affects_storage(affects_storage) {}

bool AffectsTResolver::isDirect() {
  return affects_storage.isAffectsTDirect(_lhs_value, _rhs_value);
}

bool AffectsTResolver::isAnyRight() {
    return affects_storage.isAffectsRight(_lhs_value);
}

bool AffectsTResolver::isAnyLeft() {
    return affects_storage.isAffectsLeft(_rhs_value);
}

bool AffectsTResolver::isAny() {
    return affects_storage.isAffectsExists();
}

