
#ifndef SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_DATASTORE_SYNONYMSTORAGE_H_
#define SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_DATASTORE_SYNONYMSTORAGE_H_

#include <unordered_set>
#include <unordered_map>
#include <iterator>
#include <stdexcept>
#include "PKB/type/pkb_types.h"
#include "utils/map_utils.h"

using namespace pkb_types;

class SynonymStorage {
  std::unordered_set<Constant> constants;
  std::unordered_set<Variable> variables;
  std::unordered_map<Statement, Variable> readStmts;
  std::unordered_map<Statement, Variable> printStmts;
  std::unordered_map<Statement, Variable> assignStmts;
  std::unordered_set<Statement> ifStmts;
  std::unordered_set<Statement> whileStmts;
  std::unordered_map<Procedure, std::unordered_set<Statement>> procedures;
  std::unordered_map<Statement, Procedure> callStmts;

public:
  SynonymStorage();
  ~SynonymStorage();

  void addProcedure(const Procedure &p, const std::unordered_set<Statement> &s);
  void addConstant(const Constant &c);
  void addVariable(const Variable &v);
  void addReadStmt(const Statement &s, const Variable &v);
  void addPrintStmt(const Statement &s, const Variable &v);
  void addAssignmentStmt(const Statement &s, const Variable &v);
  void addIfStmt(const Statement &s);
  void addWhileStmt(const Statement &s);
  void addCallStmt(const Statement &s, const Procedure &callee);

    // entRefs
    [[nodiscard]] std::unordered_map<Procedure, std::unordered_set<Statement>> getProcedureStatements();
    [[nodiscard]] std::unordered_set<Procedure> getProcedures() const;
    [[nodiscard]] std::unordered_set<Constant> getConstants() const;
    [[nodiscard]] std::unordered_set<Variable> getVariables() const;
    // stmtRefs
    [[nodiscard]] std::unordered_set<Statement> getReadStmts() const;
    [[nodiscard]] std::unordered_set<Statement> getPrintStmts() const;
    [[nodiscard]] std::unordered_set<Statement> getAssignStmts() const;
    [[nodiscard]] std::unordered_set<Statement> getIfStmts() const;
    [[nodiscard]] std::unordered_set<Statement> getWhileStmts() const;
    [[nodiscard]] std::unordered_set<Statement> getCallStmts() const;
    [[nodiscard]] std::unordered_set<Statement> getStatements() const;

    Entity getVarAt(const Statement &s);
    std::unordered_set<Statement> getCallsByCalledProcName(const Procedure &p);
    std::unordered_set<Statement> getReadsByVarName(const Variable &v);
    std::unordered_set<Statement> getPrintsByVarName(const Variable &v);
    std::unordered_set<Variable> getAllReadVars();
    std::unordered_set<Variable> getAllPrintedVars();
    std::unordered_set<Procedure> getAllProceduresCalled();
    bool doesConstantExist(const Constant &c);
    bool isAssignmentStatement(const Statement &s);
    bool isIfStatement(const Statement &s);
    bool isWhileStatement(const Statement &s);
    bool isReadStatement(const Statement &s);
    bool isPrintStatement(const Statement &s);
    bool isCallStatement(const Statement &s);
};

#endif // SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_DATASTORE_SYNONYMSTORAGE_H_
