#include "SynonymStorage.h"

SynonymStorage::SynonymStorage() = default;

SynonymStorage::~SynonymStorage() = default;

void SynonymStorage::addProcedure(const Procedure &p,
                                  const std::unordered_set<Statement> &s) {
  procedures.insert({p, s});
}

void SynonymStorage::addConstant(const Constant &c) { constants.insert(c); }

void SynonymStorage::addVariable(const Variable &v) { variables.insert(v); }

void SynonymStorage::addReadStmt(const Statement &s, const Variable &v) {
  readStmts.insert({s, v});
}

void SynonymStorage::addPrintStmt(const Statement &s, const Variable &v) {
  printStmts.insert({s, v});
}

void SynonymStorage::addAssignmentStmt(const Statement &s, const Variable &v) {
  assignStmts.insert({s, v});
}

void SynonymStorage::addIfStmt(const Statement &s) { ifStmts.insert(s); }

void SynonymStorage::addWhileStmt(const Statement &s) { whileStmts.insert(s); }

void SynonymStorage::addCallStmt(const Statement &s, const Procedure &callee) {
  callStmts.insert({s, callee});
}

std::unordered_map<Procedure, std::unordered_set<Statement>> SynonymStorage::getProcedureStatements() {
  return procedures;
}

std::unordered_set<Procedure> SynonymStorage::getProcedures() const {
  return map_utils::keys(procedures);
}

std::unordered_set<Constant> SynonymStorage::getConstants() const {
  return constants;
}

std::unordered_set<Variable> SynonymStorage::getVariables() const {
  return variables;
}

std::unordered_set<Statement> SynonymStorage::getReadStmts() const {
  return map_utils::keys(readStmts);
}

std::unordered_set<Statement> SynonymStorage::getPrintStmts() const {
  return map_utils::keys(printStmts);
}

std::unordered_set<Statement> SynonymStorage::getAssignStmts() const {
  return map_utils::keys(assignStmts);
}

std::unordered_set<Statement> SynonymStorage::getIfStmts() const {
  return ifStmts;
}

std::unordered_set<Statement> SynonymStorage::getWhileStmts() const {
  return whileStmts;
}

std::unordered_set<Statement> SynonymStorage::getCallStmts() const {
  return map_utils::keys(callStmts);
}

std::unordered_set<Statement> SynonymStorage::getStatements() const {
  std::unordered_set<Statement> s;
  // print and assign
  for (auto const &stmts : procedures) {
    // if procedure is empty may insert unexpected element
    if (!stmts.second.empty()) {
      std::copy(stmts.second.begin(), stmts.second.end(),
                std::inserter(s, s.begin()));
    }
  }
  return s;
}

Entity SynonymStorage::getVarAt(const Statement &s) {
  if (map_utils::isIterableContains(callStmts,
                                    s)) { // If it is a call statememt
    return callStmts.at(s);
  } else if (map_utils::isIterableContains(readStmts,
                                           s)) { // If it is a read statememt
    return readStmts.at(s);
  } else if (map_utils::isIterableContains(printStmts,
                                           s)) { // If it is a print statememt
    return printStmts.at(s);
  } else {
    throw std::runtime_error(
        "Invalid statement number"); // Should not reach this line
  }
}

std::unordered_set<Statement>
SynonymStorage::getCallsByCalledProcName(const Procedure &p) {
  std::unordered_set<Statement> callStatementNums;
  for (auto &pair : callStmts) {
    if (pair.second == p) {
      callStatementNums.insert(pair.first);
    }
  }
  return callStatementNums;
}

std::unordered_set<Statement>
SynonymStorage::getReadsByVarName(const Variable &v) {
  std::unordered_set<Statement> readStatementNums;
  for (auto &pair : readStmts) {
    if (pair.second == v) {
      readStatementNums.insert(pair.first);
    }
  }
  return readStatementNums;
}

std::unordered_set<Statement>
SynonymStorage::getPrintsByVarName(const Variable &v) {
  std::unordered_set<Statement> printStatementNums;
  for (auto &pair : printStmts) {
    if (pair.second == v) {
      printStatementNums.insert(pair.first);
    }
  }
  return printStatementNums;
}

std::unordered_set<Variable> SynonymStorage::getAllReadVars() {
  return map_utils::values(readStmts);
}

std::unordered_set<Variable> SynonymStorage::getAllPrintedVars() {
  return map_utils::values(printStmts);
}

std::unordered_set<Procedure> SynonymStorage::getAllProceduresCalled() {
  return map_utils::values(callStmts);
}

bool SynonymStorage::doesConstantExist(const Constant &c) {
  return map_utils::isIterableContains(constants, c);
}

bool SynonymStorage::isAssignmentStatement(const Statement &s) {
  return map_utils::isIterableContains(assignStmts, s);
}

bool SynonymStorage::isIfStatement(const Statement &s) {
  return map_utils::isIterableContains(ifStmts, s);
}

bool SynonymStorage::isWhileStatement(const Statement &s) {
  return map_utils::isIterableContains(whileStmts, s);
}

bool SynonymStorage::isReadStatement(const Statement &s) {
  return map_utils::isIterableContains(readStmts, s);
}

bool SynonymStorage::isPrintStatement(const Statement &s) {
  return map_utils::isIterableContains(printStmts, s);
  ;
}

bool SynonymStorage::isCallStatement(const Statement &s) {
  return map_utils::isIterableContains(callStmts, s);
}
