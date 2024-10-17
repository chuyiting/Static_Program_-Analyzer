//
// Created by Stephen Tan  Hin Khai on 7/10/22.
//

#include "ModifiesPResolver.h"
#include "PKB/Resolver/Resolver.h"

ModifiesPResolver::ModifiesPResolver(ModifiesStorage &modifies_storage)
    : EntityResolver(), modifies_storage(modifies_storage) {}

bool ModifiesPResolver::isDirectVariable() {
  return modifies_storage.isModifiesP(_lhs_value, _rhs_value);
}

bool ModifiesPResolver::isForward() {
  return modifies_storage.isModifiesPRight(_lhs_value);
}
