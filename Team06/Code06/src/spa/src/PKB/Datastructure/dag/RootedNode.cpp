//
// Created by Stephen Tan  Hin Khai on 10/10/22.
//

#include "RootedNode.h"

RootedNode::RootedNode() = default;

RootedNode::RootedNode(int id)
    : _id(id), parentNode(nullptr), root(-1), searchedRoot(false) {}

int RootedNode::getId() const { return _id; }

std::unordered_set<int> RootedNode::getChildren() {
  return map_utils::keys(childNodes);
}

void RootedNode::addChildren(const std::shared_ptr<RootedNode> &child) {
  int id = child->getId();

  childNodes.insert({id, child});
  indirect_children.insert(id);
}

void RootedNode::addParent(std::shared_ptr<RootedNode> &parent) {

  this->parentNode = parent;
  indirect_parents.insert(parent->getId());
}

void RootedNode::addRoot(std::shared_ptr<RootedNode> &rootNode) {
  this->root = rootNode->getId();
}

std::unordered_set<int> RootedNode::getRoot() const { return {root}; }

bool RootedNode::isRootNode() { return parentNode == nullptr; }

bool RootedNode::isLeafNode() { return childNodes.empty(); }

std::unordered_set<int> RootedNode::getParents() {
  if (_id == 0) {
    return {};
  } else if (!isRootNode()) {
    return {parentNode->getId()};
  } else {
    return {this->_id};
  }
}

std::unordered_set<int> RootedNode::findAllIndirectParent() {
  if (isRootNode()) {
    searchedRoot = true;
    return {};
  } else if (this->searchedRoot) {
    return this->indirect_parents;
  } else {
    std::unordered_set<int> ancestors = parentNode->findAllIndirectParent();
    indirect_parents.insert(ancestors.begin(), ancestors.end());
    searchedRoot = true;
    return indirect_parents;
  }
}

std::unordered_set<int> RootedNode::findAllIndirectChildren() {
  if (isLeafNode()) {
    return {};
  } else {
    for (const auto &iter : childNodes) {
      std::unordered_set<int> grandchildren =
          iter.second->findAllIndirectChildren();
      indirect_children.insert(grandchildren.begin(), grandchildren.end());
    }
    return indirect_children;
  }
}

bool RootedNode::isIndirectChild(int id) {
  if (map_utils::isIterableContains(indirect_children, id)) {
    return true;
  }
  if (id == getId()) {
    return true;
  }
  searchForIndirectChild(id);
  return map_utils::isIterableContains(indirect_children, id);
}

bool RootedNode::isIndirectParent(int id) {
  if (map_utils::isIterableContains(indirect_parents, id)) {
    return true;
  }
  if (id == getId()) {
    return true;
  }
  searchForIndirectParent(id);
  return map_utils::isIterableContains(indirect_parents, id);
}

std::unordered_set<int> RootedNode::searchForIndirectParent(int id) {
  if (isRootNode()) {
    // at true root we ensure it doesnt recurse up to placeholder root
    searchedRoot = true;
    return {getId()};
  }
  if (searchedRoot || getId() == id ||
      map_utils::isIterableContains(indirect_parents, id)) {
    return indirect_parents;
  }
  std::unordered_set<int> ancestors = parentNode->searchForIndirectParent(id);
  indirect_parents.insert(ancestors.begin(), ancestors.end());
  if (parentNode->searchedRoot) {
    searchedRoot = true;
  }
  return indirect_parents;
}

std::unordered_set<int> RootedNode::searchForIndirectChild(int id) {
  if (isLeafNode()) {
    // at true root we ensure it doesnt recurse up to placeholder root
    return {getId()};
  } else if (getId() == id ||
             map_utils::isIterableContains(indirect_children, id)) {
    return indirect_children;
  }
  for (const auto &children : childNodes) {
    std::unordered_set<int> grandChildren =
        children.second->searchForIndirectChild(id);
    indirect_children.insert(grandChildren.begin(), grandChildren.end());
    if (map_utils::isIterableContains(indirect_children, id)) {
      return indirect_children;
    }
  }
  return indirect_children;
}

std::unordered_set<int> RootedNode::getIndirectParentCache() {
  return indirect_parents;
}

std::unordered_set<int> RootedNode::getIndirectChildCache() {
  return indirect_children;
}

void RootedNode::reroot() {}
