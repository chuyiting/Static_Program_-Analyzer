#include <PKB/PKBGetter.h>
#include <PKB/type/pkb_types.h>
#include <unordered_set>
#include <vector>

/** Doing the following source code for testing
procedure test {
        1: a = 1;
        2: b = a;
        3: print a;
        4: read b;
}
**/
class StubPKB2 : public PKBGetter {
  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtUses = {
          {"2", {"a"}},
          {"3", {"a"}},
      };

  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtModifies = {{"1", {"a"}}, {"2", {"b"}}, {"b", {"5"}}};

  inline static std::unordered_map<Statement, Variable> isStmtFollows = {
      {"1", "2"}, {"2", "3"}, {"3", "4"}};

  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtFollowsT = {
          {"1", {"2", "3", "4"}}, {"2", {"3", "4"}}, {"3", {"4"}}};

  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtParent = {};

  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtParentT = {};

public:
  StubPKB2() {}

  ~StubPKB2() {}

  virtual std::unordered_set<Variable> getVariables() const override {
    std::unordered_set<Variable> result;
    result.insert("a");
    result.insert("b");
    return result;
  }

  virtual std::unordered_set<Constant> getConstants() const override {
    std::unordered_set<Constant> result;
    result.insert("1");
    return result;
  }

  virtual std::unordered_set<Statement> getStatements() const override {
    std::unordered_set<Statement> result;
    result.insert("1");
    result.insert("2");
    result.insert("3");
    result.insert("4");

    return result;
  }

  virtual std::unordered_set<Procedure> getProcedures() const override {
    std::unordered_set<Procedure> result;
    result.insert("test");
    return result;
  }

  virtual std::unordered_set<Statement> getAssignStmts() const override {

    std::unordered_set<Statement> result;
    result.insert("1");
    result.insert("2");

    return result;
  }

  virtual std::unordered_set<Statement> getPrintStmts() const override {
    std::unordered_set<Statement> result;
    result.insert("3");

    return result;
  }

  virtual std::unordered_set<Statement> getReadStmts() const override {
    std::unordered_set<Statement> result;
    result.insert("4");

    return result;
  }

  std::unordered_set<Statement> getIfStmts() const override {
    std::unordered_set<Statement> result;
    return result;
  }

  std::unordered_set<Statement> getWhileStmts() const override {
    std::unordered_set<Statement> result;
    return result;
  }
  std::unordered_set<Variable> getUsesS(const Entity &s,
                                        bool isForward) override {
    if (!isForward) {
      std::unordered_set<Statement> result;
      for (auto &[stmt, vars] : isStmtUses) {
        if (vars.find(s) != vars.end()) {
          result.insert(stmt);
        }
      }
      return result;
    } else {
      if (isStmtUses.find(s) == isStmtUses.end()) {
        return {};
      }
      return isStmtUses[s];
    }
  }

  virtual bool isStatementModifies(const Statement &s,
                                   const Variable &v) override {
    if (v == "a" && s == "1") {
      return true;
    } else if (v == "b" && s == "2") {
      return true;
    } else {
      return false;
    }
  }

  virtual bool isFollows(const Statement &s1, const Statement &s2) override {
    if ((std::stoi(s2) - std::stoi(s1)) == 1) {
      return true;
    }
    return false;
  }

  virtual bool isFollowsT(const Statement &s1, const Statement &s2) override {
    if ((std::stoi(s2) - std::stoi(s1)) >= 1) {
      return true;
    }
    return false;
  }

  bool isProcedureModifies(const Procedure &p, const Variable &v) override {
    return true;
  }

  virtual bool isStatementUses(const Statement &s, const Variable &v) override {
    if ((v == "a" || v == "_") && (s == "2" || s == "3")) {
      return true;
    } else {
      return false;
    }
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
    if (s == "1" && v == "a" && exp == "_1_") {
      return true;
    } else if (s == "2" && v == "b" && exp == "_") {
      return true;
    } else if (s == "2" && v == "b" && exp == "_a_") {
      return true;
    } else if (s == "_") {
      if (v == "a" && exp == "1") {
        return true;
      } else if (v == "b" && exp == "_a_") {
        return true;
      } else if (v == "_" && exp == "_") {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  std::unordered_set<Statement> getFollows(const Statement &s,
                                           bool isForward) override {
    std::unordered_set<Statement> result;
    if (!isForward) {
      for (auto &[s1, s2] : isStmtFollows) {
        if (s2 == s) {
          result.insert(s1);
        }
      }
    } else {
      for (auto &[s1, s2] : isStmtFollows) {
        if (s1 == s) {
          result.insert(s2);
        }
      }
    }

    return result;
  }

  std::unordered_set<Statement> getFollowsT(const Statement &s,
                                            bool isForward) override {
    std::unordered_set<Statement> result;
    if (!isForward) {
      for (auto &[stmt, list] : isStmtFollowsT) {
        if (list.find(s) != list.end()) {
          result.insert(stmt);
        }
      }
    } else {
      if (isStmtFollowsT.find(s) == isStmtFollowsT.end()) {
        return {};
      }

      result = isStmtFollowsT[s];
    }

    return result;
  }

  std::unordered_set<Statement> getParentOf(const Statement &s,
                                            bool isForward) override {
    std::unordered_set<Statement> result;
    if (!isForward) {
      for (auto &[stmt, list] : isStmtParent) {
        if (list.find(s) != list.end()) {
          result.insert(stmt);
        }
      }
    } else {
      if (isStmtParent.find(s) == isStmtParent.end()) {
        return {};
      }

      result = isStmtParent[s];
    }

    return result;
  }
  std::unordered_set<Statement> getParentTOf(const Statement &s,
                                             bool isForward) override {
    std::unordered_set<Statement> result;
    if (!isForward) {
      for (auto &[stmt, list] : isStmtParentT) {
        if (list.find(s) != list.end()) {
          result.insert(stmt);
        }
      }
    } else {
      if (isStmtParentT.find(s) == isStmtParentT.end()) {
        return {};
      }

      result = isStmtParentT[s];
    }

    return result;
  }

  // returns either set of statements or set of procedures, if isForward is
  // false: set of variables
  std::unordered_set<Entity> getModifiesS(const Entity &e,
                                          bool isForward) override {
    std::unordered_set<Entity> result;
    if (!isForward) {
      for (auto &[stmt, list] : isStmtModifies) {
        if (list.find(e) != list.end()) {
          result.insert(stmt);
        }
      }
    } else {
      if (isStmtModifies.find(e) == isStmtModifies.end()) {
        return {};
      }

      result = isStmtModifies[e];
    }

    return result;
  }
};
