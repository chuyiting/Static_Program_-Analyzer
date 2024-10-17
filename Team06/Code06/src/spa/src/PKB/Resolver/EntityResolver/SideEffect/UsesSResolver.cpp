#include "UsesSResolver.h"

UsesSResolver::UsesSResolver(UsesStorage &uses_storage)
    : EntityResolver(), uses_storage(uses_storage) {}

bool UsesSResolver::isDirectVariable() {
  return uses_storage.isUsesS(_lhs_value, _rhs_value);
}

bool UsesSResolver::isForward() {
  return uses_storage.isUsesSRight(_lhs_value);
}
