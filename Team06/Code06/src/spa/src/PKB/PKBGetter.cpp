#include "PKBGetter.h"

PKBGetter::~PKBGetter() = default;

PKBGetter::PKBGetter() : _pkb(*new PKB()) {}

PKBGetter::PKBGetter(PKB &pkb) : _pkb(pkb) {}

/* ============================ GETTERS ============================ */

std::unordered_set<Statement> PKBGetter::getStatements() const {
  return _pkb._synonym_storage.getStatements();
}

std::unordered_set<Constant> PKBGetter::getConstants() const {
  return _pkb._synonym_storage.getConstants();
}

std::unordered_set<Variable> PKBGetter::getVariables() const {
  return _pkb._synonym_storage.getVariables();
}

std::unordered_set<Procedure> PKBGetter::getProcedures() const {
  return _pkb._synonym_storage.getProcedures();
}

std::unordered_set<Statement> PKBGetter::getReadStmts() const {
  return _pkb._synonym_storage.getReadStmts();
}

std::unordered_set<Statement> PKBGetter::getPrintStmts() const {
  return _pkb._synonym_storage.getPrintStmts();
}

std::unordered_set<Statement> PKBGetter::getAssignStmts() const {
  return _pkb._synonym_storage.getAssignStmts();
}

std::unordered_set<Statement> PKBGetter::getIfStmts() const {
  return _pkb._synonym_storage.getIfStmts();
}

std::unordered_set<Statement> PKBGetter::getWhileStmts() const {
  return _pkb._synonym_storage.getWhileStmts();
}

std::unordered_set<Statement> PKBGetter::getCallStmts() const {
  return _pkb._synonym_storage.getCallStmts();
}

/* ========================= TO BE MOVED INTO PKB ONLY FOR TESTING PURPOSES ======================== */

std::unordered_map<Statement, Statement> PKBGetter::getFollows() const {
  return _pkb._follows_storage.getAllFollows();
}

std::unordered_map<Statement, std::unordered_set<Statement>>
PKBGetter::getFollowsT() const {
  return _pkb._follows_storage.getAllFollowsT();
}

std::unordered_map<Statement, std::unordered_set<Statement>>
PKBGetter::getParent() const {
  return _pkb._parent_storage.getAllParents();
}

std::unordered_map<Statement, std::unordered_set<Statement>>
PKBGetter::getParentT() const {
  return _pkb._parent_storage.getAllParentsT();
}

std::unordered_map<Statement, std::unordered_set<Variable>>
PKBGetter::getUsesS() const {
  return _pkb._uses_storage.getUsesS();
}

std::unordered_map<Procedure, std::unordered_set<Variable>>
PKBGetter::getUsesP() const {
  return _pkb._uses_storage.getUsesP();
}

std::unordered_map<Statement, std::unordered_set<Variable>>
PKBGetter::getModifiesS() const {
  return _pkb._modifies_storage.getModifiesS();
}

std::unordered_map<Procedure, std::unordered_set<Variable>>
PKBGetter::getModifiesP() const {
  return _pkb._modifies_storage.getModifiesP();
}

std::unordered_map<Statement, std::unordered_set<Statement>>
PKBGetter::getNext() const {
  return _pkb._next_storage.getNext();
}

/* ========================= TO BE MOVED INTO PKB ONLY FOR TESTING PURPOSES ======================== */

bool PKBGetter::isProcedureUses(const Procedure &p, const Variable &v) {
  return _pkb.usesPResolver.run(p, v, "");
}

bool PKBGetter::isStatementUses(const Statement &s, const Variable &v) {
  return _pkb.usesSResolver.run(s, v, "");
}

bool PKBGetter::isProcedureModifies(const Procedure &p, const Variable &v) {
  return _pkb.modifiesPResolver.run(p, v, "");
}

bool PKBGetter::isStatementModifies(const Statement &s, const Variable &v) {
  return _pkb.modifiesSResolver.run(s, v, "");
}

bool PKBGetter::isFollowsT(const Statement &s1, const Statement &s2) {
  return _pkb.followTResolver.run(s1, s2, "");
}

bool PKBGetter::isFollows(const Statement &s1, const Statement &s2) {
  return _pkb.followsResolver.run(s1, s2, "");
}

bool PKBGetter::isParent(const Statement &s1, const Statement &s2) {
  return _pkb.parentResolver.run(s1, s2, "");
}

bool PKBGetter::isParentT(const Statement &s1, const Statement &s2) {
  return _pkb.parentTResolver.run(s1, s2, "");
}

bool PKBGetter::isCalls(const Procedure &p1, const Procedure &p2) {
  return _pkb.callsResolver.run(p1, p2, "");
}

bool PKBGetter::isCallsT(const Procedure &p1, const Procedure &p2) {
  return _pkb.callsTResolver.run(p1, p2, "");
}

bool PKBGetter::isPatternMatches(const Statement &s, const StmtType &st,
                                 Entity &lhs, Expression &rhs) {
  if (st == StmtType::ASSIGN) {
    return _pkb.assignPatternResolver.run(lhs, rhs, s);
  } else if (st == StmtType::IF) {
    return _pkb.ifPatternResolver.run(lhs, "_", s);
  } else if (st == StmtType::WHILE) {
    return _pkb.whilePatternResolver.run(lhs, "_", s);
  } else {
    return false;
  }
}

bool PKBGetter::isNext(const Statement &s1, const Statement &s2) {
  return _pkb.nextResolver.run(s1, s2, "");
}

bool PKBGetter::isNextT(const Statement &s1, const Statement &s2) {
  return _pkb.nextTResolver.run(s1, s2, "");
}

bool PKBGetter::isAffects(const Statement &s1, const Statement &s2) {
  return _pkb.affectsResolver.run(s1, s2, "");
}

bool PKBGetter::isAffectsT(const Statement &s1, const Statement &s2) {
  return _pkb.affectsTResolver.run(s1, s2, "");
}

/* ======================================== TO SUPPORT THE WITH CLAUSE ======================================= */

// Given a statement number that is guaranteed to be a call/read/print statement, return the variable involved.
Entity PKBGetter::getVarAt(const Statement &s) {
  return _pkb._synonym_storage.getVarAt(s);
}

// Given a procedure name, return the statement numbers of all call statements
// that call the given procedure.
std::unordered_set<Statement>
PKBGetter::getCallsByCalledProcName(const Procedure &p) {
  return _pkb._synonym_storage.getCallsByCalledProcName(p);
}

// Given a variable name, return the statement numbers of all read statements
// that read the variable.
std::unordered_set<Statement> PKBGetter::getReadsByVarName(const Variable &v) {
  return _pkb._synonym_storage.getReadsByVarName(v);
}

// Given a variable name, return the statement numbers of all print statements
// that print the variable.
std::unordered_set<Statement> PKBGetter::getPrintsByVarName(const Variable &v) {
  return _pkb._synonym_storage.getPrintsByVarName(v);
}

// Returns a set of all variables that are read in the program.
std::unordered_set<Variable> PKBGetter::getAllReadVars() {
  return _pkb._synonym_storage.getAllReadVars();
}

// Returns a set of all variables that are printed in the program.
std::unordered_set<Variable> PKBGetter::getAllPrintedVars() {
  return _pkb._synonym_storage.getAllPrintedVars();
}

// Returns a set of all procedure names, where the procedure is called in the
// program.
std::unordered_set<Procedure> PKBGetter::getAllProceduresCalled() {
  return _pkb._synonym_storage.getAllProceduresCalled();
}

/* ================ SET RETURNS FOR: isRelationship(_, entity) / isRelationship(entity, _) ================ */

std::unordered_set<Statement> PKBGetter::getFollows(const Statement &s, bool isRight) {
  return isRight ? _pkb._follows_storage.getFollows(s)
                 : _pkb._follows_storage.getFollowedBy(s);
}

std::unordered_set<Statement> PKBGetter::getFollowsT(const Statement &s, bool isRight) {
  return isRight ?  _pkb._follows_storage.getFollowT(s)
                 : _pkb._follows_storage.getFollowedByT(s);
}

std::unordered_set<Statement> PKBGetter::getParentOf(const Statement &s, bool isRight) {
  return isRight ? _pkb._parent_storage.getParents(s)
                 : _pkb._parent_storage.getChildren(s);
}

std::unordered_set<Statement> PKBGetter::getParentTOf(const Statement &s, bool isRight) {
  return isRight ? _pkb._parent_storage.getParentT(s)
                 : _pkb._parent_storage.getChildrenT(s);
}

std::unordered_set<Statement> PKBGetter::getCalls(const Procedure &p, bool isRight) {
  return isRight ? _pkb._call_storage.getCalls(p)
                 : _pkb._call_storage.getCalled(p);
}

std::unordered_set<Statement> PKBGetter::getCallsT(const Procedure &p, bool isRight) {
  return isRight ? _pkb._call_storage.getCallsT(p)
                 : _pkb._call_storage.getCalledT(p);
}

std::unordered_set<Entity> PKBGetter::getModifiesS(const Statement &s, bool isRight) {
  return isRight ? _pkb._modifies_storage.getStatementModifies(s)
                 : _pkb._modifies_storage.getVariableModifiedS(s);
}

std::unordered_set<Entity> PKBGetter::getModifiesP(const Procedure &p, bool isRight) {
  return isRight ? _pkb._modifies_storage.getProcedureModifies(p)
                 : _pkb._modifies_storage.getVariableModifiedP(p);
}

std::unordered_set<Entity> PKBGetter::getUsesS(const Statement &s, bool isRight) {
  return isRight ? _pkb._uses_storage.getStatementUses(s)
                 : _pkb._uses_storage.getVariableUsedS(s);
}

std::unordered_set<Entity> PKBGetter::getUsesP(const Procedure &p, bool isRight) {
  return isRight ? _pkb._uses_storage.getProcedureUses(p)
                 : _pkb._uses_storage.getVariableUsedP(p);
}

std::unordered_set<Statement> PKBGetter::getNext(const Statement &s, bool isRight) {
  return isRight ? _pkb._next_storage.getNextStatements(s)
                 : _pkb._next_storage.getPrevStatements(s);
}

std::unordered_set<Statement> PKBGetter::getNextT(const Statement &s, bool isRight) {
  return isRight ? _pkb._next_storage.getNextTStatements(s)
                 : _pkb._next_storage.getPrevTStatements(s);
}

std::unordered_set<Statement> PKBGetter::getAffects(const Statement &s, bool isRight) {
  return isRight ? _pkb._affects_storage.getAffects(s)
                 : _pkb._affects_storage.getAffectedBy(s);
}

std::unordered_set<Statement> PKBGetter::getAffectsT(const Statement &s, bool isRight) {
  return isRight ? _pkb._affects_storage.getAffectsT(s)
                 : _pkb._affects_storage.getAffectedByT(s);
}

/* ================================ QUERIES ================ */

bool PKBGetter::doesConstantExist(const Constant &c) {
  return _pkb._synonym_storage.doesConstantExist(c);
}

bool PKBGetter::isAssignmentStatement(const Statement &s) {
  return _pkb._synonym_storage.isAssignmentStatement(s);
}

bool PKBGetter::isIfStatement(const Statement &s) {
  return _pkb._synonym_storage.isIfStatement(s);
}

bool PKBGetter::isWhileStatement(const Statement &s) {
  return _pkb._synonym_storage.isWhileStatement(s);
}

bool PKBGetter::isReadStatement(const Statement &s) {
  return _pkb._synonym_storage.isReadStatement(s);
}

bool PKBGetter::isPrintStatement(const Statement &s) {
  return _pkb._synonym_storage.isPrintStatement(s);
}

bool PKBGetter::isCallStatement(const Statement &s) {
  return _pkb._synonym_storage.isCallStatement(s);
}
