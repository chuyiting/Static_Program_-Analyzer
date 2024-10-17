#include "catch.hpp"
#include <memory>

#include "Exceptions/TokenizerException.h"
#include "utils/TokenHandler/SourceTokens.h"

void testValidToken(SourceTokenType tokenType, std::string value) {
  std::shared_ptr<SourceToken> actualToken = SourceToken::parse(value);
  std::shared_ptr<SourceToken> expectedToken =
      std::make_shared<SourceToken>(tokenType, value);
  REQUIRE(actualToken->isEqual(expectedToken));
}

void testInvalidToken(std::string value) {
  REQUIRE_THROWS_WITH(SourceToken::parse(value), TokenizerException().what());
}

void testEqualPrecedence(SourceTokenType tokenType1,
                         SourceTokenType tokenType2) {
  REQUIRE(SourceToken(tokenType1, "DUMMY").getOperatorPrecedence() ==
          SourceToken(tokenType2, "DUMMY").getOperatorPrecedence());
}

void testHigherPrecedence(SourceTokenType tokenType1,
                          SourceTokenType tokenType2) {
  REQUIRE(SourceToken(tokenType1, "DUMMY").getOperatorPrecedence() >
          SourceToken(tokenType2, "DUMMY").getOperatorPrecedence());
}

void testLowerPrecedence(SourceTokenType tokenType1,
                         SourceTokenType tokenType2) {
  REQUIRE(SourceToken(tokenType1, "DUMMY").getOperatorPrecedence() <
          SourceToken(tokenType2, "DUMMY").getOperatorPrecedence());
}

// LEXICAL TOKENS: LETTER | DIGIT | NZDIGIT | NAME | INTEGER
TEST_CASE("parse_LexicalToken") {
  SECTION("parse_EmptyToken_ShouldThrowException") { testInvalidToken(""); }
  SECTION("parse_InvalidLexicalToken_ShouldThrowException") {
    testInvalidToken("@");
  }
}

// LETTER: A - Z | a - z
TEST_CASE("parse_LexicalTokenLetter") {
  SECTION("parse_CapitalLetterMin_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "A");
  }
  SECTION("parse_CapitalLetterAboveMin_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "B");
  }
  SECTION("parse_CapitalLetterNominal_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "M");
  }
  SECTION("parse_CapitalLetterBelowMax_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "Y");
  }
  SECTION("parse_CapitalLetterMax_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "Z");
  }
  SECTION("parse_SmallLetterMin_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "a");
  }
  SECTION("parse_SmallLetterAboveMin_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "b");
  }
  SECTION("parse_SmallLetterNominal_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "m");
  }
  SECTION("parse_SmallLetterBelowMax_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "y");
  }
  SECTION("parse_SmallLetterMax_ReturnsSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "z");
  }
}

// DIGIT: 0-9
TEST_CASE("parse_LexicalTokenDigit") {
  SECTION("parse_DigitMin_ReturnsSourceTokenInteger") {
    testValidToken(SourceTokenType::INTEGER, "0");
  }
  SECTION("parse_DigitAboveMin_ReturnsSourceTokenInteger") {
    testValidToken(SourceTokenType::INTEGER, "1");
  }
  SECTION("parse_DigitNominal_ReturnsSourceTokenInteger") {
    testValidToken(SourceTokenType::INTEGER, "4");
  }
  SECTION("parse_DigitBelowMax_ReturnsSourceTokenInteger") {
    testValidToken(SourceTokenType::INTEGER, "8");
  }
  SECTION("parse_DigitMax_ReturnsSourceTokenInteger") {
    testValidToken(SourceTokenType::INTEGER, "9");
  }
}

// NAME: LETTER(LETTER | DIGIT)*
TEST_CASE("parse_LexicalTokenName") {
  SECTION("parse_LetterLetters_ShouldReturnSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "abc");
  }
  SECTION("parse_LetterDigits_ShouldReturnSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "a123");
  }
  SECTION("parse_LetterMixed_ShouldReturnSourceTokenName") {
    testValidToken(SourceTokenType::NAME, "a1b2c3");
  }
  SECTION("parse_DigitLetters_ShouldThrowException") {
    testInvalidToken("1abc");
  }
}

// INTEGER : 0 | NZDIGIT ( DIGIT )*
TEST_CASE("parse_LexicalTokenInteger") {
  SECTION("parse_NonLeadingZeroConstant_ShouldReturnSourceTokenInteger") {
    testValidToken(SourceTokenType::INTEGER, "123");
  }
  SECTION("parse_LeadingZeroConstant_ShouldThrowException") {
    testInvalidToken("0123");
  }
  SECTION("parse_Zero_ShouldReturnSourceTokenInteger") {
    testValidToken(SourceTokenType::INTEGER, "0");
  }
  SECTION("parse_ZeroWithLeadingZero_ShouldThrowException") {
    testInvalidToken("00");
  }
  SECTION("parse_ZeroWithAlphabet_ShouldThrowException") {
    testInvalidToken("0a");
  }
}

TEST_CASE("parse_SourceToken") {
  SECTION("parse_Whitespace_ShouldReturnSourceTokenWhitespace") {
    testValidToken(SourceTokenType::WHITESPACE, " ");
  }
  SECTION("parse_LeftRoundBracket_ShouldReturnSourceTokenLeftRoundBracket") {
    testValidToken(SourceTokenType::LEFTROUNDBRACKET, "(");
  }
  SECTION("parse_RightRoundBracket_ShouldReturnSourceTokenRightRoundBracket") {
    testValidToken(SourceTokenType::RIGHTROUNDBRACKET, ")");
  }
  SECTION("parse_Semicolon_ShouldReturnSourceTokenSemicolon") {
    testValidToken(SourceTokenType::SEMICOLON, ";");
  }
  SECTION("parse_LeftCurlyBracket_ShouldReturnSourceTokenLeftCurlyBracket") {
    testValidToken(SourceTokenType::LEFTCURLYBRACKET, "{");
  }
  SECTION("parse_RightCurlyBracket_ShouldReturnSourceTokenRightCurlyBracket") {
    testValidToken(SourceTokenType::RIGHTCURLYBRACKET, "}");
  }
  SECTION("parse_Plus_ShouldReturnSourceTokenPlus") {
    testValidToken(SourceTokenType::PLUS, "+");
  }
  SECTION("parse_Minus_ShouldReturnSourceTokenMinus") {
    testValidToken(SourceTokenType::MINUS, "-");
  }
  SECTION("parse_Multiply_ShouldReturnSourceTokenMultiply") {
    testValidToken(SourceTokenType::MULTIPLY, "*");
  }
  SECTION("parse_Divide_ShouldReturnSourceTokenDivide") {
    testValidToken(SourceTokenType::DIVIDE, "/");
  }
  SECTION("parse_Modulo_ShouldReturnSourceTokenModulo") {
    testValidToken(SourceTokenType::MODULO, "%");
  }
  SECTION("parse_AndDouble_ShouldReturnSourceTokenAnd") {
    testValidToken(SourceTokenType::AND, "&&");
  }
  SECTION("parse_AndSingle_ShouldThrowException") { testInvalidToken("&"); }
  SECTION("parse_OrDouble_ShouldReturnSourceTokenOr") {
    testValidToken(SourceTokenType::OR, "||");
  }
  SECTION("parse_OrSingle_ShouldThrowException") { testInvalidToken("|"); }
  SECTION("parse_EqualDouble_ShouldReturnSourceTokenEqual") {
    testValidToken(SourceTokenType::EQUAL, "==");
  }
  SECTION("parse_EqualSingle_ShouldReturnSourceTokenAssignment") {
    testValidToken(SourceTokenType::ASSIGNMENT, "=");
  }
  SECTION("parse_NotEqual_ShouldReturnSourceTokenNotEqual") {
    testValidToken(SourceTokenType::NOTEQUAL, "!=");
  }
  SECTION("parse_Not_ShouldReturnSourceTokenNot") {
    testValidToken(SourceTokenType::NOT, "!");
  }
  SECTION(
      "parse_GreaterThanOrEqual_ShouldReturnSourceTokenGreaterThanOrEqual") {
    testValidToken(SourceTokenType::GREATERTHANOREQUAL, ">=");
  }
  SECTION("parse_GreaterThan_ShouldReturnSourceTokenGreaterThan") {
    testValidToken(SourceTokenType::GREATERTHAN, ">");
  }
  SECTION("parse_LessThanOrEqual_ShouldReturnSourceTokenLessThanOrEqual") {
    testValidToken(SourceTokenType::LESSTHANOREQUAL, "<=");
  }
  SECTION("parse_LessThan_ShouldReturnSourceTokenLessThan") {
    testValidToken(SourceTokenType::LESSTHAN, "<");
  }
}

TEST_CASE("parse_SourceToken_OperatorPrecedence") {
  SECTION("parse_MultiplyDivideModulo_ShouldReturnEqualPrecedence") {
    testEqualPrecedence(SourceTokenType::MULTIPLY, SourceTokenType::DIVIDE);
    testEqualPrecedence(SourceTokenType::DIVIDE, SourceTokenType::MULTIPLY);
    testEqualPrecedence(SourceTokenType::DIVIDE, SourceTokenType::MODULO);
    testEqualPrecedence(SourceTokenType::MODULO, SourceTokenType::DIVIDE);
    testEqualPrecedence(SourceTokenType::MODULO, SourceTokenType::MULTIPLY);
    testEqualPrecedence(SourceTokenType::MULTIPLY, SourceTokenType::MODULO);
  }
  SECTION("parse_PlusMinus_ShouldReturnEqualPrecedence") {
    testEqualPrecedence(SourceTokenType::PLUS, SourceTokenType::MINUS);
    testEqualPrecedence(SourceTokenType::MINUS, SourceTokenType::PLUS);
  }
  SECTION("parse_MultiplyDivideModuloHigherThanPlusMinus_"
          "ShouldReturnHigherPrecedence") {
    testHigherPrecedence(SourceTokenType::MULTIPLY, SourceTokenType::PLUS);
    testHigherPrecedence(SourceTokenType::DIVIDE, SourceTokenType::PLUS);
    testHigherPrecedence(SourceTokenType::MODULO, SourceTokenType::PLUS);
    testHigherPrecedence(SourceTokenType::MULTIPLY, SourceTokenType::MINUS);
    testHigherPrecedence(SourceTokenType::DIVIDE, SourceTokenType::MINUS);
    testHigherPrecedence(SourceTokenType::MODULO, SourceTokenType::MINUS);
  }
  SECTION("parse_PlusMinusLowerThanMultiplyDivideModulo_"
          "ShouldReturnLowerPrecedence") {
    testLowerPrecedence(SourceTokenType::PLUS, SourceTokenType::MULTIPLY);
    testLowerPrecedence(SourceTokenType::PLUS, SourceTokenType::DIVIDE);
    testLowerPrecedence(SourceTokenType::PLUS, SourceTokenType::MODULO);
    testLowerPrecedence(SourceTokenType::MINUS, SourceTokenType::MULTIPLY);
    testLowerPrecedence(SourceTokenType::MINUS, SourceTokenType::DIVIDE);
    testLowerPrecedence(SourceTokenType::MINUS, SourceTokenType::MODULO);
  }
}
