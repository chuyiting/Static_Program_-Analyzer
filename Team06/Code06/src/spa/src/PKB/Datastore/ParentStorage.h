#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_PARENTSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_PARENTSTORAGE_H_
#include "PKB/Datastructure/dag/DAG.h"
#include "PKB/Datastructure/dag/RootedNode.h"
#include "PKB/type/pkb_types.h"
#include <unordered_set>

using namespace pkb_types;
class ParentStorage {
private:
  DAG<RootedNode> parentStorage;

public:
  ParentStorage();

  void addNode(int num);
  void addParent(const Statement &s1, const std::vector<Statement> &s);

  bool isParentDirect(const Entity &lhs, const Entity &rhs);
  bool isParentRight(const Entity &lhs);
  bool isParentLeft(const Entity &lhs);
  bool isParentExists();
  bool isParentTDirect(const Entity &lhs, const Entity &rhs);
  bool isParentTRight(const Entity &lhs);
  bool isParentTLeft(const Entity &lhs);

  std::unordered_set<Statement> getParents(const Statement &s);
  std::unordered_set<Statement> getParentT(const Statement &s);
  std::unordered_set<Statement> getChildren(const Statement &s);
  std::unordered_set<Statement> getChildrenT(const Statement &s);
  std::unordered_map<Statement, std::unordered_set<Statement>> getAllParents();
  std::unordered_map<Statement, std::unordered_set<Statement>> getAllParentsT();
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_PARENTSTORAGE_H_
