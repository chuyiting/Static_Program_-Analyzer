#include "catch.hpp"
#include <memory>

#include "Exceptions/TokenizerException.h"
#include "utils/TokenHandler/SourceTokens.h"
#include "utils/TokenHandler/TokenStream.h"

void testValidTokenStreamLength(std::shared_ptr<TokenStream> tokenStream,
                                int expectedLength) {
  REQUIRE(tokenStream->toParseableTokens().size() == expectedLength);
}

void testValidTokenStreamTokenValues(std::shared_ptr<TokenStream> tokenStream,
                                     std::vector<std::string> expectedValues) {
  std::vector<std::shared_ptr<ParseableToken>> tokens =
      tokenStream->toParseableTokens();
  for (int i = 0; i < tokens.size(); i++) {
    REQUIRE(tokens[i]->getValue() == expectedValues[i]);
  }
}

void testValidTokenStreamPeekNextToken(std::shared_ptr<TokenStream> tokenStream,
                                       std::string expectedValue) {
  REQUIRE(tokenStream->peekNextToken()->getValue() == expectedValue);
}

void testValidTokenStreamPeekAheadToken(
    std::shared_ptr<TokenStream> tokenStream, int additionalIncrement,
    std::string expectedValue) {
  REQUIRE(tokenStream->peekNextToken(additionalIncrement)->getValue() ==
          expectedValue);
}

void testValidTokenStreamGetNextToken(std::shared_ptr<TokenStream> tokenStream,
                                      std::string expectedValue) {
  REQUIRE(tokenStream->getNextToken()->getValue() == expectedValue);
}

void testValidTokenStreamEnds(std::shared_ptr<TokenStream> tokenStream) {
  // consume all tokens
  for (int i = 0; i < tokenStream->toParseableTokens().size(); i++) {
    tokenStream->getNextToken();
  }
  REQUIRE(tokenStream->hasNextToken() == false);
}

TEST_CASE("add_ValidTokens_NoException") {
  std::shared_ptr<TokenStream> tokenStream = std::make_shared<TokenStream>();
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "a"));
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "b"));
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "c"));

  SECTION("add_ValidTokens_ShouldReturnCorrectLength") {
    testValidTokenStreamLength(tokenStream, 3);
  }
  SECTION("add_ValidTokens_ShouldReturnCorrectTokenValues") {
    testValidTokenStreamTokenValues(tokenStream,
                                    std::vector<std::string>{"a", "b", "c"});
  }
  SECTION("add_ValidTokens_ShouldReturnCorrectPeekNextToken") {
    testValidTokenStreamPeekNextToken(tokenStream, "a");
  }
  SECTION("add_ValidTokens_ShouldReturnCorrectPeekAheadToken") {
    testValidTokenStreamPeekAheadToken(tokenStream, 1, "b");
    testValidTokenStreamPeekAheadToken(tokenStream, 2, "c");
  }
  SECTION("add_ValidTokens_ShouldReturnCorrectGetNextToken") {
    testValidTokenStreamGetNextToken(tokenStream, "a");
    testValidTokenStreamGetNextToken(tokenStream, "b");
    testValidTokenStreamGetNextToken(tokenStream, "c");
  }
  SECTION("add_ValidTokens_ShouldReturnCorrectHasEnded") {
    testValidTokenStreamEnds(tokenStream);
  }
}

TEST_CASE("add_InvalidTokens_NoException") {
  std::shared_ptr<TokenStream> tokenStream = std::make_shared<TokenStream>();
  tokenStream->addToken(std::make_shared<SourceToken>(
      SourceTokenType::INTEGER, "a")); // not a valid token
  tokenStream->addToken(std::make_shared<SourceToken>(
      SourceTokenType::LEFTCURLYBRACKET, "123")); // not a valid token
  tokenStream->addToken(std::make_shared<SourceToken>(
      SourceTokenType::PLUS, "-")); // not a valid token
  tokenStream->addToken(std::make_shared<SourceToken>(
      SourceTokenType::NAME, ")")); // not a valid token

  SECTION("add_InvalidTokens_ShouldReturnCorrectLength") {
    testValidTokenStreamLength(tokenStream, 4);
  }
  SECTION("add_InvalidTokens_ShouldReturnCorrectTokenValues") {
    testValidTokenStreamTokenValues(
        tokenStream, std::vector<std::string>{"a", "123", "-", ")"});
  }
  SECTION("add_InvalidTokens_ShouldReturnCorrectPeekNextToken") {
    testValidTokenStreamPeekNextToken(tokenStream, "a");
  }
  SECTION("add_InvalidTokens_ShouldReturnCorrectPeekAheadToken") {
    testValidTokenStreamPeekAheadToken(tokenStream, 1, "123");
    testValidTokenStreamPeekAheadToken(tokenStream, 2, "-");
    testValidTokenStreamPeekAheadToken(tokenStream, 3, ")");
  }
  SECTION("add_InvalidTokens_ShouldReturnCorrectGetNextToken") {
    testValidTokenStreamGetNextToken(tokenStream, "a");
    testValidTokenStreamGetNextToken(tokenStream, "123");
    testValidTokenStreamGetNextToken(tokenStream, "-");
    testValidTokenStreamGetNextToken(tokenStream, ")");
  }
  SECTION("add_ValidTokens_ShouldReturnCorrectHasEnded") {
    testValidTokenStreamEnds(tokenStream);
  }
}

TEST_CASE("saveAndRestore_ReturnsCorrectToken") {
  std::shared_ptr<TokenStream> tokenStream = std::make_shared<TokenStream>();
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "a"));
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "b"));
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "c"));
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "d"));
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "e"));
  tokenStream->addToken(
      std::make_shared<SourceToken>(SourceTokenType::NAME, "f"));

  // Consumes one token
  tokenStream->getNextToken();

  // Saves the state of the tokenStream to be at the second token
  tokenStream->saveIndex();

  // Consumes a random amount of tokens from 1-3
  int randomAmount = rand() % 3 + 1;
  for (int i = 0; i < randomAmount; i++) {
    tokenStream->getNextToken();
  }

  // Restores the state of the tokenStream to be at the second token
  tokenStream->restoreIndex();

  // Checks the rest of the tokens
  testValidTokenStreamGetNextToken(tokenStream, "b");
  testValidTokenStreamGetNextToken(tokenStream, "c");
  testValidTokenStreamGetNextToken(tokenStream, "d");
  testValidTokenStreamGetNextToken(tokenStream, "e");
  testValidTokenStreamGetNextToken(tokenStream, "f");

  // Checks that the tokenStream has ended
  testValidTokenStreamEnds(tokenStream);
}
