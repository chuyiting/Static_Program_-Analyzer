#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "QPS/QPS.h"

TEST_CASE("TestMultipleProcedures") {
  /*
  Populate pkb with the source file -
  procedure foo {
          read a; //1
          call bar; //2
  }

  procedure bar {
          call foobar; //3
  }

  procedure foobar {
          print b; //4
  }
  */
  PKB pkb = PKB();
  PKBSetter *setter = new PKBSetter(pkb);
  auto getter = std::make_shared<PKBGetter>(pkb);
  QPS qps = QPS(getter);
  std::unordered_set<std::string> stmts({"1", "2", "3", "4"});
  std::vector<std::string> stmts_vec({"1", "2", "3", "4"});
  std::unordered_set<std::string> vars({"a", "b"});

  setter->addProcedure("foo", std::unordered_set<std::string>{"1", "2"});
  setter->addProcedure("bar", std::unordered_set<std::string>{"3"});
  setter->addProcedure("foobar", std::unordered_set<std::string>{"4"});

  setter->addVariable("a");
  setter->addVariable("b");

  setter->addReadStmt("1", "a");
  setter->addCallStmt("2", "bar");
  setter->addCallStmt("3", "foobar");
  setter->addPrintStmt("4", "b");

  setter->addModifiesP("foo", std::unordered_set<std::string>{"a"});

  setter->addUsesP("foo", std::unordered_set<std::string>{"b"});
  setter->addUsesP("bar", std::unordered_set<std::string>{"b"});
  setter->addUsesP("foobar", std::unordered_set<std::string>{"b"});

  setter->addModifiesS("1", "a");

  setter->addUsesS("2", std::unordered_set<std::string>{"b"});
  setter->addUsesS("3", std::unordered_set<std::string>{"b"});
  setter->addUsesS("4", std::unordered_set<std::string>{"b"});

  setter->addCallRelation("foo", "bar");
  setter->addCallRelation("bar", "foobar");
  setter->done();

  SECTION("Test Select procedures") {
    std::string testQuery = "procedure p;variable v;\nSelect p";
    std::unordered_set<std::string> expectedResult{"foo", "bar", "foobar"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Test Modifies(procedure_syn, var_syn)") {
    std::string testQuery =
        "procedure p;variable v;\nSelect p such that Modifies(p,v)";
    std::unordered_set<std::string> expectedResult{"foo"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Test Uses(procedure_syn, var_syn)") {
    std::string testQuery =
        "procedure p;variable v;\nSelect p such that Uses(p,v)";
    std::unordered_set<std::string> expectedResult{"foo", "bar", "foobar"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Test Calls(procedure_syn, procedure_literal)") {
    std::string testQuery =
        "procedure p;\nSelect p such that Calls(p,\"foobar\")";
    std::unordered_set<std::string> expectedResult{"bar"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }

  SECTION("Test Calls*(procedure_syn, procedure_literal)") {
    std::string testQuery =
        "procedure p;\nSelect p such that Calls*(p,\"foobar\")";
    std::unordered_set<std::string> expectedResult{"foo", "bar"};

    REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
  }
}