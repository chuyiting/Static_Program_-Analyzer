#include "ParentStorage.h"

ParentStorage::ParentStorage() : parentStorage(DAG<RootedNode>()) {}

void ParentStorage::addParent(const Statement &parent,
                              const std::vector<Statement> &children) {
  std::vector<int> intSet;
  for (const auto &stmt : children) {
    intSet.emplace_back(std::stoi(stmt));
  }
  parentStorage.addAncestors(std::stoi(parent), intSet);
}

bool ParentStorage::isParentDirect(const Entity &parent, const Entity &child) {
  int lhsNum = std::stoi(parent);
  int rhsNum = std::stoi(child);
  return lhsNum < rhsNum && parentStorage.isDirectAncestor(rhsNum, lhsNum);
}

bool ParentStorage::isParentLeft(const Entity &parent) {
  return parentStorage.hasRootAncestor(std::stoi(parent));
}

bool ParentStorage::isParentRight(const Entity &child) {
  return parentStorage.hasChild(std::stoi(child));
}

bool ParentStorage::isParentExists() { return parentStorage.hasAnyChild(); }

bool ParentStorage::isParentTDirect(const Entity &parent, const Entity &child) {
  int lhsNum = std::stoi(parent);
  int rhsNum = std::stoi(child);
  return lhsNum < rhsNum && parentStorage.isIndirectAncestor(rhsNum, lhsNum);
}

// does this parent(lhs) have a child(rhs)
bool ParentStorage::isParentTLeft(const Entity &parent) {
  return parentStorage.hasRootAncestor(std::stoi(parent));
}

// does this child(rhs) have a parent(lhs)
bool ParentStorage::isParentTRight(const Entity &child) {
  return parentStorage.hasChild(std::stoi(child));
}

std::unordered_set<Statement> ParentStorage::getParents(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : parentStorage.getDirectChildren(std::stoi(s))) {
    set.insert(std::to_string(i));
  }
  return set;
}

std::unordered_set<Statement> ParentStorage::getParentT(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : parentStorage.getAllChildren(std::stoi(s))) {
    set.insert(std::to_string(i));
  }
  return set;
}

std::unordered_set<Statement> ParentStorage::getChildren(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : parentStorage.getDirectAncestor(std::stoi(s))) {
    auto x = std::to_string(i);
    if (x == s) {
      continue;
    }
    set.insert(x);
  }
  return set;
}

std::unordered_set<Statement> ParentStorage::getChildrenT(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : parentStorage.getAllAncestors(std::stoi(s))) {
    auto x = std::to_string(i);
    if (x == s) {
      continue;
    }
    set.insert(x);
  }
  return set;
}

// Parent -> {Children}
std::unordered_map<Statement, std::unordered_set<Statement>>
ParentStorage::getAllParentsT() {
  std::unordered_map<Statement, std::unordered_set<Statement>> map;
  for (const auto &iter : parentStorage.getAllNodeToLeafChildren()) {
    std::unordered_set<Statement> s;
    for (auto sno : iter.second) {
      s.insert(std::to_string(sno));
    }
    map[std::to_string(iter.first)] = s;
  }
  return map;
}

// Parent -> {ChildrenT}
std::unordered_map<Statement, std::unordered_set<Statement>>
ParentStorage::getAllParents() {
  std::unordered_map<Statement, std::unordered_set<Statement>> map;
  for (const auto &iter : parentStorage.getAllNodeToChildren()) {
    std::unordered_set<Statement> s;
    for (auto sno : iter.second) {
      s.insert(std::to_string(sno));
    }
    map[std::to_string(iter.first)] = s;
  }
  return map;
}

void ParentStorage::addNode(int num) { parentStorage.addNode(num); }
