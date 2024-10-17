#include "PKB/Datastore/PatternStorage.h"
#include "catch.hpp"
#include "utils/ExpressionHandler/ExpressionHandler.h"
#include <unordered_map>
#include <unordered_set>

TEST_CASE("addAssignmentExpr") {
  PatternStorage p = PatternStorage();
  ExpressionHandler e = ExpressionHandler(ExpressionType::EXPRESSION, "s");
  e.parse();
  std::vector<std::string> e1 = e.getInfixNotation();
  p.addAssignmentExpr("3", e1);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "s + 3");
  e.parse();
  std::vector<std::string> e2 = e.getInfixNotation();
  p.addAssignmentExpr("1", e2);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "c + 2 - 3");
  e.parse();
  std::vector<std::string> e3 = e.getInfixNotation();
  p.addAssignmentExpr("2", e3);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "x + 3 * (2 - s)");
  e.parse();
  std::vector<std::string> e4 = e.getInfixNotation();
  p.addAssignmentExpr("5", e4);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "c");
  e.parse();
  std::vector<std::string> e5 = e.getInfixNotation();
  p.addAssignmentExpr("8", e5);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "c - 1");
  e.parse();
  std::vector<std::string> e6 = e.getInfixNotation();
  p.addAssignmentExpr("10", e6);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "s + e * c");
  e.parse();
  std::vector<std::string> e7 = e.getInfixNotation();
  p.addAssignmentExpr("12", e7);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "a + b + c");
  e.parse();
  std::vector<std::string> e8 = e.getInfixNotation();
  p.addAssignmentExpr("60", e8);

  e = ExpressionHandler(ExpressionType::EXPRESSION, "a * b - (c)");
  e.parse();
  std::vector<std::string> e9 = e.getInfixNotation();
  p.addAssignmentExpr("100", e9);

  std::unordered_map<Statement, std::vector<std::string>> assignmentExpr =
      p.getAssignmentExpr();

  SECTION("addAssignmentExpr_positive") {
    std::unordered_map<Statement, std::vector<std::string>>
        expectedAssignmentExpr = {{"3", e1},  {"1", e2},  {"2", e3},
                                  {"5", e4},  {"8", e5},  {"10", e6},
                                  {"12", e7}, {"60", e8}, {"100", e9}};
    REQUIRE(assignmentExpr == expectedAssignmentExpr);
  }

  SECTION("addAssignmentExpr_negative") {
    std::unordered_map<Statement, std::vector<std::string>>
        expectedAssignmentExpr = {
            {"3", e1},
            {"1", e2},
            {"2", e3},
        };
    REQUIRE_FALSE(assignmentExpr == expectedAssignmentExpr);
  }
}

TEST_CASE("addModifiesS") {
  PatternStorage p = PatternStorage();
  p.addModifiesS("3", "s");
  p.addModifiesS("1", "a");
  p.addModifiesS("2", "v");
  p.addModifiesS("5", "z");
  p.addModifiesS("8", "s");
  p.addModifiesS("10", "e");
  p.addModifiesS("12", "c");
  p.addModifiesS("20", "j");
  p.addModifiesS("23", "c");
  p.addModifiesS("100", "s");

  std::unordered_map<Statement, Variable> modifiesS = p.getModifiesS();

  SECTION("addModifiesS_positive") {
    std::unordered_map<Statement, Variable> expectedModifiesS = {
        {"3", "s"},  {"1", "a"},  {"2", "v"},  {"5", "z"},  {"8", "s"},
        {"10", "e"}, {"12", "c"}, {"20", "j"}, {"23", "c"}, {"100", "s"}};
    REQUIRE(modifiesS == expectedModifiesS);
  }

  SECTION("addModifiesS_negative") {
    std::unordered_map<Statement, Variable> expectedModifiesS = {
        {"3", "s"}, {"1", "a"}, {"2", "v"}};
    REQUIRE_FALSE(modifiesS == expectedModifiesS);
  }
}

TEST_CASE("addUsesIf") {
  PatternStorage p = PatternStorage();
  p.addUsesIf("3", {"s"});
  p.addUsesIf("1", {"a", "b", "c"});
  p.addUsesIf("2", {"v"});
  p.addUsesIf("5", {"z"});
  p.addUsesIf("8", {"s", "a", "p"});
  p.addUsesIf("10", {"e"});
  p.addUsesIf("12", {"c"});
  p.addUsesIf("20", {"j", "k"});
  p.addUsesIf("23", {"c", "d", "e"});
  p.addUsesIf("100", {"s", "c"});

  std::unordered_map<Statement, std::unordered_set<Variable>> usesIf =
      p.getUsesIf();

  SECTION("addUsesIf_positive") {
    std::unordered_map<Statement, std::unordered_set<Variable>> expectedUsesIf =
        {{"3", {"s"}},       {"1", {"a", "b", "c"}}, {"2", {"v"}},
         {"5", {"z"}},       {"8", {"s", "a", "p"}}, {"10", {"e"}},
         {"12", {"c"}},      {"20", {"j", "k"}},     {"23", {"c", "d", "e"}},
         {"100", {"s", "c"}}};
    REQUIRE(usesIf == expectedUsesIf);
  }

  SECTION("addUsesIf_negative") {
    std::unordered_map<Statement, std::unordered_set<Variable>> expectedUsesIf =
        {
            {"3", {"s"}},
            {"1", {"a", "b", "c"}},
            {"2", {"v"}},
        };
    REQUIRE_FALSE(usesIf == expectedUsesIf);
  }
}

TEST_CASE("addUsesWhile") {
  PatternStorage p = PatternStorage();
  p.addUsesWhile("3", {"s"});
  p.addUsesWhile("1", {"a", "b", "c"});
  p.addUsesWhile("2", {"v"});
  p.addUsesWhile("5", {"z"});
  p.addUsesWhile("8", {"s", "a", "p"});
  p.addUsesWhile("10", {"e"});
  p.addUsesWhile("12", {"c"});
  p.addUsesWhile("20", {"j", "k"});
  p.addUsesWhile("23", {"c", "d", "e"});
  p.addUsesWhile("100", {"s", "c"});

  std::unordered_map<Statement, std::unordered_set<Variable>> usesWhile =
      p.getUsesWhile();

  SECTION("addUsesWhile_positive") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedUsesWhile = {
            {"3", {"s"}},       {"1", {"a", "b", "c"}}, {"2", {"v"}},
            {"5", {"z"}},       {"8", {"s", "a", "p"}}, {"10", {"e"}},
            {"12", {"c"}},      {"20", {"j", "k"}},     {"23", {"c", "d", "e"}},
            {"100", {"s", "c"}}};
    REQUIRE(usesWhile == expectedUsesWhile);
  }

  SECTION("addUsesWhile_negative") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedUsesWhile = {
            {"3", {"s"}},
            {"1", {"a", "b", "c"}},
            {"2", {"v"}},
        };
    REQUIRE_FALSE(usesWhile == expectedUsesWhile);
  }
}
