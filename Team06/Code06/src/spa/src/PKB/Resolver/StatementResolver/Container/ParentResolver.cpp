#include "ParentResolver.h"

ParentResolver::ParentResolver(ParentStorage &parent_storage)
    : StatementResolver(), parent_storage(parent_storage) {}

bool ParentResolver::isDirect() {
  return parent_storage.isParentDirect(_lhs_value, _rhs_value);
}

// have a child of parent relation <Child, Parent>
bool ParentResolver::isAnyRight() {
  return parent_storage.isParentRight(_lhs_value);
}

bool ParentResolver::isAnyLeft() {
  return parent_storage.isParentLeft(_rhs_value);
}

bool ParentResolver::isAny() { return parent_storage.isParentExists(); }
