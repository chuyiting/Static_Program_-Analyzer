//
// Created by Eddy Chu on 2022/10/2.
//

#include "QPS/evaluator/relationships/concreteRelationships/ModifiesS.h"
#include "QPS/evaluator/relationships/concreteRelationships/ParentT.h"
#include "QPS/evaluator/relationships/concreteRelationships/UsesS.h"
#include "QPS/exceptions/QPSException.h"
#include "QPS/parser/QPSParser.h"
#include "QPS/parser/QPSQuery.h"
#include "QPS/tokenizer/QueryToken.h"
#include "catch.hpp"
#include <string>
#include <unordered_map>
#include <vector>

#include "QPS/parser/querystructs/patternclause/ExpressionSpec.h"
#include "QPS/parser/querystructs/patternclause/PatternClause.h"

#include "QPS/evaluator/relationships/concreteRelationships/FollowsT.h"
#include "TestQPSParser.h"

TEST_CASE("Unit Test - Advanced QPS Parser - Test Select tuple - positive test "
          "cases") {
  SECTION("No attribute, one synonym") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"), PT(";"),
                                   ST("Select"),   PT("<"), ST("v"),
                                   PT(">")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    std::vector<std::shared_ptr<Synonym>> syns{
        std::make_shared<Synonym>(EntityType::VARIABLE, "v")};
    SelectClause selectClause;

    for (std::shared_ptr<Synonym> synonym : syns) {
      selectClause.addSelectTarget(SelectTarget(synonym));
    }

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("No attribute, multiple synonyms") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"), ST("call"), ST("c"), PT(";"),
        ST("Select"),   PT("<"), ST("v"), ST(","),    ST("c"), PT(">")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")},
        {"c", Synonym(EntityType::CALL, "c")},
    };
    std::vector<std::shared_ptr<Synonym>> syns{
        std::make_shared<Synonym>(EntityType::VARIABLE, "v"),
        std::make_shared<Synonym>(EntityType::CALL, "c")};
    SelectClause selectClause;

    for (std::shared_ptr<Synonym> synonym : syns) {
      selectClause.addSelectTarget(SelectTarget(synonym));
    }

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("All attribute, one synonym") {
    std::vector<QueryToken> tokens{ST("stmt"),   ST("s"),     PT(";"),
                                   ST("Select"), PT("<"),     ST("s"),
                                   DT("."),      AT("stmt#"), PT(">")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"s", Synonym(EntityType::STMT, "s")}};
    std::vector<SelectTarget> targets{
        SelectTarget(std::make_shared<Synonym>(EntityType::STMT, "s"),
                     AttributeType::STMTNUM)};
    SelectClause selectClause;

    for (auto target : targets) {
      selectClause.addSelectTarget(target);
    }

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("All attributes, multiple synonyms") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),        ST("call"),
        ST("c"),        PT(";"), ST("Select"),   PT("<"),
        ST("v"),        DT("."), AT("varName"),  ST(","),
        ST("c"),        DT("."), AT("procName"), PT(">")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")},
        {"c", Synonym(EntityType::CALL, "c")},
    };
    std::vector<SelectTarget> targets{
        SelectTarget(std::make_shared<Synonym>(EntityType::VARIABLE, "v"),
                     AttributeType::VARNAME),
        SelectTarget(std::make_shared<Synonym>(EntityType::CALL, "c"),
                     AttributeType::PROCNAME)};
    SelectClause selectClause;

    for (auto target : targets) {
      selectClause.addSelectTarget(target);
    }

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("some attributes, some pure synonyms, multiple synonyms") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("call"), ST("c"),
        PT(";"),        ST("Select"), PT("<"), ST("v"),    DT("."),
        AT("varName"),  ST(","),      ST("c"), PT(">")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")},
        {"c", Synonym(EntityType::CALL, "c")},
    };
    std::vector<SelectTarget> targets{
        SelectTarget(std::make_shared<Synonym>(EntityType::VARIABLE, "v"),
                     AttributeType::VARNAME),
        SelectTarget(std::make_shared<Synonym>(EntityType::CALL, "c"))};
    SelectClause selectClause;

    for (auto target : targets) {
      selectClause.addSelectTarget(target);
    }

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("multiple clauses") {
    std::vector<QueryToken> tokens{
        ST("assign"),   ST("a"), DT(","), ST("a1"),     PT(";"),
        ST("variable"), ST("v"), PT(";"), ST("Select"), PT("<"),
        ST("a1"),       ST(","), ST("v"), PT(">"),      ST("such that"),
        ST("Follows*"), PT("("), ST("a"), PT(","),      ST("a1"),
        PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::ASSIGN, "a")},
        {"a1", Synonym(EntityType::ASSIGN, "a1")},
        {"v", Synonym(EntityType::VARIABLE, "v")},
    };
    std::vector<SelectTarget> targets{
        SelectTarget(std::make_shared<Synonym>(EntityType::ASSIGN, "a1")),
        SelectTarget(std::make_shared<Synonym>(EntityType::VARIABLE, "v"))};
    SelectClause selectClause;

    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(-1, Synonym(EntityType::ASSIGN, "a"),
                          StmtRef::StmtRefType::SYNONYM);
    StmtRef rhs = StmtRef(-1, Synonym(EntityType::ASSIGN, "a1"),
                          StmtRef::StmtRefType::SYNONYM);
    std::shared_ptr<FollowsT> relationship =
        std::make_shared<FollowsT>(lhs, rhs);
    suchThatClause->setRelationship(relationship);

    for (auto target : targets) {
      selectClause.addSelectTarget(target);
    }

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }
}

TEST_CASE("Unit Test - Advanced QPS Parser - Test Select tuple - negative test "
          "cases") {
  SECTION("select undeclared variable") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("call"), ST("c"),
        PT(";"),        ST("Select"), PT("<"), ST("a"),    DT("."),
        AT("varName"),  ST(","),      ST("c"), PT(">")};

    try {
      QPSParser::parse(tokens);
      REQUIRE_FALSE(true);
    } catch (QPSException &e) {
      REQUIRE(e.getType() == QpsExceptionType::SEMANTIC);
    }
  }

  SECTION("invalid attribute") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("call"), ST("c"),
        PT(";"),        ST("Select"), PT("<"), ST("v"),    DT("."),
        AT("stmt#"),    ST(","),      ST("c"), PT(">")};

    try {
      QPSParser::parse(tokens);
      REQUIRE_FALSE(true);
    } catch (QPSException &e) {
      REQUIRE(e.getType() == QpsExceptionType::SYNTACTIC);
    }
    SECTION("invalid attribute procedure num") {
      std::vector<QueryToken> tokens{
          ST("procedure"), ST("p"),      PT(";"), ST("call"), ST("c"),
          PT(";"),         ST("Select"), PT("<"), ST("p"),    DT("."),
          AT("stmt#"),     ST(","),      ST("c"), PT(">")};

      try {
        QPSParser::parse(tokens);
        REQUIRE_FALSE(true);
      } catch (QPSException &e) {
        REQUIRE(e.getType() == QpsExceptionType::SYNTACTIC);
      }
    }

    SECTION("check priority of syntactic error vs semantic error") {
      std::vector<QueryToken> tokens{
          ST("variable"), ST("v"),       PT(";"),         ST("call"),
          ST("c"),        PT(";"),       ST("procedure"), ST("p"),
          PT(";"),        ST("Select"),  PT("<"),         ST("p"),
          DT("."),        AT("stmt#"),   ST(","),         ST("a"),
          DT("."),        AT("varName"), ST(","),         ST("c"),
          PT(">")};

      try {
        QPSParser::parse(tokens);
        REQUIRE_FALSE(true);
      } catch (QPSException &e) {
        REQUIRE(e.getType() == QpsExceptionType::SYNTACTIC);
      }
    }

    SECTION("show priority of syntactic error vs semantic error") {
      std::vector<QueryToken> tokens{
          ST("variable"),  ST("v"), PT(";"), ST("call"),   ST("c"),     PT(";"),
          ST("procedure"), ST("p"), PT(";"), ST("Select"), ST("a"),     DT("."),
          AT("varName"),   PT("<"), ST("p"), DT("."),      AT("stmt#"), ST(","),
          ST(","),         ST("c"), PT(">")};

      try {
        QPSParser::parse(tokens);
        REQUIRE_FALSE(true);
      } catch (QPSException &e) {
        REQUIRE(e.getType() == QpsExceptionType::SEMANTIC);
      }
    }
  }
}

TEST_CASE("Unit Test - Advanced QPS Parser - Test 2 Such That With No And") {
  SECTION("ModifiesS and UsesS - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"),  ST("v"),      PT(";"), ST("assign"),    ST("a"),
        PT(";"),         ST("Select"), ST("v"), ST("such that"), ST("Modifies"),
        PT("("),         ST("a"),      PT(","), ST("v"),         PT(")"),
        ST("such that"), ST("Uses"),   PT("("), ST("a"),         PT(","),
        ST("v"),         PT(")")};

    SelectClause selectClause = SelectClause("v", EntityType::VARIABLE);
    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")},
        {"a", Synonym(EntityType::ASSIGN, "a")}};

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(1, Synonym(EntityType::ASSIGN, "a"),
                          StmtRef::StmtRefType::SYNONYM);
    EntRef rhs = EntRef("", Synonym{EntityType::VARIABLE, "v"},
                        EntRef::EntRefType::SYNONYM);
    auto relationship = std::make_shared<ModifiesS>(lhs, rhs);
    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto suchThatClause2 = std::make_shared<SuchThatClause>();
    StmtRef lhs2 = StmtRef(1, Synonym(EntityType::ASSIGN, "a"),
                           StmtRef::StmtRefType::SYNONYM);
    EntRef rhs2 = EntRef("", Synonym{EntityType::VARIABLE, "v"},
                         EntRef::EntRefType::SYNONYM);
    auto relationship2 = std::make_shared<UsesS>(lhs, rhs);
    suchThatClause2->setRelationship(relationship2);
    expectedResult.addSuchThatClause(suchThatClause2);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test 2 Such That With And") {
  SECTION("ModifiesS and UsesS - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("assign"),    ST("a"),
        PT(";"),        ST("Select"), ST("v"), ST("such that"), ST("Modifies"),
        PT("("),        ST("a"),      PT(","), ST("v"),         PT(")"),
        ST("and"),      ST("Uses"),   PT("("), ST("a"),         PT(","),
        ST("v"),        PT(")")};

    SelectClause selectClause = SelectClause("v", EntityType::VARIABLE);
    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")},
        {"a", Synonym(EntityType::ASSIGN, "a")}};

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(1, Synonym(EntityType::ASSIGN, "a"),
                          StmtRef::StmtRefType::SYNONYM);
    EntRef rhs = EntRef("", Synonym{EntityType::VARIABLE, "v"},
                        EntRef::EntRefType::SYNONYM);
    auto relationship = std::make_shared<ModifiesS>(lhs, rhs);
    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto suchThatClause2 = std::make_shared<SuchThatClause>();
    StmtRef lhs2 = StmtRef(1, Synonym(EntityType::ASSIGN, "a"),
                           StmtRef::StmtRefType::SYNONYM);
    EntRef rhs2 = EntRef("", Synonym{EntityType::VARIABLE, "v"},
                         EntRef::EntRefType::SYNONYM);
    auto relationship2 = std::make_shared<UsesS>(lhs, rhs);
    suchThatClause2->setRelationship(relationship2);
    expectedResult.addSuchThatClause(suchThatClause2);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("ModifiesS and UsesS wrong use of and - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"),  ST("v"),        PT(";"),      ST("assign"),
        ST("a"),         PT(";"),        ST("Select"), ST("v"),
        ST("such that"), ST("Modifies"), PT("("),      ST("a"),
        PT(","),         ST("v"),        PT(")"),      ST("and"),
        ST("such that"), ST("Uses"),     PT("("),      ST("a"),
        PT(","),         ST("v"),        PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Different clause types with and - "
          "invalid") {
  std::vector<QueryToken> tokens{
      ST("variable"), ST("v"),      PT(";"), ST("assign"),    ST("a"),
      PT(";"),        ST("Select"), ST("v"), ST("such that"), ST("Modifies"),
      PT("("),        ST("a"),      PT(","), ST("v"),         PT(")"),
      ST("and"),      ST("a"),      PT("("), ST("v"),         PT(","),
      WT("_"),        QT("a+b"),    WT("_"), PT(")")};

  REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test Single Pattern") {

  SECTION("if pattern - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),       ST("if"), ST("f"), PT(";"),
        ST("Select"),   ST("v"), ST("pattern"), ST("f"),  PT("("), ST("v"),
        PT(","),        WT("_"), PT(","),       WT("_"),  PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"f", Synonym(EntityType::IF, "f")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("v", EntityType::VARIABLE);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    Synonym synPattern = Synonym(EntityType::IF, "f");
    EntRef lhs = EntRef("", Synonym(), EntRef::EntRefType::WILDCARD);
    ExpressionSpec rhs = ExpressionSpec(false, "_");
    auto patternClause = std::make_shared<PatternClause>(synPattern, lhs, rhs);
    expectedResult.addPatternClause(patternClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("while pattern - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("while"),   ST("w"),
        PT(";"),        ST("Select"), ST("v"), ST("pattern"), ST("w"),
        PT("("),        ST("v"),      PT(","), WT("_"),       PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"w", Synonym(EntityType::WHILE, "w")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("v", EntityType::VARIABLE);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    Synonym synPattern = Synonym(EntityType::WHILE, "w");
    EntRef lhs = EntRef("", Synonym(), EntRef::EntRefType::WILDCARD);
    ExpressionSpec rhs = ExpressionSpec(false, "_");
    auto patternClause = std::make_shared<PatternClause>(synPattern, lhs, rhs);
    expectedResult.addPatternClause(patternClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("if pattern with two arguments only - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("if"),      ST("f"),
        PT(";"),        ST("Select"), ST("v"), ST("pattern"), ST("f"),
        PT("("),        ST("v"),      PT(","), WT("_"),       PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("if pattern with second argument being QT - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),   PT(";"),       ST("if"), ST("f"), PT(";"),
        ST("Select"),   ST("v"),   ST("pattern"), ST("f"),  PT("("), ST("v"),
        PT(","),        QT("a+b"), PT(","),       WT("_"),  PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("if pattern with third argument being QT - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),       ST("if"), ST("f"), PT(";"),
        ST("Select"),   ST("v"), ST("pattern"), ST("f"),  PT("("), ST("v"),
        PT(","),        WT("_"), PT(","),       QT("a"),  PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("while pattern with second argument being QT - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("while"),   ST("w"),
        PT(";"),        ST("Select"), ST("v"), ST("pattern"), ST("w"),
        PT("("),        ST("v"),      PT(","), QT("\"a+b\""), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test 2 Pattern with no and") {
  SECTION("(synonym, partial match - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"),       ST("assign"),  ST("a"),
        PT(";"),        ST("Select"), ST("a"),       ST("pattern"), ST("a"),
        PT("("),        ST("v"),      PT(","),       WT("_"),       QT("a+b"),
        WT("_"),        PT(")"),      ST("pattern"), ST("a"),       PT("("),
        QT("x"),        PT(","),      WT("_"),       PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::ASSIGN, "a")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("a", EntityType::ASSIGN);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    Synonym synAssign = Synonym(EntityType::ASSIGN, "a");
    EntRef lhs = EntRef("", Synonym{EntityType::VARIABLE, "v"},
                        EntRef::EntRefType::SYNONYM);
    ExpressionSpec rhs = ExpressionSpec(true, "\"a+b\"");
    auto patternClause = std::make_shared<PatternClause>(synAssign, lhs, rhs);
    expectedResult.addPatternClause(patternClause);

    EntRef lhs2 = EntRef("x", Synonym(), EntRef::EntRefType::STRING);
    ExpressionSpec rhs2 = ExpressionSpec();
    auto patternClause2 =
        std::make_shared<PatternClause>(synAssign, lhs2, rhs2);
    expectedResult.addPatternClause(patternClause2);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test 2 Pattern with and") {
  SECTION("valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"),   ST("assign"),  ST("a"),
        PT(";"),        ST("Select"), ST("a"),   ST("pattern"), ST("a"),
        PT("("),        ST("v"),      PT(","),   WT("_"),       QT("a+b"),
        WT("_"),        PT(")"),      ST("and"), ST("a"),       PT("("),
        QT("x"),        PT(","),      WT("_"),   PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::ASSIGN, "a")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("a", EntityType::ASSIGN);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    Synonym synAssign = Synonym(EntityType::ASSIGN, "a");
    EntRef lhs = EntRef("", Synonym{EntityType::VARIABLE, "v"},
                        EntRef::EntRefType::SYNONYM);
    ExpressionSpec rhs = ExpressionSpec(true, "\"a+b\"");
    auto patternClause = std::make_shared<PatternClause>(synAssign, lhs, rhs);
    expectedResult.addPatternClause(patternClause);

    EntRef lhs2 = EntRef("x", Synonym(), EntRef::EntRefType::STRING);
    ExpressionSpec rhs2 = ExpressionSpec();
    auto patternClause2 =
        std::make_shared<PatternClause>(synAssign, lhs2, rhs2);
    expectedResult.addPatternClause(patternClause2);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }
  SECTION("wrong use of and - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"),   ST("assign"),  ST("a"),
        PT(";"),        ST("Select"), ST("a"),   ST("pattern"), ST("a"),
        PT("("),        ST("v"),      PT(","),   WT("_"),       QT("a+b"),
        WT("_"),        PT(")"),      ST("and"), ST("pattern"), ST("a"),
        PT("("),        QT("x"),      PT(","),   WT("_"),       PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test 3 Clauses with And") {
  SECTION("2 Such That and 1 Pattern in between - valid") {
    std::vector<QueryToken> tokens{
        ST("assign"),  ST("a"),      PT(";"), ST("while"),     ST("w"),
        PT(";"),       ST("Select"), ST("a"), ST("such that"), ST("Parent*"),
        PT("("),       ST("w"),      PT(","), ST("a"),         PT(")"),
        ST("pattern"), ST("a"),      PT("("), WT("_"),         PT(","),
        WT("_"),       QT("a+b"),    WT("_"), PT(")"),         ST("such that"),
        ST("Uses"),    PT("("),      ST("a"), PT(","),         QT("x"),
        PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::ASSIGN, "a")},
        {"w", Synonym(EntityType::WHILE, "w")}};
    SelectClause selectClause = SelectClause("a", EntityType::ASSIGN);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef rhs = StmtRef(1, Synonym(EntityType::ASSIGN, "a"),
                          StmtRef::StmtRefType::SYNONYM);
    StmtRef lhs = StmtRef(1, Synonym{EntityType::WHILE, "w"},
                          StmtRef::StmtRefType::SYNONYM);
    auto relationship = std::make_shared<ParentT>(lhs, rhs);
    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    Synonym synAssign = Synonym(EntityType::ASSIGN, "a");
    EntRef lhs2 = EntRef("", Synonym(), EntRef::EntRefType::WILDCARD);
    ExpressionSpec rhs2 = ExpressionSpec(true, "\"a+b\"");
    auto patternClause = std::make_shared<PatternClause>(synAssign, lhs2, rhs2);
    expectedResult.addPatternClause(patternClause);

    auto suchThatClause3 = std::make_shared<SuchThatClause>();
    StmtRef lhs3 = StmtRef(1, Synonym(EntityType::ASSIGN, "a"),
                           StmtRef::StmtRefType::SYNONYM);
    EntRef rhs3 = EntRef("x", Synonym(), EntRef::EntRefType::STRING);
    auto relationship3 = std::make_shared<UsesS>(lhs3, rhs3);
    suchThatClause3->setRelationship(relationship3);
    expectedResult.addSuchThatClause(suchThatClause3);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }
  SECTION(" using and such that - invalid") {
    std::vector<QueryToken> tokens{
        ST("assign"), ST("a"),        PT(";"),   ST("while"),     ST("w"),
        PT(";"),      ST("Select"),   ST("a"),   ST("such that"), ST("Parent*"),
        PT("("),      ST("w"),        PT(","),   ST("a"),         PT(")"),
        ST("and"),    ST("Modifies"), PT("("),   ST("a"),         PT(","),
        QT("x"),      PT(")"),        ST("and"), ST("such that"), ST("Uses"),
        PT("("),      ST("a"),        PT(","),   QT("x"),         PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("pattern followed by such that using and - invalid") {
    std::vector<QueryToken> tokens{
        ST("assign"),  ST("a"),      PT(";"), ST("while"),     ST("w"),
        PT(";"),       ST("Select"), ST("a"), ST("such that"), ST("Parent*"),
        PT("("),       ST("w"),      PT(","), ST("a"),         PT(")"),
        ST("pattern"), ST("a"),      PT("("), WT("_"),         PT(","),
        WT("_"),       QT("a+b"),    WT("_"), PT(")"),         ST("and"),
        ST("Uses"),    PT("("),      ST("a"), PT(","),         QT("x"),
        PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test Single With Clause - "
          "Positive test cases") {
  SECTION("IDENT = IDENT") {
    std::vector<QueryToken> tokens{ST("read"),   ST("a"), PT(";"),
                                   ST("Select"), ST("a"), ST("with"),
                                   QT("x"),      ET("="), QT("x")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);
    auto withClause =
        std::make_shared<AttrCompare>(std::make_shared<LiteralWithRef>("x"),
                                      std::make_shared<LiteralWithRef>("x"));

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("INTEGER = INTEGER") {
    std::vector<QueryToken> tokens{ST("read"),   ST("a"), PT(";"),
                                   ST("Select"), ST("a"), ST("with"),
                                   IT("1"),      ET("="), IT("2")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);
    auto withClause =
        std::make_shared<AttrCompare>(std::make_shared<IntegerWithRef>("1"),
                                      std::make_shared<IntegerWithRef>("2"));

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("attrRef = attrRef") {
    std::vector<QueryToken> tokens{
        ST("read"), ST("a"), PT(";"),    ST("Select"), ST("a"),
        ST("with"), ST("a"), DT("."),    AT("stmt#"),  ET("="),
        ST("a"),    DT("."), AT("stmt#")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    auto lhs = std::make_shared<AttrWithRef>(Synonym(EntityType::READ, "a"),
                                             AttributeType::STMTNUM);
    auto rhs = std::make_shared<AttrWithRef>(Synonym(EntityType::READ, "a"),
                                             AttributeType::STMTNUM);
    auto withClause = std::make_shared<AttrCompare>(lhs, rhs);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("INTEGER = attrRef(stmt#) (INTEGER = INTEGER)") {
    std::vector<QueryToken> tokens{
        ST("read"), ST("a"), PT(";"), ST("Select"), ST("a"),    ST("with"),
        IT("1"),    ET("="), ST("a"), DT("."),      AT("stmt#")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    auto lhs = std::make_shared<IntegerWithRef>("1");
    auto rhs = std::make_shared<AttrWithRef>(Synonym(EntityType::READ, "a"),
                                             AttributeType::STMTNUM);
    auto withClause = std::make_shared<AttrCompare>(lhs, rhs);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("INTEGER = attrRef(value) (INTEGER = INTEGER)") {
    std::vector<QueryToken> tokens{
        ST("constant"), ST("c"), PT(";"), ST("Select"), ST("c"),    ST("with"),
        IT("1"),        ET("="), ST("c"), DT("."),      AT("value")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"c", Synonym(EntityType::CONSTANT, "c")}};
    SelectClause selectClause = SelectClause("c", EntityType::CONSTANT);

    auto lhs = std::make_shared<IntegerWithRef>("1");
    auto rhs = std::make_shared<AttrWithRef>(Synonym(EntityType::CONSTANT, "c"),
                                             AttributeType::VALUE);
    auto withClause = std::make_shared<AttrCompare>(lhs, rhs);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("IDENT = attrRef(procName) (NAME = NAME)") {
    std::vector<QueryToken> tokens{ST("procedure"),  ST("p"),       PT(";"),
                                   ST("Select"),     ST("p"),       ST("with"),
                                   QT("methodName"), ET("="),       ST("p"),
                                   DT("."),          AT("procName")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"p", Synonym(EntityType::PROCEDURE, "p")}};
    SelectClause selectClause = SelectClause("p", EntityType::PROCEDURE);

    auto lhs = std::make_shared<LiteralWithRef>("methodName");
    auto rhs = std::make_shared<AttrWithRef>(
        Synonym(EntityType::PROCEDURE, "p"), AttributeType::PROCNAME);
    auto withClause = std::make_shared<AttrCompare>(lhs, rhs);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("IDENT = attrRef(varName) (NAME = NAME)") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),      PT(";"),
                                   ST("Select"),   ST("v"),      ST("with"),
                                   QT("varName"),  ET("="),      ST("v"),
                                   DT("."),        AT("varName")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("v", EntityType::VARIABLE);

    auto lhs = std::make_shared<LiteralWithRef>("varName");
    auto rhs = std::make_shared<AttrWithRef>(Synonym(EntityType::VARIABLE, "v"),
                                             AttributeType::VARNAME);
    auto withClause = std::make_shared<AttrCompare>(lhs, rhs);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test Single With Clause - "
          "Negative test cases") {
  // @todo add more negative test cases
  SECTION("Integer = attrRef(varName) (NAME = INTEGER)") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),      PT(";"),
                                   ST("Select"),   ST("v"),      ST("with"),
                                   IT("1"),        ET("="),      ST("v"),
                                   DT("."),        AT("varName")};

    try {
      QPSParser::parse(tokens);
      REQUIRE_FALSE(true);
    } catch (QPSException &e) {
      REQUIRE(e.getType() == QpsExceptionType::SEMANTIC);
    }
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test Multiple AttributeCompare in "
          "single With Clause") {
  // @todo add more test cases
  SECTION("IDENT = IDENT and Integer = Integer") {
    std::vector<QueryToken> tokens{
        ST("read"), ST("a"), PT(";"), ST("Select"), ST("a"),
        ST("with"), QT("x"), ET("="), QT("x"),      ST("and"),
        IT("1"),    ET("="), IT("2")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);
    auto withClause1 =
        std::make_shared<AttrCompare>(std::make_shared<LiteralWithRef>("x"),
                                      std::make_shared<LiteralWithRef>("x"));
    auto withClause2 =
        std::make_shared<AttrCompare>(std::make_shared<IntegerWithRef>("1"),
                                      std::make_shared<IntegerWithRef>("2"));

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause1);
    expectedResult.addAttrCompare(withClause2);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }
}

TEST_CASE("Unit Test - Advanced PQL Parser - Test Special Case") {
  SECTION("BOOLEAN as synonym") {
    std::vector<QueryToken> tokens{
        ST("read"), ST("BOOLEAN"), PT(";"), ST("Select"), ST("BOOLEAN"),
        ST("with"), QT("x"),       ET("="), QT("x"),      ST("and"),
        IT("1"),    ET("="),       IT("2"), ST("with"),   IT("3"),
        ET("="),    IT("4")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"BOOLEAN", Synonym(EntityType::READ, "BOOLEAN")}};
    SelectClause selectClause = SelectClause("BOOLEAN", EntityType::READ);
    auto withClause1 =
        std::make_shared<AttrCompare>(std::make_shared<LiteralWithRef>("x"),
                                      std::make_shared<LiteralWithRef>("x"));
    auto withClause2 =
        std::make_shared<AttrCompare>(std::make_shared<IntegerWithRef>("1"),
                                      std::make_shared<IntegerWithRef>("2"));
    auto withClause3 =
        std::make_shared<AttrCompare>(std::make_shared<IntegerWithRef>("3"),
                                      std::make_shared<IntegerWithRef>("4"));

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    expectedResult.addAttrCompare(withClause1);
    expectedResult.addAttrCompare(withClause2);
    expectedResult.addAttrCompare(withClause3);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }
}
