#include "ModifiesStorage.h"

ModifiesStorage::ModifiesStorage() = default;

void ModifiesStorage::addModifiesS(const Statement &s, const Variable &v) {
  modifiesS.insert(s, {v});
}

void ModifiesStorage::addModifiesP(const Procedure &p,
                                   const std::unordered_set<Variable> &v) {
  modifiesP.insert(p, v);
}

bool ModifiesStorage::isModifiesS(const Statement &lhs, const Variable &rhs) {
  return modifiesS.has_keymapping(lhs, rhs);
}

bool ModifiesStorage::isModifiesSRight(const Statement &lhs) {
  return modifiesS.has_key(lhs);
}

bool ModifiesStorage::isModifiesP(const Procedure &lhs, const Variable &rhs) {
  return modifiesP.has_keymapping(lhs, rhs);
}

bool ModifiesStorage::isModifiesPRight(const Procedure &lhs) {
  return modifiesP.has_key(lhs);
}

std::unordered_map<Statement, std::unordered_set<Variable>>
ModifiesStorage::getModifiesS() {
  return modifiesS.getAllKeyMapping();
}

std::unordered_map<Procedure, std::unordered_set<Variable>>
ModifiesStorage::getModifiesP() {
  return modifiesP.getAllKeyMapping();
}

std::unordered_set<Variable>
ModifiesStorage::getProcedureModifies(const Procedure &proc) {
  return modifiesP.get_valueset(proc);
}

std::unordered_set<Variable>
ModifiesStorage::getStatementModifies(const Statement &stmt) {
  return modifiesS.get_valueset(stmt);
}

std::unordered_set<Variable>
ModifiesStorage::getVariableModifiedS(const Variable &var) {
  return modifiesS.get_keyset(var);
}

std::unordered_set<Variable>
ModifiesStorage::getVariableModifiedP(const Variable &var) {
  return modifiesP.get_keyset(var);
}
