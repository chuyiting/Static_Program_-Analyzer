#ifndef SPA_SETTER_H
#define SPA_SETTER_H

#include "PKB/PKB.h"

class PKBSetter {
private:
  PKB &_pkb;

public:
  explicit PKBSetter(PKB &pkb);

  ~PKBSetter();

  /* ================ Entities ================ */

  void addProcedure(const Procedure &p, const std::unordered_set<Statement> &s);
  void addConstant(const Constant &c);
  void addVariable(const Variable &v);
  void addReadStmt(const Statement &s, const Variable &v);
  void addPrintStmt(const Statement &s, const Variable &v);
  void addAssignmentStmt(const Statement &s, const Variable &v);
  void addIfStmt(const Statement &s);
  void addWhileStmt(const Statement &s);
  void addCallStmt(const Statement &s, const Procedure &p);

  /* ================ Abstractions ================ */

  void addFollowsT(const std::vector<Statement> &s1);
  void addParent(const Statement &parent, const std::vector<Statement> &children);
  void addUsesS(const Statement &s, const std::unordered_set<Variable> &v);
  void addUsesIf(const Statement &s, const std::unordered_set<Variable> &v);
  void addUsesWhile(const Statement &s, const std::unordered_set<Variable> &v);
  void addUsesP(const Procedure &p, const std::unordered_set<Variable> &v);
  void addModifiesS(const Statement &s, const Variable &v);
  void addModifiesP(const Procedure &p, const std::unordered_set<Variable> &v);
  void addNextRelation(const Statement &parent, const Statement &child);
  void addCallRelation(const Procedure &caller, const Procedure &callee);

  /* ================ Others ================ */

  void addAssignmentExpr(const Statement &s, const std::vector<std::string> &e);
  void done();
};

#endif // SPA_SETTER_H
