#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "QPS/QPS.h"

TEST_CASE("TestNoNested") {
  /*
  Populate pkb with the source file -
  procedure foo {
                  a = 1; // 1
                  b = 2; // 2
                  c = 3+b; // 3
                  print d; //4
                  read e; //5
  }
  */
  PKB pkb = PKB();
  PKBSetter *setter = new PKBSetter(pkb);
  auto getter = std::make_shared<PKBGetter>(pkb);
  QPS qps = QPS(getter);
  std::unordered_set<std::string> stmts({"1", "2", "3", "4", "5"});
  std::vector<std::string> stmts_vec({"1", "2", "3", "4", "5"});
  std::unordered_set<std::string> constants({"1", "2", "3"});
  std::unordered_set<std::string> vars({"a", "b", "c", "d", "e"});
  setter->addProcedure("foo", stmts);

  setter->addVariable("a");
  setter->addVariable("b");
  setter->addVariable("c");
  setter->addVariable("d");
  setter->addVariable("e");
  setter->addConstant("1");
  setter->addConstant("2");
  setter->addConstant("3");

  setter->addAssignmentStmt("1", "a");
  setter->addAssignmentStmt("2", "b");
  setter->addAssignmentStmt("3", "c");
  setter->addPrintStmt("4", "d");
  setter->addReadStmt("5", "e");

  setter->addModifiesP("foo",
                       std::unordered_set<std::string>{"a", "b", "c", "e"});
  setter->addUsesP("foo", std::unordered_set<std::string>{"b", "d"});

  setter->addModifiesS("1", "a");
  setter->addModifiesS("2", "b");
  setter->addModifiesS("3", "c");
  setter->addModifiesS("5", "e");

  setter->addUsesS("3", std::unordered_set<std::string>{"b"});
  setter->addUsesS("4", std::unordered_set<std::string>{"d"});

  setter->addAssignmentExpr("1", {"1"});
  setter->addAssignmentExpr("2", {"2"});
  setter->addAssignmentExpr("3", {"3", "+", "b"});

  setter->addNextRelation("1","2");
  setter->addNextRelation("2", "3");
  setter->addNextRelation("3", "4");
  setter->addNextRelation("4", "5");
  setter->addNextRelation("5", "0");


  setter->addFollowsT(stmts_vec);
  SECTION("Testing Select Assign") {
    std::string testQuery = "assign a;\nSelect a";
    std::unordered_set<std::string> expectedResult{"1", "2", "3"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Select Read") {
    std::string testQuery = "read r;\nSelect r";
    std::unordered_set<std::string> expectedResult{"5"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Select Print") {
    std::string testQuery = "print pr;\nSelect pr";
    std::unordered_set<std::string> expectedResult{"4"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Select Stmt") {
    std::string testQuery = "stmt a;\nSelect a";
    std::unordered_set<std::string> expectedResult{"1", "2", "3", "4", "5"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Select variable") {
    std::string testQuery = "variable v;\nSelect v";
    std::unordered_set<std::string> expectedResult{"a", "b", "c", "d", "e"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Select constant") {
    std::string testQuery = "constant c;\nSelect c";
    std::unordered_set<std::string> expectedResult{"1", "2", "3"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Modifies(stmt_syn,var_syn)") {
    std::string testQuery =
        "stmt s;variable v;\nSelect v such that Modifies(s,v)";
    std::unordered_set<std::string> expectedResult{"a", "b", "c", "e"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Modifies(stmt_syn,var_literal)") {
    std::string testQuery = "stmt s;\nSelect s such that Modifies(s,\"a\")";
    std::unordered_set<std::string> expectedResult{"1"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Modifies(proc_syn,var_literal)") {
    std::string testQuery =
        "procedure p;\nSelect p such that Modifies(p,\"b\")";
    std::unordered_set<std::string> expectedResult{"foo"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Uses(stmt_syn,var_literal)") {
    std::string testQuery = "stmt s;\nSelect s such that Uses(s,\"b\")";
    std::unordered_set<std::string> expectedResult{"3"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Uses(proc_syn,var_literal)") {
    std::string testQuery = "procedure p;\nSelect p such that Uses(p,\"b\")";
    std::unordered_set<std::string> expectedResult{"foo"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Uses(stmt_literal,var_syn)") {
    std::string testQuery = "variable v;\nSelect v such that Uses(1,v)";
    std::unordered_set<std::string> expectedResult{};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Follows(stmt_syn,stmt_lit)") {
    std::string testQuery = "stmt s;\nSelect s such that Follows(s,2)";
    std::unordered_set<std::string> expectedResult{"1"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing Follows*(stmt_syn,stmt_lit)") {
    std::string testQuery = "stmt s;\nSelect s such that Follows*(3,s)";
    std::unordered_set<std::string> expectedResult{"4", "5"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Testing partial match pattern") {
     std::string testQuery = "assign a;\nSelect a pattern a(_,_\"b\"_)";
     std::unordered_set<std::string> expectedResult{"3"};

     REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

    SECTION("Testing Affects(stmt_lit,stmt_syn)") {
        std::string testQuery = "stmt s;\nSelect s such that Affects(2,s)";
        std::unordered_set<std::string> expectedResult{"3"};

        REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
    }

    SECTION("Testing Affects*(stmt_lit,stmt_syn)") {
        std::string testQuery = "stmt s;\nSelect s such that Affects*(2,s)";
        std::unordered_set<std::string> expectedResult{"3"};

        REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
    }

}