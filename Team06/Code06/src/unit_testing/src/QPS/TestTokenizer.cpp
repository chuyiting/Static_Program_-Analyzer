#include "QPS/exceptions/QPSException.h"
#include "QPS/tokenizer/QueryToken.h"
#include "QPS/tokenizer/QueryTokenizer.h"
#include "catch.hpp"
#include <string>

#include "TestQPSParser.h"

TEST_CASE("Unit test - QueryTokenizer - simple case") {
  std::string queryString = "variable v;\nSelect v";
  auto result = QueryTokenizer::tokenize(queryString);
  std::vector<QueryToken> expectedResult = {ST("variable"), ST("v"), PT(";"),
                                            ST("Select"), ST("v")};

  REQUIRE(result == expectedResult);
}

TEST_CASE("Unit test - QueryTokenizer - simple multiple tuples case") {
  std::string queryString = "variable v; stmt s;\nSelect <v,s>";
  auto result = QueryTokenizer::tokenize(queryString);
  std::vector<QueryToken> expectedResult = {
      ST("variable"), ST("v"), PT(";"), ST("stmt"), ST("s"), PT(";"),
      ST("Select"),   PT("<"), ST("v"), PT(","),    ST("s"), PT(">")};

  REQUIRE(result == expectedResult);
}

TEST_CASE("Unit test - QueryTokenizer - simple case with space at the end") {
  std::string queryString = "variable v;\nSelect v ";
  auto result = QueryTokenizer::tokenize(queryString);
  std::vector<QueryToken> expectedResult = {ST("variable"), ST("v"), PT(";"),
                                            ST("Select"), ST("v")};

  REQUIRE(result == expectedResult);
}

TEST_CASE("Unit test - QueryTokenizer - test such that") {
  SECTION("simple positive case") {
    std::string queryString = "variable v; Select v such that Uses(1,v)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),         PT(";"),    ST("Select"),
        ST("v"),        ST("such that"), ST("Uses"), PT("("),
        IT("1"),        PT(","),         ST("v"),    PT(")")};
    REQUIRE(result == expectedResult);
  }

  SECTION("simple positive case") {
    std::string queryString = "variable v; Select v such that Modifies(1, v)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),         PT(";"),        ST("Select"),
        ST("v"),        ST("such that"), ST("Modifies"), PT("("),
        IT("1"),        PT(","),         ST("v"),        PT(")")};
    REQUIRE(result == expectedResult);
  }

  SECTION("no space") {
    std::string queryString = "variable v;\nSelect v suchthat Modifies(a, b)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"), PT(";"), ST("Select"), ST("v"), ST("suchthat"),
        ST("Modifies"), PT("("), ST("a"), PT(","),      ST("b"), PT(")")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit test - QueryTokenizer - test '_'") {
  std::string queryString = "variable v;\nSelect v such that Modifies(a, _)";
  std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
  std::vector<QueryToken> expectedResult = {
      ST("variable"), ST("v"), PT(";"), ST("Select"), ST("v"), ST("such that"),
      ST("Modifies"), PT("("), ST("a"), PT(","),      WT("_"), PT(")")};
  REQUIRE(result == expectedResult);
}

TEST_CASE("Unit test - QueryTokenizer - test quotation") {
  SECTION("valid quotation") {
    std::string queryString =
        "variable v;\nSelect v such that Modifies(a, \"myvar\")";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),         PT(";"),        ST("Select"),
        ST("v"),        ST("such that"), ST("Modifies"), PT("("),
        ST("a"),        PT(","),         QT("myvar"),    PT(")")};

    REQUIRE(result == expectedResult);
  }

  SECTION("incomplete quotation") {
    std::string queryString =
        "variable v;\nSelect v such that Modifies(a, \"myvar)";
    REQUIRE_THROWS(
        QueryTokenizer::tokenize(queryString),
        QPSException(
            QPSTokenizerExceptionMessage::QPS_TOKENIZER_INVALID_TOKEN));
  }

  SECTION("incomplete quotation") {
    std::string queryString =
        "variable v;\nSelect v such that \"Modifies(a, \"myvar\")";
    REQUIRE_THROWS(
        QueryTokenizer::tokenize(queryString),
        QPSException(
            QPSTokenizerExceptionMessage::QPS_TOKENIZER_INVALID_TOKEN));
  }
}

TEST_CASE("Unit test - QueryTokenizer - test pattern") {
  SECTION("exact match") {
    std::string queryString =
        "variable v;assign a;\nSelect v pattern a(v, \"b+c\")";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),      PT(";"), ST("assign"),  ST("a"),
        PT(";"),        ST("Select"), ST("v"), ST("pattern"), ST("a"),
        PT("("),        ST("v"),      PT(","), QT("b+c"),     PT(")")};
    REQUIRE(result == expectedResult);
  }

  SECTION("partial match") {
    std::string queryString =
        "variable v;assign a;\nSelect v pattern a(v, _\"b+c\"_)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"), PT(";"),       ST("assign"), ST("a"), PT(";"),
        ST("Select"),   ST("v"), ST("pattern"), ST("a"),      PT("("), ST("v"),
        PT(","),        WT("_"), QT("b+c"),     WT("_"),      PT(")")};
    REQUIRE(result == expectedResult);
  }

  SECTION("if pattern") {
    std::string queryString = "variable v;if f;\nSelect v pattern f(v, _)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),      PT(";"), ST("if"),      ST("f"),
        PT(";"),        ST("Select"), ST("v"), ST("pattern"), ST("f"),
        PT("("),        ST("v"),      PT(","), WT("_"),       PT(")")};
    REQUIRE(result == expectedResult);
  }
  SECTION("while pattern") {
    std::string queryString = "variable v;while w;\nSelect v pattern w(v, _,_)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"), PT(";"),       ST("while"), ST("w"), PT(";"),
        ST("Select"),   ST("v"), ST("pattern"), ST("w"),     PT("("), ST("v"),
        PT(","),        WT("_"), PT(","),       WT("_"),     PT(")")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit test - QueryTokenizer - Multiple clauses with no and") {
  SECTION("2 such that clauses") {
    std::string queryString =
        "variable v;\nSelect v such that Modifies(a, v) such that Uses(a,v)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"),  ST("v"),        PT(";"),         ST("Select"), ST("v"),
        ST("such that"), ST("Modifies"), PT("("),         ST("a"),      PT(","),
        ST("v"),         PT(")"),        ST("such that"), ST("Uses"),   PT("("),
        ST("a"),         PT(","),        ST("v"),         PT(")")};
    REQUIRE(result == expectedResult);
  }
  SECTION("2 pattern clauses") {
    std::string queryString =
        "variable v;\nSelect v pattern a(v, \"b+c\") pattern a(\"x\",_)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"), PT(";"),       ST("Select"), ST("v"),
        ST("pattern"),  ST("a"), PT("("),       ST("v"),      PT(","),
        QT("b+c"),      PT(")"), ST("pattern"), ST("a"),      PT("("),
        QT("x"),        PT(","), WT("_"),       PT(")")};
    REQUIRE(result == expectedResult);
  }

  SECTION("2 such that clauses and 1 pattern at the end") {
    std::string queryString = "variable v;\nSelect v such that Modifies(a, v) "
                              "such that Uses(a,v) pattern a(\"x\",_)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"),  ST("v"),         PT(";"),        ST("Select"),
        ST("v"),         ST("such that"), ST("Modifies"), PT("("),
        ST("a"),         PT(","),         ST("v"),        PT(")"),
        ST("such that"), ST("Uses"),      PT("("),        ST("a"),
        PT(","),         ST("v"),         PT(")"),        ST("pattern"),
        ST("a"),         PT("("),         QT("x"),        PT(","),
        WT("_"),         PT(")")};
    REQUIRE(result == expectedResult);
  }

  SECTION("2 such that clauses and 1 pattern in between") {
    std::string queryString = "variable v;\nSelect v such that Modifies(a, v) "
                              "pattern a(\"x\",_) such that Uses(a,v)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),         PT(";"),        ST("Select"),
        ST("v"),        ST("such that"), ST("Modifies"), PT("("),
        ST("a"),        PT(","),         ST("v"),        PT(")"),
        ST("pattern"),  ST("a"),         PT("("),        QT("x"),
        PT(","),        WT("_"),         PT(")"),        ST("such that"),
        ST("Uses"),     PT("("),         ST("a"),        PT(","),
        ST("v"),        PT(")")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit test - QueryTokenizer - Multiple clauses with and") {
  SECTION("2 such that clause using and") {
    std::string queryString =
        "variable v;\nSelect v such that Modifies(a, v) and Uses(a,v)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"),  ST("v"),        PT(";"),   ST("Select"), ST("v"),
        ST("such that"), ST("Modifies"), PT("("),   ST("a"),      PT(","),
        ST("v"),         PT(")"),        ST("and"), ST("Uses"),   PT("("),
        ST("a"),         PT(","),        ST("v"),   PT(")")};
    REQUIRE(result == expectedResult);
  }
  SECTION("2 pattern clause using and") {
    std::string queryString =
        "variable v;\nSelect v pattern a(v, \"b+c\") and a(\"x\",_)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"), PT(";"),   ST("Select"), ST("v"),
        ST("pattern"),  ST("a"), PT("("),   ST("v"),      PT(","),
        QT("b+c"),      PT(")"), ST("and"), ST("a"),      PT("("),
        QT("x"),        PT(","), WT("_"),   PT(")")};
    REQUIRE(result == expectedResult);
  }

  SECTION("2 such that clause, 1 pattern using and") {
    std::string queryString = "variable v;\nSelect v such that Modifies(a, v) "
                              "and Uses(a,v) pattern a(\"x\",_)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"),  ST("v"),        PT(";"),   ST("Select"), ST("v"),
        ST("such that"), ST("Modifies"), PT("("),   ST("a"),      PT(","),
        ST("v"),         PT(")"),        ST("and"), ST("Uses"),   PT("("),
        ST("a"),         PT(","),        ST("v"),   PT(")"),      ST("pattern"),
        ST("a"),         PT("("),        QT("x"),   PT(","),      WT("_"),
        PT(")")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE(
    "Unit Test - QueryTokenizer - Test multiple synonyms in select clause") {
  SECTION("simple case - valid") {
    std::string queryString = "variable v; while w;\nSelect <v,w>";
    auto result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"), PT(";"), ST("while"), ST("w"), PT(";"),
        ST("Select"),   PT("<"), ST("v"), PT(","),     ST("w"), PT(">")};

    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit Test - QueryTokenizer - Test dot token") {
  SECTION("only dots") {
    std::string queryString = ".....";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {DT("."), DT("."), DT("."),
                                              DT("."), DT(".")};
    REQUIRE(result == expectedResult);
  }

  SECTION("dot + string token") {
    std::string queryString = "str1.str2.str3   .str4   .";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("str1"), DT("."), ST("str2"), DT("."),
        ST("str3"), DT("."), ST("str4"), DT(".")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit Test - QueryTokenizer - Test equal token") {
  SECTION("only equalss") {
    std::string queryString = "=====";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {ET("="), ET("="), ET("="),
                                              ET("="), ET("=")};
    REQUIRE(result == expectedResult);
  }

  SECTION("equal + string token") {
    std::string queryString = "str1=str2=str3   =str4   =";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("str1"), ET("="), ST("str2"), ET("="),
        ST("str3"), ET("="), ST("str4"), ET("=")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit Test - QueryTokenizer - Test Attribute token") {
  SECTION("single attribute token - procName") {
    std::string queryString = "procName";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {AT("procName")};
    REQUIRE(result == expectedResult);
  }

  SECTION("single attribute token - varName") {
    std::string queryString = "varName";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {AT("varName")};
    REQUIRE(result == expectedResult);
  }

  SECTION("single attribute token - value") {
    std::string queryString = "value";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {AT("value")};
    REQUIRE(result == expectedResult);
  }

  SECTION("single attribute token - stmt#") {
    std::string queryString = "stmt#";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {AT("stmt#")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit Test - QueryTokenizer - Test with clause") {
  SECTION("single attribute comparison") {
    std::string queryString =
        "variable v; while w;\nSelect v with v.varName = \" a \"";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),       PT(";"), ST("while"), ST("w"),
        PT(";"),        ST("Select"),  ST("v"), ST("with"),  ST("v"),
        DT("."),        AT("varName"), ET("="), QT("a")};
    REQUIRE(result == expectedResult);
  }

  SECTION("multiple attribute comparison") {
    std::string queryString = "variable v; while w;\nSelect v with v.varName = "
                              "\" a \" and   w.stmt#=v.value";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("variable"), ST("v"),       PT(";"),     ST("while"), ST("w"),
        PT(";"),        ST("Select"),  ST("v"),     ST("with"),  ST("v"),
        DT("."),        AT("varName"), ET("="),     QT("a"),     ST("and"),
        ST("w"),        DT("."),       AT("stmt#"), ET("="),     ST("v"),
        DT("."),        AT("value")};
    REQUIRE(result == expectedResult);
  }
}

TEST_CASE("Unit Test - QueryTokenizer - Test Multiple Clauses and Tuple") {
  SECTION("Tuple + pattern + Follows*") {
    std::string queryString = "assign a, a1; variable v; Select <a1, v> "
                              "pattern a(v, _\"y\"_) such that Follows*(a, a1)";
    std::vector<QueryToken> result = QueryTokenizer::tokenize(queryString);
    std::vector<QueryToken> expectedResult = {
        ST("assign"),   ST("a"), PT(","), ST("a1"),        PT(";"),
        ST("variable"), ST("v"), PT(";"), ST("Select"),    PT("<"),
        ST("a1"),       PT(","), ST("v"), PT(">"),         ST("pattern"),
        ST("a"),        PT("("), ST("v"), PT(","),         WT("_"),
        QT("y"),        WT("_"), PT(")"), ST("such that"), ST("Follows*"),
        PT("("),        ST("a"), PT(","), ST("a1"),        PT(")")};

    REQUIRE(result == expectedResult);
  }
}


