//
// Created by Stephen Tan  Hin Khai on 30/9/22.
//
#include "RootedNode.h"
#include "utils/map_utils.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORAGE_UNIONFIND_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORAGE_UNIONFIND_H_

// Parent in this class refers to the union find NOT the Design Abstraction
// This union find acts as a contextless datastructure with generic calls
// It is used as a storage class for the relations: Parent/ParentT,
// Follow/FollowT, Call/CallT As this is a standard data structure, it is meant
// to be a blackbox. Representation is up to the relation
template <class T> class DAG {
private:
  int size; // will retroactively initialise the newly created nodes
  std::unordered_map<int, std::shared_ptr<T>> nodes;
  bool hasAChild;

  // root has not been reset
  // extend the number of nodes until they are of the given size
  [[nodiscard]] bool isin(int key) const {
    return map_utils::isIterableContains(nodes, key);
  }

  std::shared_ptr<T> &getNode(int index) {
    return isin(index) ? nodes[index] : nodes[0];
  }

  std::shared_ptr<T> getOrCreateNode(int i) {
    if (!isin(i)) {
      addNode(i);
    }
    return getNode(i);
  }

public:
  DAG() : hasAChild(false) {
    static_assert(
        std::is_base_of_v<RootedNode, T>,
        "Type Param of this class must derive from the RootedNode class");
    size = 1;
    // initialise a placeholder "root" to make 1 base indexing
    nodes[0] = std::make_shared<T>(T(0));
  }

  ~DAG() = default;

  void addNode(int newSize) {
    size = std::max(size, newSize);
    for (int i = nodes.size(); i < newSize + 1; ++i) {
      nodes.insert({i, std::make_shared<T>(T(i))});
    }
  }

  void addAncestors(int parent, const std::vector<int> &children) {
    getOrCreateNode(parent);
    for (auto c : children) {
      addAncestor(parent, c);
    }
  }

  // will always be correctly called
  void addAncestor(int parent, int child) {
    std::shared_ptr<T> parentN = getOrCreateNode(parent);
    std::shared_ptr<T> childN = getOrCreateNode(child);
    hasAChild = true;
    // if parent is a root node, set as recursive root else, get
    if (parentN->isRootNode()) {
      childN->addRoot(getNode(parent));
    } else {

      // Set child root as the root/s of parent
      for (auto iter : parentN->getRoot()) {
        std::shared_ptr<T> root = getNode(iter);
        childN->addRoot(root);
      }
    }

    childN->addParent(getNode(parent));
    parentN->addChildren(getNode(child));
  }

  // Find the direct parent/s of the child
  std::unordered_set<int> findAncestor(int child) {
    if (child == 0 || !isin(child)) {
      return {};
    }
    return getNode(child)->getParents();
  }

  // if has ancestors, it is not a root
  bool hasRootAncestor(int node) {
    if (node == 0 || !isin(node)) {
      return false;
    }
    return !getNode(node)->isRootNode();
  }

  // if has children, it is not a leaf
  bool hasChild(int node) {
    if (node == 0 || !isin(node)) {
      return false;
    }
    return !(getNode(node)->isLeafNode());
  }

  // one node and it's child node
  // Intended for Follows
  bool hasCommonAncestor(int child1, int child2) {
    if (child1 == child2 || (!isin(child1) || !isin(child2))) {
      return false;
    }
    // if either node is an root check for root of opposite
    if (!hasRootAncestor(child1)) {
      return map_utils::isIterableContains(nodes[child2]->getRoot(),
                                           nodes[child1]->getId());
    }

    if (!hasRootAncestor(child2)) {
      return map_utils::isIterableContains(nodes[child1]->getRoot(),
                                           nodes[child2]->getId());
    }

    for (auto i : nodes[child1]->getRoot()) {
      if (map_utils::isIterableContains(nodes[child2]->getRoot(), i)) {
        return true;
      }
    }
    return false;
  }

  bool isIndirectAncestor(int child, int parent) {
    if ((child == 0 || parent == 0) || (!isin(child) || !isin(parent))) {
      return false;
    }
    return nodes[child]->isIndirectParent(parent);
  }

  bool isIndirectChild(int parent, int child) {
    if ((child == 0 || parent == 0) || (!isin(child) || !isin(parent))) {
      return false;
    }
    return nodes[parent]->isIndirectChild(child);
  }

  bool isDirectAncestor(int child, int parent) {
    if (parent == child || (!isin(child) || !isin(parent))) {
      return false;
    }

    std::unordered_set<int> v = nodes[child]->getParents();
    return map_utils::isIterableContains(v, parent);
  }

  bool isDirectChild(int parent, int child) {
    if (parent == child || (!isin(child) || !isin(parent))) {
      return false;
    }
    std::unordered_set<int> v = nodes[parent]->getChildren();
    return map_utils::isIterableContains(v, child);
  }

  bool hasAnyChild() { return hasAChild; }

  // Get all ancestors down from root

  std::unordered_set<int> getDirectAncestor(int child) {
    return getNode(child)->getParents();
  }

  std::unordered_set<int> getDirectChildren(int parent) {
    return getNode(parent)->getChildren();
  }

  std::unordered_set<int> getParentCacheState(int child) {
    return getNode(child)->getIndirectParentCache();
  }

  std::unordered_set<int> getChildCacheState(int child) {
    return getNode(child)->getIndirectChildCache();
  }

  std::unordered_set<int> getAllAncestors(int child) {
    return getNode(child)->findAllIndirectParent();
  }

  std::unordered_set<int> getRoots(int child) {
    return getNode(child)->getRoot();
  }

  std::unordered_set<int> getAllChildren(int parent) {
    return getNode(parent)->findAllIndirectChildren();
  }

  std::unordered_map<int, std::unordered_set<int>>
  getAllNodeToDirectAncestor() {
    std::unordered_map<int, std::unordered_set<int>> res;
    for (auto iter : nodes) {
      res.insert({iter.first, getDirectAncestor(iter.first)});
    }
    return res;
  }

  std::unordered_map<int, std::unordered_set<int>> getAllNodeToAncestor() {
    std::unordered_map<int, std::unordered_set<int>> res;
    for (auto iter : nodes) {
      res.insert({iter.first, getAllAncestors(iter.first)});
    }
    return res;
  }

  std::unordered_map<int, std::unordered_set<int>> getAllNodeToChildren() {
    std::unordered_map<int, std::unordered_set<int>> res;
    for (auto iter : nodes) {
      res.insert({iter.first, getDirectChildren(iter.first)});
    }
    return res;
  }

  std::unordered_map<int, std::unordered_set<int>> getAllNodeToLeafChildren() {
    std::unordered_map<int, std::unordered_set<int>> res;
    for (const auto &iter : nodes) {
      res.insert({iter.first, getAllChildren(iter.first)});
    }
    return res;
  }

  void reroot() {
    for (auto &iter : nodes) {
      iter.second->reroot();
    }
  }
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORAGE_UNIONFIND_H_
