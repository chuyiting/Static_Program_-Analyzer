#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_MODIFIESSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_MODIFIESSTORAGE_H_
#include "PKB/Datastructure/biset.h"
#include "PKB/type/pkb_types.h"
#include "utils/map_utils.h"
#include <unordered_map>
#include <set>

using namespace pkb_types;
class ModifiesStorage {
  private:
    biset::unordered_biset<Statement, Variable> modifiesS;
    biset::unordered_biset<Procedure, Variable> modifiesP;
public:
  ModifiesStorage();

  void addModifiesS(const Statement &s, const Variable &v);
  void addModifiesP(const Procedure &p, const std::unordered_set<Variable> &v);

  bool isModifiesS(const Statement &lhs, const Variable &rhs);
  bool isModifiesSRight(const Statement &lhs);
  bool isModifiesP(const Procedure &lhs, const Variable &rhs);
  bool isModifiesPRight(const Procedure &lhs);

  std::unordered_map<Statement, std::unordered_set<Variable>> getModifiesS();
  std::unordered_map<Procedure, std::unordered_set<Variable>> getModifiesP();
  std::unordered_set<Variable> getProcedureModifies(const Procedure &proc);
  std::unordered_set<Variable> getStatementModifies(const Statement &stmt);
  std::unordered_set<Variable> getVariableModifiedS(const Variable &var);
  std::unordered_set<Variable> getVariableModifiedP(const Variable &var);
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_MODIFIESSTORAGE_H_
