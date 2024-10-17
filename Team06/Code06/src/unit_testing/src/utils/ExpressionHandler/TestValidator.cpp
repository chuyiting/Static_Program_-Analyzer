#include "catch.hpp"
#include <memory>

#include <SP/Tokenizer/Tokenizer.h>
#include <utils/ExpressionHandler/Validator.h>
#include <utils/TokenHandler/TokenStream.h>

void testValidCondExpr(std::string sourceCode) {
  std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>();
  std::shared_ptr<TokenStream> tokenStream = tokenizer->process(sourceCode);
  std::shared_ptr<Validator> validator = std::make_shared<Validator>(
      tokenStream, ValidatedExpressionType::CONDITIONAL_EXPRESSION);
  REQUIRE_NOTHROW(validator->validate());
}

void testInvalidCondExpr(std::string sourceCode) {
  std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>();
  std::shared_ptr<TokenStream> tokenStream = tokenizer->process(sourceCode);
  std::shared_ptr<Validator> validator = std::make_shared<Validator>(
      tokenStream, ValidatedExpressionType::CONDITIONAL_EXPRESSION);
  REQUIRE_THROWS(validator->validate());
}

void testValidExpr(std::string sourceCode) {
  std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>();
  std::shared_ptr<TokenStream> tokenStream = tokenizer->process(sourceCode);
  std::shared_ptr<Validator> validator = std::make_shared<Validator>(
      tokenStream, ValidatedExpressionType::EXPRESSION);
  REQUIRE_NOTHROW(validator->validate());
}

void testInvalidExpr(std::string sourceCode) {
  std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>();
  std::shared_ptr<TokenStream> tokenStream = tokenizer->process(sourceCode);
  std::shared_ptr<Validator> validator = std::make_shared<Validator>(
      tokenStream, ValidatedExpressionType::EXPRESSION);
  REQUIRE_THROWS(validator->validate());
}

TEST_CASE("validate_CondExprRelExpr") {
  SECTION("validate_VarNameGreaterThanBracketedTerms") {
    testValidCondExpr("varName1 > ((a + b) * 10) / 2");
  }
  SECTION("validate_ConstValueGreaterThanOrEqualManyTerms") {
    testValidCondExpr("1000 >= x + y * z");
  }
  SECTION("validate_BracketedTermsLessThanOrEqualVarName") {
    testValidCondExpr("((a + b) * 10) / 2 <= varName1");
  }
  SECTION("validate_ManyTermsLessThanConstValue") {
    testValidCondExpr("x + y * z < 1000");
  }
  SECTION("validate_VarNameEqualBracketedTerms") {
    testValidCondExpr("varName1 == ((a + b) * 10) / 2");
  }
  SECTION("validate_ConstValueNotEqualManyTerms") {
    testValidCondExpr("1000 != x + y * z");
  }
}

TEST_CASE("validate_CondExprNot") {
  SECTION("validate_NotRelExpr") {
    testValidCondExpr("!(varName1 > ((a + b) * 10) / 2)");
  }
  SECTION("validate_NotNot_") {
    testValidCondExpr("!(!(varName1 > ((a + b) * 10) / 2))");
  }
  SECTION("validate_NotCondExprs") {
    testValidCondExpr("!((varName1 > ((a + b) * 10) / 2) && (varName1 > ((a + "
                      "b) * 10) / 2))");
  }
}

TEST_CASE("validate_CondExprs") {
  SECTION("validate_RelExprAndCondExprs") {
    testValidCondExpr(
        "(varName1 > ((a + b) * 10) / 2) && "
        "((varName1 > ((a + b) * 10) / 2) || (varName1 > ((a + b) * 10) / 2))");
  }
  SECTION("validate_NotOrNot") {
    testValidCondExpr("(!(varName1 > ((a + b) * 10) / 2)) || "
                      "(!(varName1 > ((a + b) * 10) / 2))");
  }
  SECTION("validate_CondExprsAndRelExpr") {
    testValidCondExpr("((varName1 > ((a + b) * 10) / 2) || (varName1 > ((a + "
                      "b) * 10) / 2)) && "
                      "(varName1 > ((a + b) * 10) / 2)");
  }
}

TEST_CASE("validate_ExprOne") {
  SECTION("validate_OneTerm") { testValidExpr("a"); }
  SECTION("validate_OneBracketWithOneTerm") { testValidExpr("(a)"); }
  SECTION("validate_OneBracketWithTwoTerms") { testValidExpr("(a + b)"); }
}

TEST_CASE("validate_ExprMultiple") {
  SECTION("validate_OneTermPlusOneBracketWithOneTerm") {
    testValidExpr("a + (a)");
  }
  SECTION("validate_OneBracketWithOneTermMinusOneTerm") {
    testValidExpr("(a) - a");
  }
  SECTION("validate_OneBracketWithTwoTermsMultiplyOneBracketWithTwoTerms") {
    testValidExpr("(a + b) * (a + b)");
  }
  SECTION("validate_OneTermDivideOneBracketWithOneTerm") {
    testValidExpr("a / (a)");
  }
  SECTION("validate_OneBracketWithOneTermModuloOneTerm") {
    testValidExpr("(a) % a");
  }
}
