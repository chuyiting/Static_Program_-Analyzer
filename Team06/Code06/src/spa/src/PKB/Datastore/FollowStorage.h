#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_FOLLOWSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_FOLLOWSTORAGE_H_
#include "PKB/Datastructure/dag/DAG.h"
#include "PKB/Datastructure/dag/RootedNode.h"
#include "PKB/type/pkb_types.h"
#include <unordered_set>

using namespace pkb_types;
class FollowStorage {
private:
  DAG<RootedNode> followsStorage;

public:
  FollowStorage();

  void addFollowT(const std::vector<Statement> &s);
  void addNode(int);

  bool isFollowDirect(const Entity &lhs, const Entity &rhs);
  bool isFollowRight(const Entity &lhs);
  bool isFollowLeft(const Entity &rhs);
  bool isFollowExists();
  bool isFollowTDirect(const Entity &lhs, const Entity &rhs);
  bool isFollowTRight(const Entity &lhs);
  bool isFollowTLeft(const Entity &lhs);

  std::unordered_set<Statement> getFollows(const Statement &s);
  std::unordered_set<Statement> getFollowT(const Statement &s);
  std::unordered_set<Statement> getFollowedByT(const Statement &s);
  std::unordered_set<Statement> getFollowedBy(const Statement &s);
  std::unordered_map<Statement, Statement> getAllFollows();
  std::unordered_map<Statement, std::unordered_set<Statement>> getAllFollowsT();
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_FOLLOWSTORAGE_H_
