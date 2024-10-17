#include "PKBSetter.h"
#include "PKB/PKB.h"

#include <unordered_set>

using namespace pkb_types;

PKBSetter::~PKBSetter() = default;

PKBSetter::PKBSetter(PKB &pkb) : _pkb(pkb) {}

/* ================ Entities ================ */

void PKBSetter::addProcedure(const Procedure &p,
                             const std::unordered_set<Statement> &s) {
  _pkb._synonym_storage.addProcedure(p, s);
  _pkb._call_storage.addProcedure(p);
}

void PKBSetter::addConstant(const Constant &c) {
  _pkb._synonym_storage.addConstant(c);
}

void PKBSetter::addVariable(const Variable &v) {
  _pkb._synonym_storage.addVariable(v);
}

void PKBSetter::addReadStmt(const Statement &s, const Variable &v) {
  _pkb._synonym_storage.addReadStmt(s, v);
}

void PKBSetter::addPrintStmt(const Statement &s, const Variable &v) {
  _pkb._synonym_storage.addPrintStmt(s, v);
}

void PKBSetter::addAssignmentStmt(const Statement &s, const Variable &v) {
  _pkb._synonym_storage.addAssignmentStmt(s, v);
}

void PKBSetter::addIfStmt(const Statement &s) {
  _pkb._synonym_storage.addIfStmt(s);
}

void PKBSetter::addWhileStmt(const Statement &s) {
  _pkb._synonym_storage.addWhileStmt(s);
}

void PKBSetter::addCallStmt(const Statement &s, const Procedure &callee) {
  _pkb._synonym_storage.addCallStmt(s, callee);
}

/* ================ Abstractions ================ */

void PKBSetter::addFollowsT(const std::vector<Statement> &s) {
  _pkb._follows_storage.addFollowT(s);
}

void PKBSetter::addParent(const Statement &parent,
                          const std::vector<Statement> &children) {
  _pkb._parent_storage.addParent(parent, children);
}

void PKBSetter::addUsesS(const Statement &s,
                         const std::unordered_set<Variable> &v) {
  _pkb._uses_storage.addUsesS(s, v);
};

void PKBSetter::addUsesIf(const Statement &s,
                          const std::unordered_set<Variable> &v) {
  _pkb._pattern_storage.addUsesIf(s, v);
}

void PKBSetter::addUsesWhile(const Statement &s,
                             const std::unordered_set<Variable> &v) {
  _pkb._pattern_storage.addUsesWhile(s, v);
}

void PKBSetter::addUsesP(const Procedure &p,
                         const std::unordered_set<Variable> &v) {
  _pkb._uses_storage.addUsesP(p, v);
}

void PKBSetter::addModifiesS(const Statement &s, const Variable &v) {
  _pkb._modifies_storage.addModifiesS(s, v);
  _pkb._pattern_storage.addModifiesS(s, v);
}

void PKBSetter::addModifiesP(const Procedure &p,
                             const std::unordered_set<Variable> &v) {
  _pkb._modifies_storage.addModifiesP(p, v);
}

void PKBSetter::addNextRelation(const Statement &parent,
                                const Statement &child) {
  _pkb._next_storage.addNext(parent, child);
}

void PKBSetter::addCallRelation(const Procedure &caller,
                                const Procedure &callee) {
  _pkb._call_storage.addCall(caller, callee);
}

/* ================ Others ================ */

void PKBSetter::addAssignmentExpr(const Statement &s,
                                  const std::vector<std::string> &e) {
  _pkb._pattern_storage.addAssignmentExpr(s, e);
}

void PKBSetter::done() { _pkb.done(); }
