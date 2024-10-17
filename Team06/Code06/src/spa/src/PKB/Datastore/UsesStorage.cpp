#include "UsesStorage.h"

UsesStorage::UsesStorage() = default;

void UsesStorage::addUsesS(const Statement &s,
                           const std::unordered_set<Variable> &v) {
  usesS.insert(s, v);
}

void UsesStorage::addUsesP(const Procedure &p,
                           const std::unordered_set<Variable> &v) {
  usesP.insert(p, v);
}

bool UsesStorage::isUsesS(const Statement &lhs, const Variable &rhs) {
  return usesS.has_keymapping(lhs, rhs);
}

bool UsesStorage::isUsesSRight(const Statement &lhs) {
  return usesS.has_key(lhs);
}

bool UsesStorage::isUsesP(const Procedure &lhs, const Variable &rhs) {
  return usesP.has_keymapping(lhs, rhs);
}

bool UsesStorage::isUsesPRight(const Procedure &lhs) {
  return usesP.has_key(lhs);
}

std::unordered_map<Statement, std::unordered_set<Variable>>
UsesStorage::getUsesS() {
  return usesS.getAllKeyMapping();
}

std::unordered_map<Procedure, std::unordered_set<Variable>>
UsesStorage::getUsesP() {
  return usesP.getAllKeyMapping();
}

std::unordered_set<Variable>
UsesStorage::getProcedureUses(const Procedure &proc) {
  return usesP.get_valueset(proc);
}

std::unordered_set<Statement>
UsesStorage::getVariableUsedP(const Variable &var) {
  return usesP.get_keyset(var);
}

std::unordered_set<Statement>
UsesStorage::getVariableUsedS(const Variable &var) {
  return usesS.get_keyset(var);
}

std::unordered_set<Variable>
UsesStorage::getStatementUses(const Statement &stmt) {
  return usesS.get_valueset(stmt);
}
