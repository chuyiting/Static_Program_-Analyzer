//
// Created by Stephen Tan  Hin Khai on 10/10/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_DAG_TREENODE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_DAG_TREENODE_H_

#include "RootedNode.h"
#include <queue>
// For DAG Storage class with multiple parent,  multiple children, multiple
// roots ie Calls/CallsT Log n is acceptable as compared to memory overhead of
// unordered
class TreeNode : public RootedNode {

  std::map<int, std::shared_ptr<TreeNode>> rootNodes;
  std::map<int, std::shared_ptr<TreeNode>> parentNodes;
  bool searchedAllRoots;
  // If node is just initialised or being inserted, in context of the
  bool allSetRoot;
  void validateRoot();

public:
  TreeNode();
  explicit TreeNode(int id);
  // Doesnt work with Polymorphism afaik :/
  void addParent(std::shared_ptr<TreeNode> &parent);
  void addRoot(std::shared_ptr<TreeNode> &Root);

  [[nodiscard]] std::unordered_set<int> getParents() override;
  [[nodiscard]] std::unordered_set<int> getRoot() const override;

  bool isRootNode() override;

  std::unordered_set<int> findAllIndirectParent() override;
  std::unordered_set<int> searchForIndirectParent(int) override;

  std::unordered_set<int> getIndirectParentCache() override;

  void reroot() override;
  std::unordered_set<std::shared_ptr<TreeNode>> setRootFromParent();
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTRUCTURE_DAG_TREENODE_H_
