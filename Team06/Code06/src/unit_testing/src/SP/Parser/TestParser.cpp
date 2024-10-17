#include "catch.hpp"

#include "SP/Parser/Parser.h"
#include "SP/Tokenizer/Tokenizer.h"
#include "utils/TokenHandler/TokenStream.h"

void testValidCode(std::string sourceCode) {
  std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>();
  std::shared_ptr<TokenStream> tokenStream = tokenizer->process(sourceCode);
  std::shared_ptr<Parser> parser = std::make_shared<Parser>();
  REQUIRE_NOTHROW(parser->process(tokenStream));
}

void testInvalidCode(std::string sourceCode) {
  std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>();
  std::shared_ptr<TokenStream> tokenStream = tokenizer->process(sourceCode);
  std::shared_ptr<Parser> parser = std::make_shared<Parser>();
  REQUIRE_THROWS(parser->process(tokenStream));
}

TEST_CASE("process_Program") {
  SECTION("process_ProcedureZero_ShouldThrowException") { testInvalidCode(""); }
  SECTION("process_ProcedureOne_ShouldReturnProgram") {
    testValidCode("procedure foo { read x; }");
  }
  SECTION("process_ProcedureTwo_ShouldReturnProgram") {
    testValidCode("procedure foo { read x; }"
                  "procedure bar{ read y; }");
  }
  SECTION("process_InvalidProgramSyntax_ShouldThrowException") {
    testInvalidCode("notprocedure"
                    "procedure foo { read x; }");
    testInvalidCode("procedure foo { read x; }"
                    "notprocedure");
  }
  SECTION("process_DuplicateProcName_ShouldThrowException") {
    testInvalidCode("procedure foo { read x; }"
                    "procedure foo { read y; }");
  }
  SECTION("process_ProcDoesNotExist_ShouldThrowException") {
    testInvalidCode("procedure foo { call noSuchProc; }");
  }
  SECTION("process_RecursiveProcCall_ShouldThrowException") {
    testInvalidCode("procedure foo { call foo; }");
  }
  SECTION("process_CyclicProcCall_ShouldThrowException") {
    testInvalidCode("procedure foo { call bar; }"
                    "procedure bar { call baz; }"
                    "procedure baz { call foo; }");
  }
}

TEST_CASE("process_Procedure") {
  SECTION("process_ValidProcedureName_ShouldReturnProgram") {
    testValidCode("procedure procedure { read x; }");
    testValidCode("procedure foo1 { read x; }");
  }
  SECTION("process_MissingComponent_ShouldThrowException") {
    testInvalidCode("foo { read x; }");
    testInvalidCode("procedure { read x; }");
    testInvalidCode("procedure foo read x; }");
    testInvalidCode("procedure foo { }");
    testInvalidCode("procedure foo { read x;");
  }
  SECTION("process_InvalidProcedureKeyword_ShouldThrowException") {
    testInvalidCode("proc foo { read x; }");
  }
  SECTION("process_InvalidProcedureName_ShouldThrowException") {
    testInvalidCode("procedure 1 { read x; }");
  }
}

TEST_CASE("process_StmtLst") {
  SECTION("process_StmtZero_ShouldThrowException") {
    testInvalidCode("procedure foo {}");
  }
  SECTION("process_StmtOne_ShouldReturnProgram") {
    testValidCode("procedure foo {"
                  "read one;"
                  "}");
  }
  SECTION("process_StmtTwo_ShouldReturnProgram") {
    testValidCode("procedure foo {"
                  "read one;"
                  "read two;"
                  "}");
  }
  SECTION("process_InvalidStmtLst_ShouldThrowException") {
    testInvalidCode("procedure foo {"
                    "invalidstmt"
                    "read one;"
                    "}");
    testInvalidCode("procedure foo {"
                    "read one;"
                    "invalidstmt"
                    "}");
  }
}

TEST_CASE("process_Stmt") {
  SECTION("process_InvalidType_ShouldThrowException") {
    testInvalidCode("procedure foo { stmt x; }");
    testInvalidCode("procedure foo { procedure bar { print y; } }");
  }
}

TEST_CASE("process_Read") {
  SECTION("process_ValidReadVariableName_ShouldReturnProgram") {
    testValidCode("procedure foo { read read; }");
    testValidCode("procedure foo { read foo; }");
    testValidCode("procedure foo { read x1; }");
  }
  SECTION("process_MissingComponent_ShouldThrowException") {
    testInvalidCode("procedure foo { x; }");
    testInvalidCode("procedure foo { read; }");
    testInvalidCode("procedure foo { read x }");
  }
  SECTION("process_InvalidReadKeyword_ShouldThrowException") {
    testInvalidCode("procedure foo { r x; }");
  }
  SECTION("process_InvalidReadVariableName_ShouldThrowException") {
    testInvalidCode("procedure foo { read 1; }");
  }
}

TEST_CASE("process_Print") {
  SECTION("process_ValidPrintVariableName_ShouldReturnProgram") {
    testValidCode("procedure foo { print print; }");
    testValidCode("procedure foo { print foo; }");
    testValidCode("procedure foo { print y1; }");
  }
  SECTION("process_MissingComponent_ShouldThrowException") {
    testInvalidCode("procedure foo { y; }");
    testInvalidCode("procedure foo { print; }");
    testInvalidCode("procedure foo { print y }");
  }
  SECTION("process_InvalidPrintKeyword_ShouldThrowException") {
    testInvalidCode("procedure foo { p y; }");
  }
  SECTION("process_InvalidPrintVariableName_ShouldThrowException") {
    testInvalidCode("procedure foo { print 1; }");
  }
}

TEST_CASE("process_Call") {
  SECTION("process_ValidCallVariableName_ShouldReturnProgram") {
    testValidCode("procedure foo { call call; } procedure call { read x; } ");
    testValidCode("procedure foo { call foo1; } procedure foo1 { read x; }");
    testValidCode("procedure foo { call read; } procedure read { read x; }");
  }
  SECTION("process_MissingComponent_ShouldThrowException") {
    testInvalidCode("procedure foo { bar; }");
    testInvalidCode("procedure foo { call; }");
    testInvalidCode("procedure foo { call bar }");
  }
  SECTION("process_InvalidCallKeyword_ShouldThrowException") {
    testInvalidCode("procedure foo { c bar; }");
  }
  SECTION("process_InvalidProcedureName_ShouldThrowException") {
    testInvalidCode("procedure foo { call 1; }");
  }
}

TEST_CASE("process_While") {
  SECTION("process_While_ShouldReturnProgram") {
    testValidCode("procedure foo { while (x == 0) { read x; } }");
    testValidCode("procedure foo { while (x == y + (1 * z)) { read x; } }");
    testValidCode("procedure foo { while (((a+b) == 0) && (!(((c + (d/e)) == "
                  "1) || (f >= (g+h*i))))) { read x; } }");
  }
  SECTION("process_NestedWhile_ShouldReturnProgram") {
    testValidCode(
        "procedure foo { while (x == 0) { while (x == 0) { read x; } } }");
  }
  SECTION("process_MissingComponent_ShouldThrowException") {
    testInvalidCode("procedure foo { (x == 0) { read x; } }");
    testInvalidCode("procedure foo { while x == 0) { read x; } }");
    testInvalidCode("procedure foo { while () { read x; } }");
    testInvalidCode("procedure foo { while (x == 0 { read x; } }");
    testInvalidCode("procedure foo { while (x == 0) read x; } }");
    testInvalidCode("procedure foo { while (x == 0) { } }");
    testInvalidCode("procedure foo { while (x == 0) { read x; }");
  }
  SECTION("process_InvalidWhileKeyword_ShouldThrowException") {
    testInvalidCode("procedure foo { w (x == 0) { read x; } }");
  }
  SECTION("process_InvalidCondExpr_ShouldThrowException") {
    testInvalidCode("procedure foo { while (x) { read x; } }");
  }
}

TEST_CASE("process_If") {
  SECTION("process_If_ShouldReturnProgram") {
    testValidCode(
        "procedure foo { if (x == 0) then { read x; } else { print y; } }");
    testValidCode("procedure foo { if (x == y + (1 * z)) then { read x; } else "
                  "{ print y; } }");
  }
  SECTION("process_NestedIf_ShouldReturnProgram") {
    testValidCode(
        "procedure foo {"
        "if (x == 0) then { if (x == 0) then { read x; } else { print y; } }"
        "else { if (x == 0) then { read x; } else { print y; } } }");
  }
  SECTION("process_MissingComponent_ShouldThrowException") {
    testInvalidCode(
        "procedure foo { (x == 0) then { read x; } else { print y; } }");
    testInvalidCode(
        "procedure foo { if x == 0) then { read x; } else { print y; } }");
    testInvalidCode(
        "procedure foo { if () then { read x; } else { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0 then { read x; } else { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0) { read x; } else { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0) then read x; } else { print y; } }");
    testInvalidCode("procedure foo { if (x == 0) then { } else { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0) then { read x; else { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0) then { read x; } { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0) then { read x; } else print y; } }");
    testInvalidCode("procedure foo { if (x == 0) then { read x; } else { } }");
    testInvalidCode(
        "procedure foo { if (x == 0) then { read x; } else { print y; }");
  }
  SECTION("process_InvalidKeyword_ShouldThrowException") {
    testInvalidCode(
        "procedure foo { i (x == 0) then { read x; } else { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0) t { read x; } else { print y; } }");
    testInvalidCode(
        "procedure foo { if (x == 0) then { read x; } e { print y; } }");
  }
  SECTION("process_InvalidCondExpr_ShouldThrowException") {
    testInvalidCode(
        "procedure foo { if (x) then { read x; } else { print y; } }");
  }
}

TEST_CASE("process_Assign") {
  SECTION("process_Assign_ShouldReturnProgram") {
    testValidCode("procedure foo { result = (a + 3 * b) - (c / (25 % d)); }");
  }
  SECTION("process_MissingComponent_ShouldThrowException") {
    testInvalidCode("procedure foo { = (a + 3 * b) - (c / (25 % d)); }");
    testInvalidCode("procedure foo { result (a + 3 * b) - (c / (25 % d)); }");
    testInvalidCode("procedure foo { result = ; }");
    testInvalidCode("procedure foo { result = (a + 3 * b) - (c / (25 % d)) }");
  }
  SECTION("process_InvalidVariableName_ShouldThrowException") {
    testInvalidCode("procedure foo { 1 = (a + 3 * b) - (c / (25 % d)); }");
  }
  SECTION("process_InvalidExpression_ShouldThrowException") {
    testInvalidCode("procedure foo { result = 1 + (2 * a; }");
  }
}

TEST_CASE("process_CondExpr") {
  SECTION("process_RelExprMissingComponent") {
    testInvalidCode(
        "procedure foo { while (((a + b) * 10) / 2 ==) { read x; } }");
    testInvalidCode("procedure foo { while (((a + b) * 10) / 2) { read x; } }");
    testInvalidCode("procedure foo { while (x + y * z) { read x; } }");
    testInvalidCode("procedure foo { while (== x + y * z) { read x; } }");
  }
  SECTION("process_RelExprInvalidComponent") {
    testInvalidCode("procedure foo { while (; == x + y * z) { read x; } }");
    testInvalidCode(
        "procedure foo { while (((a + b) * 10) / 2 ; x + y * z) { read x; } }");
    testInvalidCode(
        "procedure foo { while (((a + b) * 10) / 2 == ;) { read x; } }");
  }
  SECTION("process_NotExprMissingComponent") {
    testInvalidCode("procedure foo { while ((varName1 > ((a + b) * 10) / 2)) { "
                    "read x; } }");
    testInvalidCode("procedure foo { while (!varName1 > ((a + b) * 10) / 2)) { "
                    "read x; } }");
    testInvalidCode("procedure foo { while (!()) { read x; } }");
    testInvalidCode("procedure foo { while (!(varName1 > ((a + b) * 10) / 2) { "
                    "read x; } }");
  }
  SECTION("process_NotExprInvalidComponent") {
    testInvalidCode("procedure foo { while (;(varName1 > ((a + b) * 10) / 2)) "
                    "{ read x; } }");
  }
  SECTION("process_CondExprsMissingComponent") {
    testInvalidCode(
        "procedure foo { while (x == 0) && (y == 0)) { read x; } }");
    testInvalidCode("procedure foo { while (() && (y == 0)) { read x; } }");
    testInvalidCode(
        "procedure foo { while ((x == 0 && (y == 0)) { read x; } }");
    testInvalidCode("procedure foo { while ((x == 0) (y == 0)) { read x; } }");
    testInvalidCode(
        "procedure foo { while ((x == 0) && y == 0)) { read x; } }");
    testInvalidCode("procedure foo { while ((x == 0) && ()) { read x; } }");
    testInvalidCode(
        "procedure foo { while ((x == 0) && (y == 0) { read x; } }");
  }
  SECTION("process_CondExprsInvalidComponent") {
    testInvalidCode("procedure foo { while ((varName1 > ((a + b) * 10) / 2) == "
                    "(varName1 > ((a + b) * 10) / 2)) { read x; } }");
  }
}

TEST_CASE("process_Expr") {
  SECTION("process_MissingComponent") {
    testInvalidCode("procedure foo { result = (a; }");
    testInvalidCode("procedure foo { result = a); }");
    testInvalidCode("procedure foo { result = (a)); }");
    testInvalidCode("procedure foo { result = ((a); }");
    testInvalidCode("procedure foo { result = (a + b; }");
    testInvalidCode("procedure foo { result = a + b); }");
    testInvalidCode("procedure foo { result = ((a + b); }");
    testInvalidCode("procedure foo { result = (a + b)); }");
    testInvalidCode("procedure foo { result = (+ b; }");
    testInvalidCode("procedure foo { result = + b); }");
    testInvalidCode("procedure foo { result = (a +; }");
    testInvalidCode("procedure foo { result = a +); }");
    testInvalidCode("procedure foo { result = + b; }");
    testInvalidCode("procedure foo { result = a +; }");
  }
  SECTION("process_InvalidComponent") {
    testInvalidCode("procedure foo { result = a = b; }");
  }
  SECTION("process_ExprWithExprOperators") {
    testValidCode("procedure foo { result = a + b - c + d - e; }");
    testValidCode("procedure foo { result = a + (b - c) + (d) - e; }");
    testValidCode("procedure foo { result = (a + b) - ((c + (d)) - e); }");
    testValidCode("procedure foo { result = (a + b) - (c + (d) - e); }");
    testValidCode(
        "procedure foo { result = (a + 5 * b) - (23 - c + (d) - 6 + e); }");
    testValidCode(
        "procedure foo { result = (a + 5 * b) - (c % 10 + (d * 7) % e % 8); }");
  }
  SECTION("process_ExprWithTermOperators") {
    testValidCode("procedure foo { result = a * b / c % d; }");
    testValidCode("procedure foo { result = (a * b) / c % d; }");
    testValidCode("procedure foo { result = a * (b / c) % d; }");
    testValidCode("procedure foo { result = a * (b / c % d); }");
    testValidCode(
        "procedure foo { result = 1 + 2 % a * (34 - b) * (5 / c) + 6 % d; }");
  }
  SECTION("process_ExprWithAllOperators") {
    testValidCode(
        "procedure foo { result = (a * b) / c % (d + e) + f * g - h % i; }");
    testValidCode("procedure foo { result = (a + b) * ((c % d) / (e / f)) + g "
                  "* (((h))) + i; }");
    testValidCode("procedure foo { result = (123 * a + b) * 4 + 5 % 6 / (7 + "
                  "(c % d) / (8 *e / f % 9)) + g * 10 + (((h))) + i % 11; }");
  }
}
