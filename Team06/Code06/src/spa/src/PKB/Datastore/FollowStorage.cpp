#include "FollowStorage.h"

FollowStorage::FollowStorage() : followsStorage(DAG<RootedNode>()) {}

void FollowStorage::addFollowT(const std::vector<Statement> &s) {
  for (int i = 1; i < s.size(); ++i) {
    this->followsStorage.addAncestor(std::stoi(s[i - 1]), std::stoi(s[i]));
  }
}

bool FollowStorage::isFollowDirect(const Entity &lhs, const Entity &rhs) {
  int lhsNum = std::stoi(lhs);
  int rhsNum = std::stoi(rhs);
  return lhsNum < rhsNum &&
         followsStorage.isDirectAncestor(std::stoi(rhs), std::stoi(lhs));
}

bool FollowStorage::isFollowRight(const Entity &parent) {
  return followsStorage.hasChild(std::stoi(parent));
}

bool FollowStorage::isFollowLeft(const Entity &child) {
  return followsStorage.hasRootAncestor(std::stoi(child));
}

bool FollowStorage::isFollowExists() { return followsStorage.hasAnyChild(); }

bool FollowStorage::isFollowTDirect(const Entity &lhs, const Entity &rhs) {
  int lhsNum = std::stoi(lhs);
  int rhsNum = std::stoi(rhs);
  return lhsNum < rhsNum && followsStorage.hasCommonAncestor(rhsNum, lhsNum);
}

bool FollowStorage::isFollowTRight(const Entity &parent) {
  return followsStorage.hasChild(std::stoi(parent));
}

bool FollowStorage::isFollowTLeft(const Entity &child) {
  return followsStorage.hasRootAncestor(std::stoi(child));
}

std::unordered_set<Statement> FollowStorage::getFollows(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : followsStorage.getDirectChildren(std::stoi(s))) {
    set.insert(std::to_string(i));
  }
  return set;
}

std::unordered_set<Statement> FollowStorage::getFollowT(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : followsStorage.getAllChildren(std::stoi(s))) {
    set.insert(std::to_string(i));
  }
  return set;
}

std::unordered_set<Statement> FollowStorage::getFollowedBy(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : followsStorage.getDirectAncestor(std::stoi(s))) {
    auto x = std::to_string(i);
    if (x == s) {
      continue;
    }
    set.insert(x);
  }
  return set;
}

std::unordered_set<Statement>
FollowStorage::getFollowedByT(const Statement &s) {
  std::unordered_set<Statement> set;
  for (int i : followsStorage.getAllAncestors(std::stoi(s))) {
    auto x = std::to_string(i);
    if (x == s) {
      continue;
    }
    set.insert(x);
  }
  return set;
}

std::unordered_map<Statement, std::unordered_set<Statement>>
FollowStorage::getAllFollowsT() {
  std::unordered_map<Statement, std::unordered_set<Statement>> map;
  for (const auto &iter : followsStorage.getAllNodeToLeafChildren()) {
    std::unordered_set<Statement> s;
    for (auto sno : iter.second) {
      s.insert(std::to_string(sno));
    }
    map[std::to_string(iter.first)] = s;
  }
  return map;
}

std::unordered_map<Statement, Statement> FollowStorage::getAllFollows() {
  std::unordered_map<Statement, Statement> map;
  for (auto iter : followsStorage.getAllNodeToChildren()) {
    // We know it is 1-1
    if (iter.second.begin() != iter.second.end()) {
      Statement s = std::to_string(*iter.second.begin());
      map.insert({std::to_string(iter.first), s});
    }
  }
  return map;
}

void FollowStorage::addNode(int num) { followsStorage.addNode(num); }
