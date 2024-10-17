#include "PKB/type/pkb_types.h"
#include "QPS/evaluator/QueryEvaluator.h"
#include "QPS/evaluator/relationships/concreteRelationships/Follows.h"
#include "QPS/evaluator/relationships/concreteRelationships/FollowsT.h"
#include "QPS/evaluator/relationships/concreteRelationships/ModifiesS.h"
#include "QPS/evaluator/relationships/concreteRelationships/UsesS.h"
#include "QPS/parser/querystructs/suchthatclause/SuchThatClause.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "StubPKB.h"
#include "StubPKB2.h"
#include "TestQueryDataBuilder.h"
#include "catch.hpp"
#include <iostream>
#include <unordered_map>

TEST_CASE("QueryEvaluator Positive cases") {
  TestQPSDataBuilder testQueryData;

  SECTION("variable v; Select v") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    auto syn = std::make_shared<Synonym>(EntityType::VARIABLE, "v");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);
    auto testData = testQueryData.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Variable> expectedVector{"a", "b"};
    QueryResult expectedResult{expectedVector, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }

  SECTION("stmt s; Select s") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};
    auto syn = std::make_shared<Synonym>(EntityType::STMT, "s");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);

    auto testData = testQueryData.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Statement> expectedVector{"1", "2", "3", "4"};
    QueryResult expectedResult{expectedVector, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }

  SECTION("constant c; Select c") {
    StubPKB pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB>(pkb)};

    auto syn = std::make_shared<Synonym>(EntityType::CONSTANT, "c");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);

    auto testData = testQueryData.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Constant> expectedVector{"100", "50"};
    QueryResult expectedResult{expectedVector, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }

  SECTION("assign a; Select a") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    auto syn = std::make_shared<Synonym>(EntityType::ASSIGN, "a");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);

    auto testData = testQueryData.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Statement> expectedVector{"1", "2"};
    QueryResult expectedResult{expectedVector, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("QueryEvaluator - One Pattern Clause") {
  TestQPSDataBuilder testQuery;
  SECTION("assign a; Select a pattern a(\"a\",_\"1\"_) ") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    auto syn = std::make_shared<Synonym>(EntityType::ASSIGN, "a");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);
    EntRef eref = EntRef("a");
    Synonym synAsign = Synonym(EntityType::ASSIGN, "a");
    ExpressionSpec es = ExpressionSpec(true, "1");
    PatternClause pattern = PatternClause(synAsign, eref, es);

    auto testData = testQuery.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->withPattern(pattern)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    QueryResult expectedResult = QueryResult({"1"}, QueryResultType::SINGLE);

    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("QueryEvaluator - Modifies") {
  TestQPSDataBuilder testQueryData;

  SECTION("variable v; Select v such that Modifies(\"1\",v)") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    auto syn = std::make_shared<Synonym>(EntityType::VARIABLE, "v");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);
    StmtRef sref{1};
    EntRef eref{syn};
    auto modifyRel = std::make_shared<ModifiesS>(sref, eref);

    auto testData = testQueryData.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->withRelationship(modifyRel)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    QueryResult expectedResult = QueryResult({"a"}, QueryResultType::SINGLE);

    REQUIRE(result == expectedResult);
  }

  SECTION("stmt s; Select s such that Modifies(s,_)") {
    StubPKB pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB>(pkb)};

    std::unordered_map<synonym_t, Synonym> synonymDecls = {
        {"s", Synonym(EntityType::STMT, "s")}};
    SelectClause selectClause = SelectClause("s", EntityType::STMT);
    SuchThatClause suchThatClause = SuchThatClause();
    StmtRef lhs{1, Synonym(EntityType::STMT, "s"),
                StmtRef::StmtRefType::SYNONYM};
    EntRef rhs{"_", Synonym(), EntRef::EntRefType::WILDCARD};
    auto relationship = std::make_shared<ModifiesS>(lhs, rhs);

    auto testData = testQueryData.withSynonymDecls(synonymDecls)
                        ->withSelectClause(selectClause)
                        ->withRelationship(relationship)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Variable> expected{"1", "2"};
    QueryResult expectedResult = QueryResult(expected, QueryResultType::SINGLE);

    REQUIRE(result == expectedResult);
  }

  SECTION("variable v;stmt s; Select v such that Modifies(s,v)") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    std::unordered_map<synonym_t, Synonym> synonymDecls = {
        {"s", Synonym(EntityType::STMT, "s")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};

    SelectClause selectClause;
    SelectTarget target =
        SelectTarget(std::make_shared<Synonym>(EntityType::VARIABLE, "v"));
    selectClause.addSelectTarget(target);
    StmtRef lhs{1, Synonym(EntityType::STMT, "s"),
                StmtRef::StmtRefType::SYNONYM};
    EntRef rhs{"a", Synonym(EntityType::VARIABLE, "v"),
               EntRef::EntRefType::SYNONYM};
    auto modifyRel = std::make_shared<ModifiesS>(lhs, rhs);

    auto testData = testQueryData.withSynonymDecls(synonymDecls)
                        ->withSelectClause(selectClause)
                        ->withRelationship(modifyRel)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Variable> expected{"a", "b"};
    QueryResult expectedResult{expected, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("QueryEvaluator - Uses", "[required]") {
  TestQPSDataBuilder testQueryData;

  SECTION("variable v; Select v such that Uses(\"2\",v)") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    auto syn = std::make_shared<Synonym>(EntityType::VARIABLE, "v");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);
    StmtRef sref{2};
    EntRef eref{syn};
    UsesS usesRel{sref, eref};

    auto testData = testQueryData.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->withRelationship(std::make_unique<UsesS>(usesRel))
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Variable> expected{"a"};
    QueryResult expectedResult{expected, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }

  SECTION(
      "variable v; Select v such that Uses(\"2\",v) such that Uses(\"4\",v)") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    auto syn = std::make_shared<Synonym>(EntityType::VARIABLE, "v");
    SelectClause selectClause;
    SelectTarget target = SelectTarget(syn);
    selectClause.addSelectTarget(target);
    StmtRef sref{2};
    EntRef eref{syn};
    UsesS usesRel{sref, eref};
    StmtRef sref2{4};
    UsesS usesRel2{sref2, eref};

    auto testData = testQueryData.withSynonymDecl(*syn)
                        ->withSelectClause(selectClause)
                        ->withRelationship(std::make_unique<UsesS>(usesRel))
                        ->withRelationship(std::make_unique<UsesS>(usesRel2))
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Variable> expected{};
    QueryResult expectedResult{expected, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }

  SECTION("Select s such that Uses(s,_)") {
    StubPKB pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB>(pkb)};

    std::unordered_map<synonym_t, Synonym> synonymDecls = {
        {"s", Synonym(EntityType::STMT, "s")}};
    SelectClause selectClause = SelectClause("s", EntityType::STMT);
    SuchThatClause suchThatClause = SuchThatClause();
    StmtRef lhs{1, Synonym(EntityType::STMT, "s"),
                StmtRef::StmtRefType::SYNONYM};
    EntRef rhs{"_", Synonym(), EntRef::EntRefType::WILDCARD};
    auto relationship = std::make_shared<UsesS>(lhs, rhs);

    auto testData = testQueryData.withSynonymDecls(synonymDecls)
                        ->withSelectClause(selectClause)
                        ->withRelationship(relationship)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Variable> expected{"1", "2"};
    QueryResult expectedResult = QueryResult(expected, QueryResultType::SINGLE);

    REQUIRE(result == expectedResult);
  }

  SECTION("Select s such that Uses(s,'a') ") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    std::unordered_map<synonym_t, Synonym> synonymDecls = {
        {"s", Synonym(EntityType::STMT, "s")}};
    SelectClause selectClause = SelectClause("s", EntityType::STMT);
    SuchThatClause suchThatClause = SuchThatClause();
    StmtRef lhs{1, Synonym(EntityType::STMT, "s"),
                StmtRef::StmtRefType::SYNONYM};
    EntRef rhs{"a", Synonym(), EntRef::EntRefType::STRING};
    auto relationship = std::make_shared<UsesS>(lhs, rhs);

    auto testData = testQueryData.withSynonymDecls(synonymDecls)
                        ->withSelectClause(selectClause)
                        ->withRelationship(relationship)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    // nothing
    std::unordered_set<Variable> expected{"2", "3"};
    QueryResult expectedResult = QueryResult(expected, QueryResultType::SINGLE);

    REQUIRE(result == expectedResult);
  }

  SECTION("variable v;stmt s; Select v such that Uses(s,v)") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    std::unordered_map<synonym_t, Synonym> synonymDecls = {
        {"s", Synonym(EntityType::STMT, "s")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause;
    SelectTarget target =
        SelectTarget(std::make_shared<Synonym>(EntityType::VARIABLE, "v"));
    selectClause.addSelectTarget(target);
    StmtRef lhs{1, Synonym(EntityType::STMT, "s"),
                StmtRef::StmtRefType::SYNONYM};
    EntRef rhs{"a", Synonym(EntityType::VARIABLE, "v"),
               EntRef::EntRefType::SYNONYM};
    auto usesRel = std::make_shared<UsesS>(lhs, rhs);

    auto testData = testQueryData.withSynonymDecls(synonymDecls)
                        ->withSelectClause(selectClause)
                        ->withRelationship(usesRel)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Variable> expected{"a"};
    QueryResult expectedResult{expected, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("QueryEvaluator - Follows") {
  TestQPSDataBuilder testQueryData;

  SECTION("stmt s; Select s such that Follows(\"1\",s)") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    std::unordered_map<synonym_t, Synonym> synonymDecls = {
        {"s", Synonym(EntityType::STMT, "s")},
    };
    SelectClause selectClause;
    SelectTarget target =
        SelectTarget(std::make_shared<Synonym>(EntityType::STMT, "s"));
    selectClause.addSelectTarget(target);
    StmtRef lhs{1, Synonym(), StmtRef::StmtRefType::INTEGER};
    StmtRef rhs{1, Synonym(EntityType::STMT, "s"),
                StmtRef::StmtRefType::SYNONYM};
    auto follows = std::make_shared<Follows>(lhs, rhs);

    auto testData = testQueryData.withSynonymDecls(synonymDecls)
                        ->withSelectClause(selectClause)
                        ->withRelationship(follows)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Statement> expected{"2"};
    QueryResult expectedResult{expected, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("QueryEvaluator - FollowsT") {
  TestQPSDataBuilder testQueryData;

  SECTION("stmt s; Select s such that FollowsT(\"1\",s)") {
    StubPKB2 pkb;
    QueryEvaluator evaluator{std::make_shared<StubPKB2>(pkb)};

    std::unordered_map<synonym_t, Synonym> synonymDecls = {
        {"s", Synonym(EntityType::STMT, "s")},
    };
    SelectClause selectClause;
    SelectTarget target =
        SelectTarget(std::make_shared<Synonym>(EntityType::STMT, "s"));
    selectClause.addSelectTarget(target);
    StmtRef lhs{1, Synonym(), StmtRef::StmtRefType::INTEGER};
    StmtRef rhs{1, Synonym(EntityType::STMT, "s"),
                StmtRef::StmtRefType::SYNONYM};
    auto followst = std::make_shared<FollowsT>(lhs, rhs);

    auto testData = testQueryData.withSynonymDecls(synonymDecls)
                        ->withSelectClause(selectClause)
                        ->withRelationship(followst)
                        ->createQPSQuery();

    QueryResult result = *evaluator.evaluate(testData);
    std::unordered_set<Statement> expected{"2", "3", "4"};
    QueryResult expectedResult{expected, QueryResultType::SINGLE};

    REQUIRE(result == expectedResult);
  }
}
