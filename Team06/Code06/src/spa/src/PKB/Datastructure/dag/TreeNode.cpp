//
// Created by Stephen Tan  Hin Khai on 10/10/22.
//

#include "TreeNode.h"

TreeNode::TreeNode(int id) : RootedNode(id), searchedAllRoots(false), allSetRoot(false) {}

TreeNode::TreeNode() = default;

void TreeNode::addParent(std::shared_ptr<TreeNode> &parent) {
  int id = parent->getId();
  parentNodes.insert({id, parent});
  indirect_parents.insert(id);
}

void TreeNode::addRoot(std::shared_ptr<TreeNode> &root) {
  int id = root->getId();
  rootNodes.insert({id, root});
  indirect_parents.insert(id);
}

bool TreeNode::isRootNode() { return rootNodes.empty() && parentNodes.empty(); }

std::unordered_set<int> TreeNode::findAllIndirectParent() {
  if (isRootNode()) {
    searchedAllRoots = true;
    return {};
  } else if (this->searchedAllRoots) {
    return this->indirect_parents;
  } else {
    for (const auto &p : parentNodes) {
      std::unordered_set<int> ancestors = p.second->findAllIndirectParent();
      indirect_parents.insert(ancestors.begin(), ancestors.end());
    }
    searchedAllRoots = true;
    return indirect_parents;
  }
}

std::unordered_set<int> TreeNode::getParents() {
  if (!isRootNode() || _id == 0) {
    return map_utils::keys(parentNodes);
    ;
  } else {
    return {this->_id};
  }
}

std::unordered_set<int> TreeNode::getRoot() const {
  return map_utils::keys(rootNodes);
}

// necessary to reroot as Calls/CallsT nodes cannot be deterministically added
void TreeNode::reroot() { validateRoot(); }

// Get Root
std::unordered_set<std::shared_ptr<TreeNode>> TreeNode::setRootFromParent() {
  if (isRootNode()) {
    return {std::make_shared<TreeNode>(*this)};
  } else if (allSetRoot) {
    return map_utils::values(rootNodes);
  } else {
    for (auto ptr : parentNodes) {
      auto parRoot = ptr.second->setRootFromParent();
      std::for_each(
          parRoot.begin(), parRoot.end(),
          [this](std::shared_ptr<TreeNode> ptr) { this->addRoot(ptr); });
    }
    return map_utils::values(rootNodes);
  }
}

// Remove node if not actually a root
void TreeNode::validateRoot() {
  setRootFromParent();
  std::vector<int> removeKey;
  for (const auto &i : rootNodes) {
    if (!i.second->isRootNode()) {
      removeKey.emplace_back(i.first);
    }
  }

  for (int key : removeKey) {
    rootNodes.erase(key);
  }
}
std::unordered_set<int> TreeNode::searchForIndirectParent(int id) {
  if (isRootNode()) {
    searchedAllRoots = true;
    return {};
  } else if (this->searchedAllRoots || getId() == id ||
             map_utils::isIterableContains(indirect_parents, id)) {
    return this->indirect_parents;
  } else {
    for (const auto &p : parentNodes) {
      std::unordered_set<int> ancestors = p.second->searchForIndirectParent(id);
      indirect_parents.insert(ancestors.begin(), ancestors.end());
      if (map_utils::isIterableContains(indirect_parents, id)) {
        return indirect_parents;
      }
    }
    searchedAllRoots = true;
    return indirect_parents;
  }
}
std::unordered_set<int> TreeNode::getIndirectParentCache() {
  return RootedNode::getIndirectParentCache();
}
