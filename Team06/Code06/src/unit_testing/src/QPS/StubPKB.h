//
// Created by Eddy Chu on 2022/9/12.
//

#ifndef SPA_STUBPKB_H
#define SPA_STUBPKB_H

#include <PKB/PKBGetter.h>
#include <unordered_set>
#include <vector>

class StubPKB : public PKBGetter {
public:
  inline static std::unordered_set<Variable> VARIABLES = {"a", "b"};
  inline static std::unordered_set<Constant> CONSTANTS = {"100", "50"};
  inline static std::unordered_set<Statement> STATEMENTS = {"1", "2", "3"};
  inline static std::unordered_set<Procedure> PROCEDURES = {"test"};
  inline static std::unordered_map<Statement, Variable> isStmtModifies = {
      {"1", "a"}, {"2", "b"}};
  inline static std::unordered_map<Statement, Variable> isStmtUses = {
      {"1", "a"}, {"2", "b"}};

  std::unordered_set<Variable> getVariables() const override {
    return VARIABLES;
  }

  std::unordered_set<Constant> getConstants() const override {
    return CONSTANTS;
  }

  std::unordered_set<Statement> getStatements() const override {
    return STATEMENTS;
  }

  std::unordered_set<Procedure> getProcedures() const override {
    return PROCEDURES;
  }

  std::unordered_set<Procedure> getAssignStmts() const override {
    return STATEMENTS;
  }

  std::unordered_set<Procedure> getPrintStmts() const override {
    return STATEMENTS;
  }

  std::unordered_set<Procedure> getReadStmts() const override {
    return STATEMENTS;
  }

  virtual bool isStatementModifies(const Statement &s,
                                   const Variable &v) override {
    // test wildcard
    if (v == "_" && isStmtModifies.find(s) != isStmtModifies.end()) {
      return true;
    }

    if (isStmtModifies[s] == v) {
      return true;
    }
    return false;
  }

  virtual bool isFollows(const Statement &s1, const Statement &s2) override {
    return true;
  }

  virtual bool isProcedureModifies(const Procedure &p,
                                   const Variable &v) override {
    return true;
  }

  bool isFollowsT(const Statement &s1, const Statement &s2) override {
    return true;
  }

  virtual bool isStatementUses(const Statement &s, const Variable &v) override {
    // test wildcard
    if (v == "_" && isStmtUses.find(s) != isStmtUses.end()) {
      return true;
    }

    if (isStmtUses[s] == v) {
      return true;
    }
    return false;
  }

  virtual bool isProcedureUses(const Procedure &p, const Variable &v) override {
    if (p == "test" && v == "a") {
      return true;
    } else {
      return false;
    }
  }

  bool isParent(const Statement &s, const Statement &s2) override {
    return true;
  }

  bool isParentT(const Statement &s, const Statement &s2) override {
    return true;
  }

  bool isPatternMatches(const Statement &s, const StmtType &st, Entity &v,
                        Expression &exp) override {
    return true;
  }

  std::unordered_set<Variable> getUsesS(const Entity &s,
                                        bool isForward) override {
    if (!isForward) {
      if (s == "1") {
        return {"a"};
      }

      if (s == "2") {
        return {"b"};
      }
      return {};
    } else {
      if (s == "a") {
        return {"1"};
      }

      if (s == "b") {
        return {"2"};
      }
      return {};
    }
  }

  std::unordered_set<Entity> getModifiesS(const Entity &e,
                                          bool isForward) override {
    std::unordered_set<Entity> result;
    if (!isForward) {
      for (auto &[stmt, var] : isStmtModifies) {
        if (e == var) {
          result.insert(stmt);
        }
      }
    } else {
      if (isStmtModifies.find(e) == isStmtModifies.end()) {
        return {};
      }

      for (auto &[stmt, var] : isStmtModifies) {
        if (e == stmt) {
          result.insert(var);
        }
      }
    }

    return result;
  }
};

#endif // SPA_STUBPKB_H
