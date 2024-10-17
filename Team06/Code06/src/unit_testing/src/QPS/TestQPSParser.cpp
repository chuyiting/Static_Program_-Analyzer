//
// Created by Eddy Chu on 2022/9/2.
//

#include "QPS/evaluator/relationships/concreteRelationships/Affects.h"
#include "QPS/evaluator/relationships/concreteRelationships/AffectsT.h"
#include "QPS/evaluator/relationships/concreteRelationships/Calls.h"
#include "QPS/evaluator/relationships/concreteRelationships/CallsT.h"
#include "QPS/evaluator/relationships/concreteRelationships/ModifiesS.h"
#include "QPS/evaluator/relationships/concreteRelationships/Next.h"
#include "QPS/evaluator/relationships/concreteRelationships/NextT.h"
#include "QPS/evaluator/relationships/concreteRelationships/ParentT.h"
#include "QPS/evaluator/relationships/concreteRelationships/UsesS.h"
#include "QPS/exceptions/QPSException.h"
#include "QPS/parser/QPSParser.h"
#include "QPS/parser/QPSQuery.h"
#include "QPS/parser/querystructs/patternclause/ExpressionSpec.h"
#include "QPS/parser/querystructs/patternclause/PatternClause.h"
#include "QPS/tokenizer/QueryToken.h"
#include "catch.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "QPS/evaluator/relationships/concreteRelationships/UsesP.h"
#include "TestQPSParser.h"

TEST_CASE("Unit Test - PQL Parser - Test Synonym Declaration") {
  SECTION("Single Type - Single Variable") {
    std::vector<QueryToken> tokens{ST("read"), ST("a"), PT(";"), ST("Select"),
                                   ST("a")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("Single type - Multiple Variable") {
    std::vector<QueryToken> tokens{ST("print"), ST("a"),      PT(","),
                                   ST("b"),     PT(","),      ST("c"),
                                   PT(";"),     ST("Select"), ST("b")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::PRINT, "a")},
        {"b", Synonym(EntityType::PRINT, "b")},
        {"c", Synonym(EntityType::PRINT, "c")}};
    SelectClause selectClause = SelectClause("b", EntityType::PRINT);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("Multiple type - Multiple Variable") {
    std::vector<QueryToken> tokens{
        ST("call"),     ST("a"),      PT(","),         ST("b"),
        PT(";"),        ST("while"),  ST("c"),         PT(","),
        ST("d"),        PT(";"),      ST("if"),        ST("e"),
        PT(";"),        ST("assign"), ST("f"),         PT(";"),
        ST("variable"), ST("g"),      PT(";"),         ST("constant"),
        ST("h"),        PT(";"),      ST("procedure"), ST("i"),
        PT(";"),        ST("Select"), ST("a")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::CALL, "a")},
        {"b", Synonym(EntityType::CALL, "b")},
        {"c", Synonym(EntityType::WHILE, "c")},
        {"d", Synonym(EntityType::WHILE, "d")},
        {"e", Synonym(EntityType::IF, "e")},
        {"f", Synonym(EntityType::ASSIGN, "f")},
        {"g", Synonym(EntityType::VARIABLE, "g")},
        {"h", Synonym(EntityType::CONSTANT, "h")},
        {"i", Synonym(EntityType::PROCEDURE, "i")}};
    SelectClause selectClause = SelectClause("a", EntityType::CALL);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("Duplicate declaration") {
    std::vector<QueryToken> tokens{ST("read"), ST("v"), ST("read"),
                                   ST("v"),    PT(";"), ST("Select"),
                                   ST("v")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("Invalid Synonym name") {
    std::vector<QueryToken> tokens{ST("read"), ST("123a"), PT(";"),
                                   ST("Select"), ST("v")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("Invalid Synonym type") {
    std::vector<QueryToken> tokens{ST("ReAd"), ST("a"), PT(";"), ST("Select"),
                                   ST("v")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("Missing Semi-colon") {
    std::vector<QueryToken> tokens{ST("ReAd"), ST("a"), ST("Select"), ST("v")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }
}

TEST_CASE("Unit Test - PQL Parser - Test Select") {
  SECTION("Positive test case") {
    std::vector<QueryToken> tokens{ST("read"), ST("a"), PT(";"), ST("Select"),
                                   ST("a")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    auto result = QPSParser::parse(tokens);
    REQUIRE(*result == expectedResult);
  }

  SECTION("Select tuples simple") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"), ST("stmt"), ST("s"), PT(";"),
        ST("Select"),   PT("<"), ST("v"), PT(","),    ST("s"), PT(">")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")},
        {"s", Synonym(EntityType::STMT, "s")}};
    std::vector<std::shared_ptr<Synonym>> syns{
        std::make_shared<Synonym>(EntityType::VARIABLE, "v"),
        std::make_shared<Synonym>(EntityType::STMT, "s")};
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

  SECTION("Select tuples with only one syn") {
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

  SECTION("Select tuples without < ... >") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("stmt"), ST("s"),
        PT(";"),        ST("Select"), ST("v"), PT(","),    ST("s")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

   SECTION("Select tuples without >") {
       std::vector<QueryToken> tokens{
           ST("variable"), ST("v"), PT(";"), ST("stmt"),ST("s"),PT(";"),
           ST("Select"), PT("<"), ST("v"),PT(","),ST("s")
       };

       REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));

   }

   SECTION("Select tuples with one syn only without <") {
       std::vector<QueryToken> tokens{
           ST("variable"), ST("v"), PT(";"), ST("stmt"),ST("s"),PT(";"),
           ST("Select"), PT("<"), ST("v")
       };

       REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));

   }

  SECTION("Select tuples with undeclared syn") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("stmt"), ST("s"),
        PT(";"),        ST("Select"), PT("<"), ST("v"),    PT(","),
        ST("s"),        PT(","),      ST("a"), PT(">")};

    REQUIRE_THROWS_WITH(
        QPSParser::parse(tokens),
        QPSParseExceptionMessage::QPS_INVALID_SELECT_SYNONYM_NOT_DEFINED);
  }

}

TEST_CASE("Unit Test - PQL Parser - Test Single Such That") {
  SECTION("ModifiesS - (literal, literal) - valid") {
    std::vector<QueryToken> tokens{ST("read"),     ST("a"),     PT(";"),
                                   ST("Select"),   ST("a"),     ST("such that"),
                                   ST("Modifies"), PT("("),     IT("1"),
                                   PT(","),        QT("myvar"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(1, Synonym(), StmtRef::StmtRefType::INTEGER);
    EntRef rhs = EntRef("myvar", Synonym(), EntRef::EntRefType::STRING);
    auto relationship = std::make_shared<ModifiesS>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("ModifiesS - (literal, literal) - invalid literal - stmtRef not "
          "integer") {
    std::vector<QueryToken> tokens{
        ST("read"),       ST("a"),         PT(";"),        ST("Select"),
        ST("a"),          ST("such that"), ST("Modifies"), PT("("),
        ST("\"mystmt\""), PT(","),         QT("myvar"),    PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION(
      "ModifiesS - (literal, literal) - invalid literal - entRef not IDENT") {
    std::vector<QueryToken> tokens{
        ST("read"), ST("a"),         PT(";"),        ST("Select"),
        ST("a"),    ST("such that"), ST("Modifies"), PT("("),
        IT("1"),    PT(","),         QT("1myent"),   PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("ModifiesS - (wildcard, literal) - invalid") {
    std::vector<QueryToken> tokens{
        ST("read"), ST("a"),         PT(";"),        ST("Select"),
        ST("a"),    ST("such that"), ST("Modifies"), PT("("),
        WT("_"),    PT(","),         QT("1myent"),   PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("ModifiesS - (synonym, literal) syn is non-proc and non-stmt-related "
          "- invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),     PT(";"),
                                   ST("Select"),   ST("v"),     ST("such that"),
                                   ST("Modifies"), PT("("),     ST("v"),
                                   PT(","),        QT("\"a\""), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("ModifiesS - (literal, synonym) syn is non-var - invalid") {
    std::vector<QueryToken> tokens{
        ST("stmt"), ST("s"),      PT(";"), ST("procedure"), ST("p"),
        PT(";"),    ST("Select"), ST("s"), ST("such that"), ST("Modifies"),
        PT("("),    ST("s"),      PT(","), ST("p"),         PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("ModifiesS - (literal, wildcard) - valid") {
    std::vector<QueryToken> tokens{ST("read"),     ST("a"), PT(";"),
                                   ST("Select"),   ST("a"), ST("such that"),
                                   ST("Modifies"), PT("("), IT("1"),
                                   PT(","),        WT("_"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(1, Synonym(), StmtRef::StmtRefType::INTEGER);
    EntRef rhs = EntRef("_", Synonym(), EntRef::EntRefType::WILDCARD);
    auto relationship = std::make_shared<ModifiesS>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("ModifiesS - (literal, synonym) - valid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"), PT(";"),
                                   ST("Select"),   ST("v"), ST("such that"),
                                   ST("Modifies"), PT("("), IT("1"),
                                   PT(","),        ST("v"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("v", EntityType::VARIABLE);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(1, Synonym(), StmtRef::StmtRefType::INTEGER);
    EntRef rhs = EntRef("", Synonym{EntityType::VARIABLE, "v"},
                        EntRef::EntRefType::SYNONYM);
    auto relationship = std::make_shared<ModifiesS>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("UsesS - (synonym, syn constant) - invalid") {
    std::vector<QueryToken> tokens{
        ST("assign"), ST("a"),      PT(";"), ST("constant"),  ST("c"),
        PT(";"),      ST("Select"), ST("a"), ST("such that"), ST("Uses"),
        PT("("),      ST("a"),      PT(","), ST("c"),         PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::ASSIGN, "a")},
        {"c", Synonym(EntityType::CONSTANT, "c")}};
    SelectClause selectClause = SelectClause("a", EntityType::ASSIGN);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(-1, Synonym(EntityType::ASSIGN, "a"),
                          StmtRef::StmtRefType::SYNONYM);
    EntRef rhs = EntRef("", Synonym(EntityType::CONSTANT, "c"),
                        EntRef::EntRefType::SYNONYM);
    auto relationship = std::make_shared<UsesS>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    try {
      auto result = QPSParser::parse(tokens);
      REQUIRE_FALSE(true);
    } catch (QPSException &e) {
      REQUIRE(e.getType() == QpsExceptionType::SEMANTIC);
    }
  }

  SECTION("UsesP - (synonym, literal) - valid") {
    std::vector<QueryToken> tokens{ST("procedure"), ST("p"), PT(";"),
                                   ST("Select"),    ST("p"), ST("such that"),
                                   ST("Uses"),      PT("("), ST("p"),
                                   PT(","),         QT("x"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"p", Synonym(EntityType::PROCEDURE, "p")}};
    SelectClause selectClause = SelectClause("p", EntityType::PROCEDURE);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    EntRef lhs = EntRef("", Synonym(EntityType::PROCEDURE, "p"),
                        EntRef::EntRefType::SYNONYM);
    EntRef rhs = EntRef("x", Synonym(), EntRef::EntRefType::STRING);
    auto relationship = std::make_shared<UsesP>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("Calls - (synonym, literal) syn is non-procedure- invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),     PT(";"),
                                   ST("Select"),   ST("v"),     ST("such that"),
                                   ST("Calls"),    PT("("),     ST("v"),
                                   PT(","),        QT("proc2"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("ModifiesS - (synonym, synonym) - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("assign"),    ST("a"),
        PT(";"),        ST("Select"), ST("v"), ST("such that"), ST("Modifies"),
        PT("("),        ST("a"),      PT(","), ST("v"),         PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"v", Synonym(EntityType::VARIABLE, "v")},
        {"a", Synonym(EntityType::ASSIGN, "a")}};
    SelectClause selectClause = SelectClause("v", EntityType::VARIABLE);

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

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("Calls - (literal, literal) - valid") {
    std::vector<QueryToken> tokens{ST("read"),   ST("a"),     PT(";"),
                                   ST("Select"), ST("a"),     ST("such that"),
                                   ST("Calls"),  PT("("),     QT("proc1"),
                                   PT(","),      QT("proc2"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    EntRef lhs = EntRef("proc1", Synonym(), EntRef::EntRefType::STRING);
    EntRef rhs = EntRef("proc2", Synonym(), EntRef::EntRefType::STRING);
    auto relationship = std::make_shared<Calls>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("Calls - (synonym, literal) - valid") {
    std::vector<QueryToken> tokens{
        ST("procedure"), ST("p"),         PT(";"),     ST("Select"),
        ST("p"),         ST("such that"), ST("Calls"), PT("("),
        ST("p"),         PT(","),         QT("proc2"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"p", Synonym(EntityType::PROCEDURE, "p")}};
    SelectClause selectClause = SelectClause("p", EntityType::PROCEDURE);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    EntRef lhs = EntRef("", Synonym(EntityType::PROCEDURE, "p"),
                        EntRef::EntRefType::SYNONYM);
    EntRef rhs = EntRef("proc2", Synonym(), EntRef::EntRefType::STRING);
    auto relationship = std::make_shared<Calls>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("CallsT - (literal, literal) - valid") {
    std::vector<QueryToken> tokens{ST("read"),   ST("a"),     PT(";"),
                                   ST("Select"), ST("a"),     ST("such that"),
                                   ST("Calls*"), PT("("),     QT("proc1"),
                                   PT(","),      QT("proc2"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::READ, "a")}};
    SelectClause selectClause = SelectClause("a", EntityType::READ);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    EntRef lhs = EntRef("proc1", Synonym(), EntRef::EntRefType::STRING);
    EntRef rhs = EntRef("proc2", Synonym(), EntRef::EntRefType::STRING);
    auto relationship = std::make_shared<CallsT>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("CallsT - (synonym, literal) - valid") {
    std::vector<QueryToken> tokens{
        ST("procedure"), ST("p"),         PT(";"),      ST("Select"),
        ST("p"),         ST("such that"), ST("Calls*"), PT("("),
        ST("p"),         PT(","),         QT("proc2"),  PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"p", Synonym(EntityType::PROCEDURE, "p")}};
    SelectClause selectClause = SelectClause("p", EntityType::PROCEDURE);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    EntRef lhs = EntRef("", Synonym(EntityType::PROCEDURE, "p"),
                        EntRef::EntRefType::SYNONYM);
    EntRef rhs = EntRef("proc2", Synonym(), EntRef::EntRefType::STRING);
    auto relationship = std::make_shared<CallsT>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("Calls - (synonym, literal) syn is non-proc - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),     PT(";"),
                                   ST("Select"),   ST("v"),     ST("such that"),
                                   ST("Calls"),    PT("("),     ST("v"),
                                   PT(","),        QT("proc2"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("Calls - (literal, syn) syn is non-proc - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"), PT(";"),
                                   ST("Select"),   ST("v"), ST("such that"),
                                   ST("Calls"),    PT("("), QT("\"proc1\""),
                                   PT(","),        ST("v"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("Next - (syn, literal)  - valid") {
    std::vector<QueryToken> tokens{
        ST("stmt"), ST("s"), PT(";"), ST("Select"), ST("s"),  ST("such that"),
        ST("Next"), PT("("), ST("s"), PT(","),      IT("10"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"s", Synonym(EntityType::STMT, "s")}};
    SelectClause selectClause = SelectClause("s", EntityType::STMT);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(Synonym(EntityType::STMT, "s"));
    StmtRef rhs = StmtRef(10);
    auto relationship = std::make_shared<Next>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("NextT - (syn, literal)  - valid") {
    std::vector<QueryToken> tokens{
        ST("stmt"),  ST("s"), PT(";"), ST("Select"), ST("s"),  ST("such that"),
        ST("Next*"), PT("("), ST("s"), PT(","),      IT("10"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"s", Synonym(EntityType::STMT, "s")}};
    SelectClause selectClause = SelectClause("s", EntityType::STMT);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(Synonym(EntityType::STMT, "s"));
    StmtRef rhs = StmtRef(10);
    auto relationship = std::make_shared<NextT>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("Affects - (syn, literal)  - valid") {
    std::vector<QueryToken> tokens{ST("stmt"),    ST("s"),  PT(";"),
                                   ST("Select"),  ST("s"),  ST("such that"),
                                   ST("Affects"), PT("("),  ST("s"),
                                   PT(","),       IT("10"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"s", Synonym(EntityType::STMT, "s")}};
    SelectClause selectClause = SelectClause("s", EntityType::STMT);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(Synonym(EntityType::STMT, "s"));
    StmtRef rhs = StmtRef(10);
    auto relationship = std::make_shared<Affects>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("AffectsT - (syn, literal)  - valid") {
    std::vector<QueryToken> tokens{ST("stmt"),     ST("s"),  PT(";"),
                                   ST("Select"),   ST("s"),  ST("such that"),
                                   ST("Affects*"), PT("("),  ST("s"),
                                   PT(","),        IT("10"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"s", Synonym(EntityType::STMT, "s")}};
    SelectClause selectClause = SelectClause("s", EntityType::STMT);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(Synonym(EntityType::STMT, "s"));
    StmtRef rhs = StmtRef(10);
    auto relationship = std::make_shared<AffectsT>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("Next - (invalid_syn,literal) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),  PT(";"),
                                   ST("Select"),   ST("v"),  ST("such that"),
                                   ST("Next"),     PT("("),  ST("v"),
                                   PT(","),        IT("10"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("Next - (literal,invalid_syn) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"), PT(";"),
                                   ST("Select"),   ST("v"), ST("such that"),
                                   ST("Next"),     PT("("), IT("10"),
                                   PT(","),        ST("v"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("NextT - (invalid_syn,literal) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),  PT(";"),
                                   ST("Select"),   ST("v"),  ST("such that"),
                                   ST("Next*"),    PT("("),  ST("v"),
                                   PT(","),        IT("10"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("NextT - (literal,invalid_syn) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"), PT(";"),
                                   ST("Select"),   ST("v"), ST("such that"),
                                   ST("Next*"),    PT("("), IT("10"),
                                   PT(","),        ST("v"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("Affects - (invalid_syn,literal) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),  PT(";"),
                                   ST("Select"),   ST("v"),  ST("such that"),
                                   ST("Affects"),  PT("("),  ST("v"),
                                   PT(","),        IT("10"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("Affects - (literal,invalid_syn) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"), PT(";"),
                                   ST("Select"),   ST("v"), ST("such that"),
                                   ST("Affects"),  PT("("), IT("10"),
                                   PT(","),        ST("v"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("AffectsT - (invalid_syn,literal) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"),  PT(";"),
                                   ST("Select"),   ST("v"),  ST("such that"),
                                   ST("Affects*"), PT("("),  ST("v"),
                                   PT(","),        IT("10"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("AffectsT - (literal,invalid_syn) - invalid") {
    std::vector<QueryToken> tokens{ST("variable"), ST("v"), PT(";"),
                                   ST("Select"),   ST("v"), ST("such that"),
                                   ST("Affects*"), PT("("), IT("10"),
                                   PT(","),        ST("v"), PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens),
                   QPSException("", QpsExceptionType::SEMANTIC));
  }

  SECTION("Affects - (wildcard, wildcard) - valid") {
    std::vector<QueryToken> tokens{ST("stmt"), ST("such"), PT(";"),
                                   ST("Select"),   ST("such"), ST("such that"),
                                   ST("Affects"), PT("("), WT("_"),
                                   PT(","),        ST("such"), PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"such", Synonym(EntityType::STMT, "such")}};
    SelectClause selectClause = SelectClause("such", EntityType::STMT);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);
    auto suchThatClause = std::make_shared<SuchThatClause>();
    StmtRef lhs = StmtRef(1, Synonym(), StmtRef::StmtRefType::WILDCARD);
    StmtRef rhs = StmtRef(Synonym(EntityType::STMT, "such"));
    auto relationship = std::make_shared<Affects>(lhs, rhs);

    suchThatClause->setRelationship(relationship);
    expectedResult.addSuchThatClause(suchThatClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }
}

TEST_CASE("Unit Test - PQL Parser - Test Single Pattern") {
  SECTION("(synonym, partial match - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),       ST("assign"), ST("a"), PT(";"),
        ST("Select"),   ST("a"), ST("pattern"), ST("a"),      PT("("), ST("v"),
        PT(","),        WT("_"), QT("a+b"),     WT("_"),      PT(")")};

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

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("(synonym, total match - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("assign"),  ST("a"),
        PT(";"),        ST("Select"), ST("a"), ST("pattern"), ST("a"),
        PT("("),        ST("v"),      PT(","), QT("a+b"),     PT(")")};

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
    ExpressionSpec rhs = ExpressionSpec(false, "\"a+b\"");
    auto patternClause = std::make_shared<PatternClause>(synAssign, lhs, rhs);
    expectedResult.addPatternClause(patternClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("(wildcard, total match - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("assign"),  ST("a"),
        PT(";"),        ST("Select"), ST("a"), ST("pattern"), ST("a"),
        PT("("),        ST("v"),      PT(","), QT("a+b"),     PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::ASSIGN, "a")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("a", EntityType::ASSIGN);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    Synonym synAssign = Synonym(EntityType::ASSIGN, "a");
    EntRef lhs = EntRef("", Synonym(), EntRef::EntRefType::WILDCARD);
    ExpressionSpec rhs = ExpressionSpec(false, "\"a+b\"");
    auto patternClause = std::make_shared<PatternClause>(synAssign, lhs, rhs);
    expectedResult.addPatternClause(patternClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("(wildcard, partial match - valid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),       ST("assign"), ST("a"), PT(";"),
        ST("Select"),   ST("a"), ST("pattern"), ST("a"),      PT("("), ST("v"),
        PT(","),        WT("_"), QT("a+b"),     WT("_"),      PT(")")};

    std::unordered_map<synonym_t, Synonym> synonymDecls{
        {"a", Synonym(EntityType::ASSIGN, "a")},
        {"v", Synonym(EntityType::VARIABLE, "v")}};
    SelectClause selectClause = SelectClause("a", EntityType::ASSIGN);

    QPSQuery expectedResult = QPSQuery();
    expectedResult.setSynonymDecls(synonymDecls);
    expectedResult.setSelectClause(selectClause);

    Synonym synAssign = Synonym(EntityType::ASSIGN, "a");
    EntRef lhs = EntRef("", Synonym(), EntRef::EntRefType::WILDCARD);
    ExpressionSpec rhs = ExpressionSpec(true, "\"a+b\"");
    auto patternClause = std::make_shared<PatternClause>(synAssign, lhs, rhs);
    expectedResult.addPatternClause(patternClause);

    auto result = QPSParser::parse(tokens);

    REQUIRE(*result == expectedResult);
  }

  SECTION("(wildcard, total match, invalid expression - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),       ST("assign"), ST("a"), PT(";"),
        ST("Select"),   ST("a"), ST("pattern"), ST("a"),      PT("("), ST("v"),
        PT(","),        WT("_"), QT("+a+b"),    WT("_"),      PT(")")};
    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("(wildcard, total match, invalid expression - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"),      PT(";"), ST("assign"),  ST("a"),
        PT(";"),        ST("Select"), ST("a"), ST("pattern"), ST("a"),
        PT("("),        ST("v"),      PT(","), QT("+a+b"),    PT(")")};
    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("(wildcard, missing right underscore) - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),       ST("assign"), ST("a"), PT(";"),
        ST("Select"),   ST("a"), ST("pattern"), ST("a"),      PT("("), ST("v"),
        PT(","),        WT("_"), QT("a+b"),     PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }

  SECTION("not- assign (wildcard, partial match) - invalid") {
    std::vector<QueryToken> tokens{
        ST("variable"), ST("v"), PT(";"),       ST("assign"), ST("a"), PT(";"),
        ST("Select"),   ST("a"), ST("pattern"), ST("v"),      PT("("), ST("v"),
        PT(","),        WT("_"), QT("a+b"),     WT("_"),      PT(")")};

    REQUIRE_THROWS(QPSParser::parse(tokens), QPSException(""));
  }
}
