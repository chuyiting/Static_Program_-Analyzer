#include "ParentTResolver.h"

ParentTResolver::ParentTResolver(ParentStorage &parent_storage)
    : StatementResolver(), parent_storage(parent_storage) {}

bool ParentTResolver::isDirect() {
  return parent_storage.isParentTDirect(_lhs_value, _rhs_value);
}

bool ParentTResolver::isAnyLeft() {
  return parent_storage.isParentTLeft(_rhs_value);
}

bool ParentTResolver::isAnyRight() {
  return parent_storage.isParentTRight(_lhs_value);
}

bool ParentTResolver::isAny() { return parent_storage.isParentExists(); }
