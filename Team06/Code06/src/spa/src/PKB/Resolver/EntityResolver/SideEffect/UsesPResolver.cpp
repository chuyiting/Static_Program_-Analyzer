//
// Created by Stephen Tan  Hin Khai on 7/10/22.
//

#include "UsesPResolver.h"

UsesPResolver::UsesPResolver(UsesStorage &uses_storage)
    : EntityResolver(), uses_storage(uses_storage) {}

bool UsesPResolver::isDirectVariable() {
  return uses_storage.isUsesP(_lhs_value, _rhs_value);
}

bool UsesPResolver::isForward() {
  return uses_storage.isUsesPRight(_lhs_value);
}
