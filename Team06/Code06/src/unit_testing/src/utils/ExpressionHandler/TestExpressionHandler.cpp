//
// Created by Chong Jun Wei on 14/10/22.
//

#include "PKB/type/pkb_types.h"
#include "catch.hpp"
#include "utils/ExpressionHandler/ExpressionHandler.h"

using namespace pkb_types;

// tests for infix/postfix conversion
// expression with brackets
TEST_CASE("expr_1") {
  // (x1 + (y))
  Expression e = "(x1 + (y))";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"(", "x1", "+", "(", "y", ")", ")"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = x1 y +
  std::vector<std::string> expectedPostfix = {"x1", "y", "+"};
  REQUIRE(storedPostfix == expectedPostfix);
}

TEST_CASE("expr_1 but many brackets") {
  // (x1 + (y))
  Expression e = "(x1 + ((((((y)))))))";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"(", "x1", "+", "(", "(", "(",
                                            "(", "(",  "(", "y", ")", ")",
                                            ")", ")",  ")", ")", ")"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = x1 y +
  std::vector<std::string> expectedPostfix = {"x1", "y", "+"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// brackets with whitespace
TEST_CASE("expr_2") {
  // var + 3 * foo * (s + 3 )
  Expression e = "var + 3 * foo * (s                              + 3 )";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"var", "+", "3", "*", "foo", "*",
                                            "(",   "s", "+", "3", ")"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = var 3 foo * s 3 + * +
  std::vector<std::string> expectedPostfix = {"var", "3", "foo", "*", "s",
                                              "3",   "+", "*",   "+"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// multiplication
TEST_CASE("expr_3") {
  // a*b*c*d
  Expression e = "a*b*c*d";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"a", "*", "b", "*", "c", "*", "d"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b * c * d *
  std::vector<std::string> expectedPostfix = {"a", "b", "*", "c",
                                              "*", "d", "*"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// multiplication with brackets
TEST_CASE("expr_4") {
  // (a * b * e * (3 * s) * 1) * c
  Expression e = "(a * b * e * (3 * s) * 1) * c";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"(", "a", "*", "b", "*", "e",
                                            "*", "(", "3", "*", "s", ")",
                                            "*", "1", ")", "*", "c"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b * e * 3 s * * 1 * c *
  std::vector<std::string> expectedPostfix = {"a", "b", "*", "e", "*", "3", "s",
                                              "*", "*", "1", "*", "c", "*"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// division
TEST_CASE("expr_5") {
  // a/b/c/d
  Expression e = "a/b/c/d";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"a", "/", "b", "/", "c", "/", "d"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b / c / d /
  std::vector<std::string> expectedPostfix = {"a", "b", "/", "c",
                                              "/", "d", "/"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// division with brackets
TEST_CASE("expr_6") {
  // (a / b / e / (3 / s) / 1) / c
  Expression e = "(a / b / e / (3 / s) / 1) / c";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"(", "a", "/", "b", "/", "e",
                                            "/", "(", "3", "/", "s", ")",
                                            "/", "1", ")", "/", "c"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b / e / 3 s / / 1 / c /
  std::vector<std::string> expectedPostfix = {"a", "b", "/", "e", "/", "3", "s",
                                              "/", "/", "1", "/", "c", "/"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// addition
TEST_CASE("expr_7") {
  // a+b+c+d
  Expression e = "a+b+c+d";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"a", "+", "b", "+", "c", "+", "d"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b + c + d +
  std::vector<std::string> expectedPostfix = {"a", "b", "+", "c",
                                              "+", "d", "+"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// addition with brackets, variables and constant of >1 length
TEST_CASE("expr_8") {
  // (a1 + b3 + e + (3 + s) + 12) + c
  Expression e = "(a1 + b3 + e + (3 + s) + 12) + c";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"(", "a1", "+", "b3", "+", "e",
                                            "+", "(",  "3", "+",  "s", ")",
                                            "+", "12", ")", "+",  "c"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a1 b3 + e + 3 s + + 12 + c +
  std::vector<std::string> expectedPostfix = {
      "a1", "b3", "+", "e", "+", "3", "s", "+", "+", "12", "+", "c", "+"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// subtraction
TEST_CASE("expr_9") {
  // a-b-c-d
  Expression e = "a-b-c-d";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"a", "-", "b", "-", "c", "-", "d"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b - c - d -
  std::vector<std::string> expectedPostfix = {"a", "b", "-", "c",
                                              "-", "d", "-"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// subtraction with brackets
TEST_CASE("expr_10") {
  // (a - b - e - (3 - s) - 1) - c
  Expression e = "(a - b - e - (3 - s) - 1) - c";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"(", "a", "-", "b", "-", "e",
                                            "-", "(", "3", "-", "s", ")",
                                            "-", "1", ")", "-", "c"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b - e - 3 s - - 1 - c -
  std::vector<std::string> expectedPostfix = {"a", "b", "-", "e", "-", "3", "s",
                                              "-", "-", "1", "-", "c", "-"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// mix of operators
TEST_CASE("expr_11") {
  // a/b*c+d
  Expression e = "a/b*c+d";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();

  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"a", "/", "b", "*", "c", "+", "d"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b / c * d +
  std::vector<std::string> expectedPostfix = {"a", "b", "/", "c",
                                              "*", "d", "+"};
  REQUIRE(storedPostfix == expectedPostfix);
}

// mix of operators
TEST_CASE("expr_12") {
  // a/(b*c)+d
  Expression e = "a/(b*c)+d";
  ExpressionHandler eh = ExpressionHandler(ExpressionType::EXPRESSION, e);
  eh.parse();
  std::vector<std::string> storedInfix = eh.getInfixNotation();
  std::vector<std::string> storedPostfix = eh.getPostfixNotation();

  std::vector<std::string> expectedInfix = {"a", "/", "(", "b", "*",
                                            "c", ")", "+", "d"};
  REQUIRE(storedInfix == expectedInfix);

  // postfix = a b c * / d +
  std::vector<std::string> expectedPostfix = {"a", "b", "c", "*",
                                              "/", "d", "+"};
  REQUIRE(storedPostfix == expectedPostfix);
}