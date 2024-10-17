#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_USESSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_USESSTORAGE_H_
#include "PKB/Datastructure/biset.h"
#include "PKB/type/pkb_types.h"
#include "utils/map_utils.h"
#include <unordered_map>
#include <unordered_set>

using namespace pkb_types;
class UsesStorage {
private:
  biset::unordered_biset<Statement, Variable> usesS;
  biset::unordered_biset<Procedure, Variable> usesP;

public:
  UsesStorage();

  void addUsesS(const Statement &s, const std::unordered_set<Variable> &v);
  void addUsesP(const Procedure &p, const std::unordered_set<Variable> &v);

  bool isUsesS(const Statement &lhs, const Variable &rhs);
  bool isUsesSRight(const Statement &lhs);
  bool isUsesP(const Procedure &lhs, const Variable &rhs);
  bool isUsesPRight(const Procedure &lhs);

  std::unordered_set<Variable> getProcedureUses(const Procedure &proc);
  std::unordered_set<Variable> getStatementUses(const Statement &stmt);

  std::unordered_map<Statement, std::unordered_set<Variable>> getUsesS();
  std::unordered_map<Procedure, std::unordered_set<Variable>> getUsesP();
  std::unordered_set<Statement> getVariableUsedP(const Variable &var);
  std::unordered_set<Statement> getVariableUsedS(const Variable &var);
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_MODIFIESSTORAGE_H_
