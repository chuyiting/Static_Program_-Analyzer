#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_CALLSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_CALLSTORAGE_H_
#include "PKB/Datastructure/bimap.h"
#include "PKB/Datastructure/dag/DAG.h"
#include "PKB/Datastructure/dag/TreeNode.h"
#include "PKB/type/pkb_types.h"

using namespace pkb_types;
class CallStorage {
  using ident = int;
  DAG<TreeNode> callsStorage;
  // Has been rerooted
  bool isComplete;

  // procedure begins at id = 1 (see constructor)
  ident proc;
  bimap::bimap<Procedure, ident> identMapping;
  // Generates a internal mapping of procedure to id.
  int generateIdentityMap(const Procedure &p);
  Procedure mapToProc(ident id);
  ident mapToId(const Procedure &procedure);
  bool hasProcedure(const Procedure &p);
  void executeReroot();

public:
  CallStorage();

  void addProcedure(const Procedure &p);
  void addCall(const Procedure &caller, const Procedure &callee);

  bool isCallsDirect(const Entity &lhs, const Entity &rhs);
  bool isCallsRight(const Entity &lhs);
  bool isCallsLeft(const Entity &lhs);
  bool isCallsExists();
  bool isCallsTDirect(const Entity &lhs, const Entity &rhs);
  bool isCallsTRight(const Entity &lhs);
  bool isCallsTLeft(const Entity &lhs);

  std::unordered_set<Procedure> getCalls(const Statement &s);
  std::unordered_set<Procedure> getCallsT(const Statement &s);
  std::unordered_set<Procedure> getCalled(const Statement &s);
  std::unordered_set<Procedure> getCalledT(const Statement &s);
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_CALLSTORAGE_H_
