//
// Created by Stephen Tan  Hin Khai on 10/10/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_DAG_ROOTEDNODE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_DAG_ROOTEDNODE_H_

#include "utils/map_utils.h"
#include <map>
#include <memory>

// For DAG Storage class with single parent, multiple children
// Log n is acceptable as compared to memory overhead of unordered
class RootedNode {
  // multiple roots in subclass
  int root;
  // multiple parents in subclass
  std::shared_ptr<RootedNode> parentNode;
  bool searchedRoot;

protected:
  int _id;

  std::map<int, std::shared_ptr<RootedNode>> childNodes;

  // Only known at cache time.
  std::unordered_set<int> indirect_parents;
  std::unordered_set<int> indirect_children;

public:
  explicit RootedNode(int id);
  RootedNode();

  [[nodiscard]] int getId() const;

  // No function required in
  virtual void reroot();

  // collection methods for multi child, multi parent
  void addChildren(const std::shared_ptr<RootedNode> &);
  // @Reviewers should these be set as final?
  void addParent(std::shared_ptr<RootedNode> &parent);
  void addRoot(std::shared_ptr<RootedNode> &Root);

  virtual std::unordered_set<int> getParents();
  [[nodiscard]] virtual std::unordered_set<int> getRoot() const;
  std::unordered_set<int> getChildren();

  virtual bool isRootNode();
  bool isLeafNode();

  // id is a indirect parent
  bool isIndirectParent(int id);
  // id is a indirect child
  bool isIndirectChild(int id);
  virtual std::unordered_set<int> searchForIndirectParent(int);
  std::unordered_set<int> searchForIndirectChild(int);

  virtual std::unordered_set<int> findAllIndirectParent();
  std::unordered_set<int> findAllIndirectChildren();

  virtual std::unordered_set<int> getIndirectParentCache();
  std::unordered_set<int> getIndirectChildCache();
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_DAG_ROOTEDNODE_H_
