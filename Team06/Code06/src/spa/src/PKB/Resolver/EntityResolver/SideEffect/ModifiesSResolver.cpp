#include "ModifiesSResolver.h"

ModifiesSResolver::ModifiesSResolver(ModifiesStorage &modifies_storage)
    : EntityResolver(), modifies_storage(modifies_storage) {}

bool ModifiesSResolver::isDirectVariable() {
  return modifies_storage.isModifiesS(_lhs_value, _rhs_value);
}

bool ModifiesSResolver::isForward() {
  return modifies_storage.isModifiesSRight(_lhs_value);
}
