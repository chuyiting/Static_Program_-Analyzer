#ifndef SPA_SRC_SPA_SRC_PKB_PKBGETTER_H_
#define SPA_SRC_SPA_SRC_PKB_PKBGETTER_H_

#include "PKB.h"

class PKBGetter {
private:
  PKB &_pkb;

public:
  explicit PKBGetter(PKB &pkb);
  PKBGetter();
  ~PKBGetter();

  // entRefs
  [[nodiscard]] virtual std::unordered_set<Procedure> getProcedures() const;
  [[nodiscard]] virtual std::unordered_set<Constant> getConstants() const;
  [[nodiscard]] virtual std::unordered_set<Variable> getVariables() const;
  // stmtRefs
  [[nodiscard]] virtual std::unordered_set<Statement> getReadStmts() const;
  [[nodiscard]] virtual std::unordered_set<Statement> getPrintStmts() const;
  [[nodiscard]] virtual std::unordered_set<Statement> getAssignStmts() const;
  [[nodiscard]] virtual std::unordered_set<Statement> getIfStmts() const;
  [[nodiscard]] virtual std::unordered_set<Statement> getWhileStmts() const;
  [[nodiscard]] virtual std::unordered_set<Statement> getCallStmts() const;
  [[nodiscard]] virtual std::unordered_set<Statement> getStatements() const;
  // abstractions
  [[nodiscard]] virtual std::unordered_map<Statement, Statement>
  getFollows() const;
  [[nodiscard]] virtual std::unordered_map<Statement,
                                           std::unordered_set<Statement>>
  getFollowsT() const;
  [[nodiscard]] virtual std::unordered_map<Statement,
                                           std::unordered_set<Statement>>
  getParent() const;
  [[nodiscard]] virtual std::unordered_map<Statement,
                                           std::unordered_set<Statement>>
  getParentT() const;
  [[nodiscard]] virtual std::unordered_map<Statement,
                                           std::unordered_set<Variable>>
  getUsesS() const;
  [[nodiscard]] virtual std::unordered_map<Procedure,
                                           std::unordered_set<Variable>>
  getUsesP() const;
  [[nodiscard]] virtual std::unordered_map<Statement,
                                           std::unordered_set<Variable>>
  getModifiesS() const;
  [[nodiscard]] virtual std::unordered_map<Procedure,
                                           std::unordered_set<Variable>>
  getModifiesP() const;
  [[nodiscard]] virtual std::unordered_map<Statement,
                                           std::unordered_set<Statement>>
  getNext() const;

    virtual bool isProcedureUses(const Procedure &p, const Variable &v);
    virtual bool isStatementUses(const Statement &s, const Variable &v);
    virtual bool isStatementModifies(const Statement &s, const Variable &v);
    virtual bool isProcedureModifies(const Procedure &p, const Variable &v);
    virtual bool isFollows(const Statement &s1, const Statement &s2);
    virtual bool isFollowsT(const Statement &s1, const Statement &s2);
    virtual bool isParent(const Statement &s1, const Statement &s2);
    virtual bool isParentT(const Statement &s1, const Statement &s2);
    virtual bool isCalls(const Procedure &p1, const Procedure &p2);
    virtual bool isCallsT(const Procedure &p1, const Procedure &p2);
    virtual bool isPatternMatches(const Statement &s, const StmtType &st, Entity &lhs, Expression &rhs);
    virtual bool isNext(const Statement &s1, const Statement &s2);
    virtual bool isNextT(const Statement &s1, const Statement &s2);
    virtual bool isAffects(const Statement &s1, const Statement &s2);
    virtual bool isAffectsT(const Statement &s1, const Statement &s2);

    // To support the With clause
    virtual Entity getVarAt(const Statement &s);
    virtual std::unordered_set<Statement> getCallsByCalledProcName(const Procedure &p);
    virtual std::unordered_set<Statement> getReadsByVarName(const Variable &v);
    virtual std::unordered_set<Statement> getPrintsByVarName(const Variable &v);
    virtual std::unordered_set<Variable> getAllReadVars();
    virtual std::unordered_set<Variable> getAllPrintedVars();
    virtual std::unordered_set<Procedure> getAllProceduresCalled();

    // Set Return Domain Optimisations
    virtual std::unordered_set<Statement> getFollows(const Statement &s, bool isRight);
    virtual std::unordered_set<Statement> getFollowsT(const Statement &s, bool isRight);
    virtual std::unordered_set<Statement> getParentOf(const Statement &s, bool isRight);
    virtual std::unordered_set<Statement> getParentTOf(const Statement &s, bool isRight);
    virtual std::unordered_set<Statement> getCalls(const Procedure &p, bool isRight);
    virtual std::unordered_set<Statement> getCallsT(const Procedure &p, bool isRight);
    virtual std::unordered_set<Entity> getModifiesS(const Statement &s, bool isRight);
    virtual std::unordered_set<Entity> getModifiesP(const Procedure &p, bool isRight);
    virtual std::unordered_set<Entity> getUsesS(const Statement &s, bool isRight);
    virtual std::unordered_set<Entity> getUsesP(const Procedure &p, bool isRight);
    virtual std::unordered_set<Statement> getNext(const Statement &s, bool isRight);
    virtual std::unordered_set<Statement> getNextT(const Statement &s, bool isRight);
    virtual std::unordered_set<Statement> getAffects(const Statement &s, bool isRight);
    virtual std::unordered_set<Statement> getAffectsT(const Statement &s, bool isRight);

    virtual bool doesConstantExist(const Constant &c);
    virtual bool isAssignmentStatement(const Statement &s);
    virtual bool isIfStatement(const Statement &s);
    virtual bool isWhileStatement(const Statement &s);
    virtual bool isReadStatement(const Statement &s);
    virtual bool isPrintStatement(const Statement &s);
    virtual bool isCallStatement(const Statement &s);
};

#endif // SPA_SRC_SPA_SRC_PKB_PKBGETTER_H_