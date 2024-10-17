#include "QPS/QPS.h"
#include "QPS/evaluator/QueryResult.h"
#include "StubPKB2.h"
#include "StubPKB3.h"
#include "catch.hpp"

TEST_CASE("QPS - Successes No Such That Clauses") {
  StubPKB2 pkb;
  SECTION("variable v;Select v") {
    std::string testQuery = "variable v;\nSelect v";
    std::unordered_set<Variable> expectedVector = pkb.getVariables();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
  SECTION("constant c;Select c") {
    std::string testQuery = "constant c;\nSelect c";
    std::unordered_set<Constant> expectedVector = pkb.getConstants();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a;Select a") {
    std::string testQuery = "assign a;Select a";
    std::unordered_set<Statement> expectedVector = pkb.getAssignStmts();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("stmt s;Select s") {
    std::string testQuery = "stmt s;Select s";
    std::unordered_set<Statement> expectedVector = pkb.getStatements();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("print p; Select p") {
    std::string testQuery = "print p; Select p";
    std::unordered_set<Statement> expectedVector = pkb.getPrintStmts();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("read r; Select r") {
    std::string testQuery = "read r; Select r";
    std::unordered_set<Statement> expectedVector = pkb.getReadStmts();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("procedure p; Select p") {
    std::string testQuery = "procedure p; Select p";
    std::unordered_set<Statement> expectedVector = pkb.getProcedures();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("if p; Select p") {
    std::string testQuery = "if p; Select p";
    std::unordered_set<Statement> expectedVector = pkb.getIfStmts();
    QueryResult expectedResult =
        QueryResult{expectedVector, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("QPS - Successes One Such That Clause") {
  StubPKB2 pkb;
  QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

  SECTION("variable v; Select v such that Uses(1,v)") {
    std::string testQuery = "variable v;\nSelect v such that Uses(1,v)";
    std::unordered_set<Variable> expected{};
    QueryResult expectedResult = QueryResult{expected, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
  SECTION("stmt s; Select s such that Uses(s,\"b\")") {
    std::string testQuery = "stmt s; Select s such that Uses(s,\"b\")";
    QueryResult expectedResult = QueryResult{{}, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("variable v; Select v such that Modifies(1,v)") {
    std::string testQuery = "variable v;\nSelect v such that Modifies(1,v)";
    std::unordered_set<Variable> expected{"a"};
    QueryResult expectedResult = QueryResult{expected, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
  SECTION("stmt s; Select s such that Follows(1,s)") {
    std::string testQuery = "stmt s;\nSelect s such that Follows(1,s)";
    std::unordered_set<Variable> expected{"2"};
    QueryResult expectedResult = QueryResult{expected, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("QPS - Successes 2 Such That Clause") {
  StubPKB2 pkb;
  QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

  SECTION("stmt s; Select s such that Uses(s,\"b\") such that Follows(1,s)") {
    std::string testQuery =
        "stmt s; Select s such that Uses(s,\"b\") such that Follows(1,s)";
    QueryResult expectedResult = QueryResult{{}, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("stmt s; Select s such that Uses(s,\"b\") and Follows(1,s)") {
    std::string testQuery =
        "stmt s; Select s such that Uses(s,\"b\") and Follows(1,s)";
    QueryResult expectedResult = QueryResult{{}, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("QPS - Successes One Pattern Clause") {
  StubPKB2 pkb;

  SECTION("assign a; Select a pattern a(\"a\",_\"1\"_)") {
    std::string testQuery = "assign a; Select a pattern a(\"a\",_\"1\"_)";
    std::unordered_set<Variable> expected{"1"};
    QueryResult expectedResult = QueryResult{expected, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("QPS - Multiple Clauses - Such That and Pattern") {
  StubPKB2 pkb;

  SECTION("unrelated such that and pattern clause") {
    std::string testQuery = "assign a, a2; variable v; Select a such that "
                            "Uses(a2, v) pattern a(\"a\",_\"1\"_)";
    std::unordered_set<Variable> expected{"1"};
    QueryResult expectedResult = QueryResult{expected, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("related Uses and pattern clause") {
    std::string testQuery = "assign a, a2; variable v; Select v such that "
                            "Uses(a, v) pattern a(\"b\",_)";
    std::unordered_set<Variable> expected{"a"};
    QueryResult expectedResult = QueryResult{expected, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("QPS - Error") {
  StubPKB2 pkb;
  // Example results: use for unit testing
  QueryResult SemanticError =
      QueryResult({"SemanticError"}, QueryResultType::SINGLE);
  QueryResult SyntacticError =
      QueryResult({"SyntaxError"}, QueryResultType::SINGLE);

  SECTION("no select - syntax error") {
    std::string testQuery = "assign a;";
    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == SyntacticError);
  }

  SECTION("select undefined synonym - syntax error") {
    std::string testQuery = "assign a; Select b";
    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == SemanticError);
  }

  SECTION("undefined synonym type - syntax error") {
    std::string testQuery = "loop l; Select l";
    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == SyntacticError);
  }

  SECTION("unmatched parenthesis - syntax error") {
    std::string testQuery = "assign a; Select a such that Uses(a, _";
    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == SyntacticError);
  }

  SECTION("invalid synonym name - semantic error") {
    std::string testQuery = "assign 1abc; Select 1abc";
    QPS qps = QPS(std::make_shared<StubPKB2>(pkb));
    REQUIRE(*qps.run(testQuery) == SemanticError);
  }
}

TEST_CASE("QPS - Bigger SIMPLE program") {
  StubPKB3 pkb;
  // Example results: use for unit testing
  QueryResult SemanticError =
      QueryResult({"SemanticError"}, QueryResultType::SINGLE);
  QueryResult SyntacticError =
      QueryResult({"SyntaxError"}, QueryResultType::SINGLE);

  SECTION("constant c; Select c") {
    std::string testQuery = "constant c; Select c";
    QueryResult expectedResult =
        QueryResult{StubPKB3::CONSTANTS, QueryResultType::SINGLE};

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("stmt s; Select s such that Follows(9, s)") {
    std::string testQuery = "stmt s; Select s such that Follows(9, s)";
    QueryResult expectedResult = QueryResult();

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("stmt s; Select s such that Follows*(s, 13)") {
    std::string testQuery = "stmt s; Select s such that Follows*(s, 13)";
    QueryResult expectedResult = QueryResult({"12"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("stmt s; Select s such that Parent(s, 12)") {
    std::string testQuery = "stmt s; Select s such that Parent(s, 12)";
    QueryResult expectedResult = QueryResult({"9"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a; Select a such that Parent*(_, a)") {
    std::string testQuery = "assign a; Select a such that Parent*(_, a)";
    QueryResult expectedResult = QueryResult(
        {"8", "10", "11", "12", "17", "18"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("variable v; Select v such that Modifies(8, v)") {
    std::string testQuery = "variable v; Select v such that Modifies(8, v)";
    QueryResult expectedResult = QueryResult({"x"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    auto result = qps.run(testQuery);
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("variable v; Select v such that Uses(3, v)") {
    std::string testQuery = "variable v; Select v such that Uses(3, v)";
    QueryResult expectedResult = QueryResult({"east"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a; Select a pattern a(_, _\"east+west\"_)") {
    std::string testQuery = "assign a; Select a pattern a(_, _\"east+west\"_)";
    QueryResult expectedResult = QueryResult();

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a; Select a pattern a(_, _\"x*y*z\"_)") {
    std::string testQuery = "assign a; Select a pattern a(_, _\"x*y*z\"_)";
    QueryResult expectedResult =
        QueryResult({"6", "11", "14"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a; variable v; Select v pattern a(v, _\"y\"_)") {
    std::string testQuery =
        "assign a; variable v; Select v pattern a(v, _\"y\"_)";
    QueryResult expectedResult =
        QueryResult({"east", "west", "y"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("read re; Select re such that Modifies(12, \"x\")") {
    std::string testQuery = "read re; Select re such that Modifies(12, \"x\")";
    QueryResult expectedResult = QueryResult();

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a; variable v; Select a such that Uses(a, v) pattern a(v, "
          "_\"x\"_)") {
    std::string testQuery = "assign a; variable v; Select a such that Uses(a, "
                            "v) pattern a(v, _\"x\"_)";
    QueryResult expectedResult =
        QueryResult({"6", "10", "14"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a; variable v, v1; Select a such that Uses(a, v) pattern "
          "a(v1, _\"x\"_)") {
    std::string testQuery = "assign a; variable v, v1; Select a such that "
                            "Uses(a, v) pattern a(v1, _\"x\"_)";
    QueryResult expectedResult =
        QueryResult({"6", "10", "11", "14"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a, a1; variable v, v1; Select v such that Uses(a, v) pattern "
          "a1(v, _\"x\"_)") {
    std::string testQuery = "assign a, a1; variable v, v1; Select v such that "
                            "Uses(a, v) pattern a1(v, _\"x\"_)";
    QueryResult expectedResult =
        QueryResult({"east", "west", "x"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("assign a, a1; variable v, v1; Select v such that Uses(a, v) pattern "
          "a1(v1, _\"x\"_)") {
    std::string testQuery = "assign a, a1; variable v, v1; Select v such that "
                            "Uses(a, v) pattern a1(v1, _\"x\"_)";
    QueryResult expectedResult =
        QueryResult({"east", "west", "x", "y", "z"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("Advanced QPS - With Clause") {
  StubPKB3 pkb;
  // Example results: use for unit testing
  QueryResult SemanticError =
      QueryResult({"SemanticError"}, QueryResultType::SINGLE);
  QueryResult SyntacticError =
      QueryResult({"SyntaxError"}, QueryResultType::SINGLE);

  SECTION("without Synonym - True result") {
    std::string testQuery =
        R"(assign a; variable v; Select <a, v> with "x"=    "x" and 1=1)";

    QueryResult expectedResult = QueryResult();
    for (auto &assign : pkb.getAssignStmts()) {
      for (auto &var : pkb.getVariables()) {
        expectedResult.addResult(assign + " " + var);
      }
    }

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("without Synonym - False result") {
    std::string testQuery =
        R"(assign a; variable v; Select <a, v> with "x"=    "y" and 1=1)";
    QueryResult expectedResult = QueryResult();

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("without Synonym - False result - more complicated case") {
    std::string testQuery =
        R"(assign a; variable v; Select <a, v> with "x"=    "x" and 1=1 and "abc" ="abc" and 2 =3)";
    QueryResult expectedResult = QueryResult();

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("Synonym, literal - Select synonym ") {
    std::string testQuery =
        R"(call c; Select c with c.procName = "Yishun" and 1=1)";

    QueryResult expectedResult =
        QueryResult({"5", "13"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("Synonym, literal - Select attribute ") {
    std::string testQuery =
        R"(call c; Select c.procName with c.stmt# = 5 and 1=1)";

    QueryResult expectedResult =
        QueryResult({"Yishun"}, QueryResultType::SINGLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("Advanced QPS - Tuple And Multiple Clauses") {
  StubPKB3 pkb;
  // Example results: use for unit testing
  QueryResult SemanticError =
      QueryResult({"SemanticError"}, QueryResultType::TUPLE);
  QueryResult SyntacticError =
      QueryResult({"SyntaxError"}, QueryResultType::TUPLE);

  SECTION("assign a; variable v; Select <a, v> ") {
    std::string testQuery = "assign a; variable v; Select <a, v>";

    QueryResult expectedResult;
    for (auto &assign : pkb.getAssignStmts()) {
      for (auto &var : pkb.getVariables()) {
        expectedResult.addResult(assign + " " + var);
      }
    }

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("unrelated synonyms, simple cross product result") {
    std::string testQuery = "assign a, a1, a2; Select <a,   a1> such that "
                            "Parent*(_, a) pattern a1(_, _\"x*y*z\"_)";
    std::vector<std::string> aRes = {"8", "10", "11", "12", "17", "18"};
    std::vector<std::string> a1Res = {"6", "11", "14"};

    QueryResult expectedResult = QueryResult(QueryResultType::TUPLE);
    for (auto &a : aRes) {
      for (auto &a1 : a1Res) {
        expectedResult.addResult(a + " " + a1);
      }
    }

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("related by one clause") {
    std::string testQuery =
        "assign a; variable v; Select <a, v> pattern a(v, _\"y\"_)";
    QueryResult expectedResult = QueryResult(
        {"11 east", "6 west", "14 west", "12 y"}, QueryResultType::TUPLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("related by two clauses (intermediate clause involved)") {
    std::string testQuery = "assign a, a1; variable v; Select <a1, v> pattern "
                            "a(v, _\"y\"_) such that Follows*(a, a1)";
    QueryResult expectedResult =
        QueryResult({"14 west"}, QueryResultType::TUPLE);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

TEST_CASE("Test BOOLEAN") {
  StubPKB3 pkb;
  // Example results: use for unit testing
  QueryResult SemanticError =
      QueryResult({"SemanticError"}, QueryResultType::BOOLEAN);
  QueryResult SyntacticError =
      QueryResult({"SyntaxError"}, QueryResultType::BOOLEAN);

  SECTION("No Constraints") {
    std::string testQuery = "Select BOOLEAN";
    QueryResult expectedResult =
        QueryResult({"TRUE"}, QueryResultType::BOOLEAN);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("Constraints can be Fulfilled") {
    std::string testQuery =
        "assign a; variable v; Select BOOLEAN pattern a(v, _\"y\"_)";
    QueryResult expectedResult =
        QueryResult({"TRUE"}, QueryResultType::BOOLEAN);

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }

  SECTION("Constraints can not be Fulfilled") {
    std::string testQuery =
        "assign a; variable v; Select BOOLEAN such that Uses(a, \"notExist\")";
    QueryResult expectedResult = QueryResult();
    expectedResult.addResult("FALSE");

    QPS qps = QPS(std::make_shared<StubPKB3>(pkb));
    REQUIRE(*qps.run(testQuery) == expectedResult);
  }
}

