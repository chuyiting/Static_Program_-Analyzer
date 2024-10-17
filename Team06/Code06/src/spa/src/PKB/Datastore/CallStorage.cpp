#include "CallStorage.h"

CallStorage::CallStorage()
    : callsStorage(DAG<TreeNode>()), identMapping(), proc(1),
      isComplete(false) {}

int CallStorage::generateIdentityMap(const Procedure &p) {
  if (!hasProcedure(p)) {
    identMapping.insert({p, proc});
    this->callsStorage.addNode(proc);
    proc++;
  }
  return mapToId(p);
}

bool CallStorage::hasProcedure(const Procedure &p) {
  return identMapping.has_key(p);
}

Procedure CallStorage::mapToProc(CallStorage::ident id) {
  return identMapping.get_key(id);
}

CallStorage::ident CallStorage::mapToId(const Procedure &procedure) {
  return identMapping.get_value(procedure);
}

void CallStorage::addProcedure(const Procedure &p) { generateIdentityMap(p); }

void CallStorage::addCall(const Procedure &caller, const Procedure &callee) {
  ident parent, child;
  // if not exists
  if (!identMapping.has_key(caller)) {
    parent = generateIdentityMap(caller);
  } else {
    parent = mapToId(caller);
  }

  if (!identMapping.has_key(callee)) {
    child = generateIdentityMap(callee);
  } else {
    child = mapToId(callee);
  }
  callsStorage.addAncestor(parent, child);
}

bool CallStorage::isCallsDirect(const Entity &lhs, const Entity &rhs) {
  // existance check
  executeReroot();
  if (!hasProcedure(lhs) || !hasProcedure(rhs)) {
    return false;
  }
  ident parent = mapToId(lhs);
  ident child = mapToId(rhs);

  return !(parent == child) && callsStorage.isDirectAncestor(child, parent);
}

bool CallStorage::isCallsRight(const Entity &parent) {
  executeReroot();
  if (!hasProcedure(parent)) {
    return false;
  }
  ident parentId = mapToId(parent);
  return callsStorage.hasChild(parentId);
}

bool CallStorage::isCallsLeft(const Entity &child) {
  executeReroot();
  if (!hasProcedure(child)) {
    return false;
  }
  ident childId = mapToId(child);
  return callsStorage.hasRootAncestor(childId);
}

bool CallStorage::isCallsExists() {
  executeReroot();
  return callsStorage.hasAnyChild();
}

bool CallStorage::isCallsTDirect(const Entity &lhs, const Entity &rhs) {
  executeReroot();
  if (!hasProcedure(lhs) || !hasProcedure(rhs)) {
    return false;
  }
  ident parent = mapToId(lhs);
  ident child = mapToId(rhs);

  return !(parent == child) && callsStorage.isIndirectAncestor(child, parent);
}

bool CallStorage::isCallsTRight(const Entity &parent) {
  executeReroot();
  if (!hasProcedure(parent)) {
    return false;
  }
  ident parentId = mapToId(parent);
  return callsStorage.hasChild(parentId);
}

bool CallStorage::isCallsTLeft(const Entity &child) {
  executeReroot();
  if (!hasProcedure(child)) {
    return false;
  }
  ident childId = mapToId(child);
  return callsStorage.hasRootAncestor(childId);
}

void CallStorage::executeReroot() {
  if (!isComplete) {
    callsStorage.reroot();
    isComplete = true;
  }
}

std::unordered_set<Procedure> CallStorage::getCalls(const Statement &s) {
  executeReroot();
  std::unordered_set<Procedure> set;

  if (!hasProcedure(s)) {
    return set;
  }

  for (int i : callsStorage.getDirectChildren(mapToId(s))) {
    set.insert(mapToProc(i));
  }
  return set;
}

std::unordered_set<Procedure> CallStorage::getCallsT(const Statement &s) {
  executeReroot();
  std::unordered_set<Procedure> set;

  if (!hasProcedure(s)) {
    return set;
  }

  for (int i : callsStorage.getAllChildren(mapToId(s))) {
    set.insert(mapToProc(i));
  }
  return set;
}

std::unordered_set<Procedure> CallStorage::getCalled(const Statement &s) {
  executeReroot();
  std::unordered_set<Procedure> set;

  if (!hasProcedure(s)) {
    return set;
  }

  for (int i : callsStorage.getDirectAncestor(mapToId(s))) {
    auto x = mapToProc(i);
    if (x == s) {
      continue;
    }
    set.insert(x);
  }

  return set;
}

std::unordered_set<Procedure> CallStorage::getCalledT(const Statement &s) {
  executeReroot();
  std::unordered_set<Procedure> set;

  if (!hasProcedure(s)) {
    return set;
  }

  for (int i : callsStorage.getAllAncestors(mapToId(s))) {
    auto x = mapToProc(i);
    if (x == s) {
      continue;
    }
    set.insert(x);
  }

  return set;
}
