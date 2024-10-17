//
// Created by Eddy Chu on 2022/9/15.
//

#ifndef SPA_STUBPKB3_H
#define SPA_STUBPKB3_H

#include <PKB/PKBGetter.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/** Doing the following source code for testing
procedure Bedok {
1.  print east;
2.  west=9+east;
3.  y=east-4;
4.  z=west+2;
5.  call Yishun;
6.  west=9+x*y*z*east+west;
7.  while (east < 0) {
8.      x = z - east;
9.      if (east > x) then {
10.         x = x + 1;
11.         east = z - 9 + (x * y) * z; }
        else {
12.         y = east * y * z;
13.         call Yishun; }}
14. west=9+x*y*z*east+west;
15. print west; }

procedure Yishun {
16. if(west>y)then{
17.     east = z * 9 + east; }
    else {
18.     west = 4 + east + west; }
19. call Serangoon; }

procedure Serangoon {
20. read x;
21. read y; }
**/

class StubPKB3 : public PKBGetter {
public:
  inline static int NUM_STMT = 21;

  inline static std::unordered_set<Variable> VARIABLES = {"east", "west", "y",
                                                          "z", "x"};
  inline static std::unordered_set<Constant> CONSTANTS = {"9", "4", "2", "0",
                                                          "1"};
  inline static std::unordered_set<Statement> STATEMENTS;
  inline static std::unordered_set<std::string> ASSIGNS = {
      "2", "3", "4", "6", "8", "10", "11", "12", "14", "17", "18"};
  inline static std::unordered_set<Procedure> PROCEDURES = {"Bedok", "Yishun",
                                                            "Serangoon"};
  inline static std::unordered_set<Procedure> CALLS = {"5", "13", "19"};
  inline static std::unordered_set<std::string> READS = {"20", "21"};
  inline static std::unordered_set<std::string> PRINTS = {"1", "15"};
  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtModifies = {{"2", {"west"}},    {"3", {"y"}},
                        {"4", {"z"}},       {"5", {"east", "west", "x", "y"}},
                        {"6", {"west"}},    {"7", {"x", "east", "y", "west"}},
                        {"8", {"x"}},       {"9", {"x", "y", "east", "west"}},
                        {"10", {"x"}},      {"11", {"east"}},
                        {"12", {"y"}},      {"13", {"east", "west", "x", "y"}},
                        {"14", {"west"}},   {"16", {"east", "west", "x", "y"}},
                        {"17", {"east"}},   {"18", {"west"}},
                        {"19", {"x", "y"}}, {"20", {"x"}},
                        {"21", {"y"}}};
  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtUses = {
          {"1", {"east"}},
          {"2", {"east"}},
          {"3", {"east"}},
          {"4", {"west"}},
          {"5", {"east", "west", "z"}},
          {"6", {"west", "east", "x", "y", "z"}},
          {"7", {"west", "east", "x", "y", "z"}},
          {"8", {"z", "west"}},
          {"9", {"x", "y", "z", "east", "west"}},
          {"10", {"x"}},
          {"11", {"x", "y", "z"}},
          {"12", {"east", "y", "z"}},
          {"13", {"east", "west", "z"}},
          {"14", {"x", "y", "z", "east", "west"}},
          {"15", {"west"}},
          {"16", {"east", "west", "z", "y"}},
          {"17", {"east", "z"}},
          {"18", {"west", "east"}},
      };

  inline static std::unordered_map<Statement, Variable> isStmtFollows = {
      {"1", "2"},   {"2", "3"},   {"3", "4"},   {"4", "5"}, {"5", "6"},
      {"6", "7"},   {"7", "14"},  {"14", "15"}, {"8", "9"}, {"10", "11"},
      {"12", "13"}, {"16", "19"}, {"20", "21"},
  };

  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtFollowsT = {{"1", {"2", "3", "4", "5", "6", "7", "14", "15"}},
                        {"2", {"3", "4", "5", "6", "7", "14", "15"}},
                        {"3", {"4", "5", "6", "7", "14", "15"}},
                        {"4", {"5", "6", "7", "14", "15"}},
                        {"5", {"6", "7", "14", "15"}},
                        {"6", {"7", "14", "15"}},
                        {"7", {"14", "15"}},
                        {"8", {"9"}},
                        {"10", {"11"}},
                        {"12", {"13"}},
                        {"16", {"19"}},
                        {"20", {"21"}}};

  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtParent = {
          {"7", {"8", "9"}},
          {"9", {"10", "11", "12", "13"}},
          {"16", {"17", "18"}},
      };

  inline static std::unordered_map<Statement, std::unordered_set<Variable>>
      isStmtParentT = {
          {"7", {"8", "9", "10", "11", "12", "13"}},
          {"9", {"10", "11", "12", "13"}},
          {"16", {"17", "18"}},
      };

  inline static std::unordered_map<Statement, std::string> stmtVarMap = {
      {"1", "east"},       {"5", "Yishun"}, {"13", "Yishun"}, {"15", "west"},
      {"19", "Serangoon"}, {"20", "x"},     {"21", "y"},
  };

  inline static std::unordered_map<Statement, std::string> callProcMap = {
      {"5", "Yishun"}, {"13", "Yishun"}, {"19", "Serangoon"}};

  inline static std::unordered_map<Statement, std::string> printVarMap = {
      {"1", "east"}, {"15", "west"}};

  inline static std::unordered_map<Statement, std::string> readVarMap = {
      {"20", "x"}, {"21", "y"}};

  StubPKB3() {
    // init statements
    for (int i = 0; i < NUM_STMT; i++) {
      STATEMENTS.insert(std::to_string(i + 1));
    }
  }

  virtual std::unordered_set<Variable> getVariables() const override {
    return VARIABLES;
  }

  virtual std::unordered_set<Constant> getConstants() const override {
    return CONSTANTS;
  }

  virtual std::unordered_set<Statement> getStatements() const override {
    return STATEMENTS;
  }

  virtual std::unordered_set<Procedure> getProcedures() const override {
    return PROCEDURES;
  }

  virtual std::unordered_set<Statement> getCallStmts() const override {
    return CALLS;
  }

  std::unordered_set<Procedure> getAssignStmts() const override {
    return ASSIGNS;
  }

  std::unordered_set<Procedure> getPrintStmts() const override {
    return PRINTS;
  }

  std::unordered_set<Procedure> getReadStmts() const override { return READS; }

  virtual bool isStatementModifies(const Statement &s,
                                   const Variable &v) override {
    // test wildcard
    if (v == "_" && isStmtModifies.find(s) != isStmtModifies.end()) {
      return true;
    }

    std::unordered_set<std::string> vars = isStmtModifies[s];
    if (vars.find(v) != vars.end()) {
      return true;
    }
    return false;
  }

  virtual bool isFollows(const Statement &s1, const Statement &s2) override {
    // test wildcard
    if (s1 == "_" && s2 == "_") {
      return true;
    }

    if (s1 == "_") {
      // for testing, efficiency is not an issue
      for (auto &[_, x] : isStmtFollows) {
        if (s2 == x) {
          return true;
        }
      }
      return false;
    }

    if (s2 == "_") {
      if (isStmtFollows.find(s1) != isStmtFollows.end()) {
        return true;
      }
      return false;
    }

    return s2 == isStmtFollows[s1];
  }

  virtual bool isProcedureModifies(const Procedure &p,
                                   const Variable &v) override {
    // out of scope
    return true;
  }

  bool isFollowsT(const Statement &s1, const Statement &s2) override {
    // test wildcard
    if (s1 == "_" && s2 == "_") {
      return true;
    }

    if (s1 == "_") {
      // for testing, efficiency is not an issue
      for (auto &[_, xs] : isStmtFollowsT) {
        if (xs.find(s2) != xs.end()) {
          return true;
        }
      }
      return false;
    }

    if (s2 == "_") {
      if (isStmtFollowsT.find(s1) != isStmtFollowsT.end()) {
        return true;
      }
      return false;
    }

    std::unordered_set<Statement> set = isStmtFollowsT[s1];
    return set.find(s2) != set.end();
  }

  virtual bool isStatementUses(const Statement &s, const Variable &v) override {
    // test wildcard
    if (v == "_" && isStmtUses.find(s) != isStmtUses.end()) {
      return true;
    }

    if (isStmtUses[s].find(v) != isStmtUses[s].end()) {
      return true;
    }
    return false;
  }

  virtual bool isProcedureUses(const Procedure &p, const Variable &v) override {
    // out of scope
    return true;
  }

  bool isParent(const Statement &s1, const Statement &s2) override {
    // test wildcard
    if (s1 == "_" && s2 == "_") {
      return true;
    }

    if (s1 == "_") {
      // for testing, efficiency is not an issue
      for (auto &[_, x] : isStmtParent) {
        if (x.find(s2) != x.end()) {
          return true;
        }
      }
      return false;
    }

    if (s2 == "_") {
      if (isStmtParent.find(s1) != isStmtParent.end()) {
        return true;
      }
      return false;
    }

    std::unordered_set<Statement> set = isStmtParent[s1];
    return set.find(s2) != set.end();
  }

  bool isParentT(const Statement &s1, const Statement &s2) override {
    // test wildcard
    if (s1 == "_" && s2 == "_") {
      return true;
    }

    if (s1 == "_") {
      // for testing, efficiency is not an issue
      for (auto &[_, x] : isStmtParentT) {
        if (x.find(s2) != x.end()) {
          return true;
        }
      }
      return false;
    }

    if (s2 == "_") {
      if (isStmtParentT.find(s1) != isStmtParentT.end()) {
        return true;
      }
      return false;
    }

    std::unordered_set<Statement> set = isStmtParentT[s1];
    return set.find(s2) != set.end();
  }

  // WARNING: this part is very hacky
  bool isPatternMatches(const Statement &s, const StmtType &st, Entity &v,
                        Expression &exp) override {
    if (exp == "_east+west_") {
      return false;
    }

    if (exp == "_x*y*z_") {
      return s == "6" || s == "11" || s == "14";
    }

    if (exp == "_y_") {

      if (s == "6" && v == "west") {
        return true;
      }

      if (s == "11" && v == "east") {
        return true;
      }

      if (s == "12" && v == "y") {
        return true;
      }

      if (s == "14" && v == "west") {
        return true;
      }
      return false;
    }

    if (exp == "_x_") {
      if (s == "6" && v == "west") {
        return true;
      }

      if (s == "10" && v == "x") {
        return true;
      }

      if (s == "11" && v == "east") {
        return true;
      }

      if (s == "14" && v == "west") {
        return true;
      }
      return false;
    }
    return true;
  }

  std::string getVarAt(const Statement &s) override {
    if (stmtVarMap.find(s) == stmtVarMap.end()) {
      return "";
    }

    return stmtVarMap.find(s)->second;
  }

  std::unordered_set<Entity> getUsesS(const Entity &s,
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

  std::unordered_set<Statement>
  getCallsByCalledProcName(const Procedure &p) override {
    std::unordered_set<Statement> result;
    for (auto &[call, proc] : callProcMap) {
      if (proc == p) {
        result.insert(call);
      }
    }
    return result;
  }

  std::unordered_set<Statement> getReadsByVarName(const Variable &v) override {
    std::unordered_set<Statement> result;
    for (auto &[read, var] : readVarMap) {
      if (var == v) {
        result.insert(read);
      }
    }
    return result;
  }

  std::unordered_set<Statement> getPrintsByVarName(const Variable &v) override {
    std::unordered_set<Statement> result;
    for (auto &[print, var] : printVarMap) {
      if (var == v) {
        result.insert(print);
      }
    }
    return result;
  }
};

#endif // SPA_STUBPKB3_H
