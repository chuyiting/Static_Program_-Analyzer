#include <memory>
#include <vector>

#include "catch.hpp"

#include "Exceptions/TokenizerException.h"
#include "SP/Tokenizer/Tokenizer.h"
#include "utils/TokenHandler/TokenStream.h"

void testValidTokenizerInput(std::string value) {
  Tokenizer tokenizer = Tokenizer();
  REQUIRE_NOTHROW(tokenizer.process(value));
}

void testInvalidTokenizerInput(std::string value) {
  Tokenizer tokenizer = Tokenizer();
  REQUIRE_THROWS_WITH(tokenizer.process(value), TokenizerException().what());
}

void testReturnRightTokenStream(std::string value,
                                TokenStream expectedTokenStream) {
  Tokenizer tokenizer = Tokenizer();
  std::shared_ptr<TokenStream> tokenStream = tokenizer.process(value);
  REQUIRE(*tokenStream == expectedTokenStream);
}

TEST_CASE("process_SingleInput") {
  SECTION("process_EmptyInput_ShouldNotThrowException") {
    testValidTokenizerInput("");
  }
  SECTION("process_InvalidLexicalToken_ShouldThrowException") {
    testInvalidTokenizerInput("@");
  }
}

TEST_CASE("process_LongInput_NoVariation") {

  std::vector<std::shared_ptr<SourceToken>> abc = {
      std::make_shared<SourceToken>(SourceTokenType::NAME, "a"),
      std::make_shared<SourceToken>(SourceTokenType::NAME, "b"),
      std::make_shared<SourceToken>(SourceTokenType::NAME, "c")};

  SECTION("process_InputWithSpaces_ShoudReturnCorrectTokenStream") {
    testReturnRightTokenStream("a b c", TokenStream(abc));
  }

  SECTION("process_InputWithSpacesAndTabs_ShoudReturnCorrectTokenStream") {
    testReturnRightTokenStream("a \t b \t c", TokenStream(abc));
  }
  SECTION("process_InputWithSpacesAndTabsAndNewLines_"
          "ShoudReturnCorrectTokenStream") {
    testReturnRightTokenStream("a  \t\n  b  \t\n  c", TokenStream(abc));
  }
}

TEST_CASE("process_MixedTokens") {

  SECTION("process_InputWithNamesIntegersOperatorsSpaces_"
          "ShouldReturnCorrectTokenStream") {

    std::vector<std::shared_ptr<SourceToken>> input = {
        std::make_shared<SourceToken>(SourceTokenType::NAME, "a"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "1"),
        std::make_shared<SourceToken>(SourceTokenType::PLUS, "+"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "b"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "2"),
        std::make_shared<SourceToken>(SourceTokenType::MINUS, "-"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "c"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "3"),
        std::make_shared<SourceToken>(SourceTokenType::MULTIPLY, "*"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "d"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "4"),
        std::make_shared<SourceToken>(SourceTokenType::DIVIDE, "/"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "e"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "5")};

    testReturnRightTokenStream("a 1 + b 2 - c 3 * d 4 / e 5", input);
  }

  SECTION(
      "process_IfWithConditionalExpressions_ShouldReturnCorrectTokenStream") {

    std::vector<std::shared_ptr<SourceToken>> input = {
        std::make_shared<SourceToken>(SourceTokenType::NAME, "if"),
        std::make_shared<SourceToken>(SourceTokenType::LEFTROUNDBRACKET, "("),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "a"),
        std::make_shared<SourceToken>(SourceTokenType::LESSTHAN, "<"),
        std::make_shared<SourceToken>(SourceTokenType::LEFTROUNDBRACKET, "("),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "var"),
        std::make_shared<SourceToken>(SourceTokenType::PLUS, "+"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "2"),
        std::make_shared<SourceToken>(SourceTokenType::RIGHTROUNDBRACKET, ")"),
        std::make_shared<SourceToken>(SourceTokenType::MODULO, "%"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "3"),
        std::make_shared<SourceToken>(SourceTokenType::RIGHTROUNDBRACKET, ")"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "then"),
        std::make_shared<SourceToken>(SourceTokenType::LEFTCURLYBRACKET, "{"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "read"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "x"),
        std::make_shared<SourceToken>(SourceTokenType::SEMICOLON, ";"),
        std::make_shared<SourceToken>(SourceTokenType::RIGHTCURLYBRACKET, "}"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "else"),
        std::make_shared<SourceToken>(SourceTokenType::LEFTCURLYBRACKET, "{"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "z"),
        std::make_shared<SourceToken>(SourceTokenType::ASSIGNMENT, "="),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "y"),
        std::make_shared<SourceToken>(SourceTokenType::MULTIPLY, "*"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "1234"),
        std::make_shared<SourceToken>(SourceTokenType::PLUS, "+"),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "2"),
        std::make_shared<SourceToken>(SourceTokenType::SEMICOLON, ";"),
        std::make_shared<SourceToken>(SourceTokenType::RIGHTCURLYBRACKET, "}")};

    testReturnRightTokenStream(
        "if (a < (var + 2) % 3) then { read x; } else { z = y * 1234 + 2; }",
        input);
  }
}

TEST_CASE("process_InvalidSourceProgram_ValidInput") {

  SECTION(
      "process_InputWithInvalidMathExpression_ShoudReturnCorrectTokenStream") {

    std::vector<std::shared_ptr<SourceToken>> input = {
        std::make_shared<SourceToken>(SourceTokenType::NAME, "a"),
        std::make_shared<SourceToken>(SourceTokenType::PLUS, "+"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "b"),
        std::make_shared<SourceToken>(SourceTokenType::MULTIPLY, "*"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "c"),
        std::make_shared<SourceToken>(SourceTokenType::ASSIGNMENT, "="),
        std::make_shared<SourceToken>(SourceTokenType::INTEGER, "1")};

    testReturnRightTokenStream("a + b * c = 1", input);
  }

  SECTION(
      "process_InputWithIncompleteProcedure_ShoudReturnCorrectTokenStream") {

    std::vector<std::shared_ptr<SourceToken>> input = {
        std::make_shared<SourceToken>(SourceTokenType::NAME, "procedure"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "Bedok"),
        std::make_shared<SourceToken>(SourceTokenType::LEFTCURLYBRACKET, "{"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "read"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "x"),
        std::make_shared<SourceToken>(SourceTokenType::SEMICOLON, ";"),
    };

    testReturnRightTokenStream("procedure Bedok { read x;", input);
  }

  SECTION("process_InputWithMathExpression_ShoudReturnCorrectTokenStream") {

    std::vector<std::shared_ptr<SourceToken>> input = {
        std::make_shared<SourceToken>(SourceTokenType::LEFTCURLYBRACKET, "{"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "a"),
        std::make_shared<SourceToken>(SourceTokenType::ASSIGNMENT, "="),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "b"),
        std::make_shared<SourceToken>(SourceTokenType::PLUS, "+"),
        std::make_shared<SourceToken>(SourceTokenType::NAME, "c"),
        std::make_shared<SourceToken>(SourceTokenType::SEMICOLON, ";"),
        std::make_shared<SourceToken>(SourceTokenType::RIGHTCURLYBRACKET, "}")};

    testReturnRightTokenStream("{ a = b + c; }", input);
  }
}
