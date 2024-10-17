#include "catch.hpp"

#include "SP/Parser/Parser.h"
#include "SP/Tokenizer/Tokenizer.h"
#include "utils/TokenHandler/TokenStream.h"

std::shared_ptr<SPProcedure>
parseProcedureForTestCase(std::string sourceCode, std::string procedureName) {
  std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer>();
  std::shared_ptr<TokenStream> tokenStream = tokenizer->process(sourceCode);
  std::shared_ptr<Parser> parser = std::make_shared<Parser>();
  std::shared_ptr<SourceProgram> program = parser->process(tokenStream);
  return program->getProcedure(procedureName);
}

TEST_CASE("process_ProcedureWithOneIf_CheckNextParsing") {

  std::string simpleIfSourceCode = "procedure simpleIfProc {"
                                   "	read a;"           // 1
                                   "   if (1 == 1) then {" // 2
                                   "       read x;"        // 3
                                   "       read y;"        // 4
                                   "       read z;"        // 5
                                   "   } else {"
                                   "       read x;" // 6
                                   "       read y;" // 7
                                   "       read z;" // 8
                                   "   }"
                                   "   read b;" // 9
                                   "}";

  std::shared_ptr<SPProcedure> procedure =
      parseProcedureForTestCase(simpleIfSourceCode, "simpleIfProc");
  std::unordered_map<int, std::unordered_set<int>> parsedEdges =
      procedure->getEdges();

  std::unordered_map<int, std::unordered_set<int>> expectedEdges = {
      {1, {2}}, {2, {3, 6}}, {3, {4}}, {4, {5}}, {5, {9}},
      {6, {7}}, {7, {8}},    {8, {9}}, {9, {0}},
  };

  REQUIRE(parsedEdges == expectedEdges);
}

TEST_CASE("process_ProcedureWithOneWhile_CheckNextParsing") {

  std::string simpleWhileSourceCode = "procedure simpleWhileProc {"
                                      "	read a;"            // 1
                                      "   while (1 == 1) {" // 2
                                      "       read x;"      // 3
                                      "       read y;"      // 4
                                      "       read z;"      // 5
                                      "   }"
                                      "   read b;" // 6
                                      "}";

  std::shared_ptr<SPProcedure> procedure =
      parseProcedureForTestCase(simpleWhileSourceCode, "simpleWhileProc");
  std::unordered_map<int, std::unordered_set<int>> parsedEdges =
      procedure->getEdges();

  std::unordered_map<int, std::unordered_set<int>> expectedEdges = {
      {1, {2}}, {2, {3, 6}}, {3, {4}}, {4, {5}}, {5, {2}}, {6, {0}},
  };

  REQUIRE(parsedEdges == expectedEdges);
}

TEST_CASE("process_ProcedureWithIfInWhile_CheckNextParsing") {

  std::string singleIfInWhileSouceCode = "procedure singleIfInWhileProc {"
                                         "	read a;"             // 1
                                         "   while (1 == 1) {"       // 2
                                         "       if (1 == 1) then {" // 3
                                         "           read y;"        // 4
                                         "           read z;"        // 5
                                         "       } else {"
                                         "           read y;" // 6
                                         "           read z;" // 7
                                         "       }"
                                         "       read w;" // 8
                                         "   }"
                                         "   read b;" // 9
                                         "}";

  std::shared_ptr<SPProcedure> singleIfInWhileProc = parseProcedureForTestCase(
      singleIfInWhileSouceCode, "singleIfInWhileProc");
  std::unordered_map<int, std::unordered_set<int>>
      singleIfInWhileProcParsedEdges = singleIfInWhileProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      singleIfInWhileProcExpectedEdges = {
          {1, {2}}, {2, {3, 9}}, {3, {4, 6}}, {4, {5}}, {5, {8}},
          {6, {7}}, {7, {8}},    {8, {2}},    {9, {0}},
      };
  REQUIRE(singleIfInWhileProcParsedEdges == singleIfInWhileProcExpectedEdges);

  std::string ifStatementInStartOfWhileSourceCode =
      "procedure ifStatementInStartOfWhileProc {"
      "	read a;"                  // 1
      "   while (1 == 1) {"       // 2
      "       if (1 == 1) then {" // 3
      "           read x;"        // 4
      "       } else {"
      "           read x;" // 5
      "       }"
      "       read x;" // 6
      "       read y;" // 7
      "   }"
      "   read b;" // 8
      "}";

  std::shared_ptr<SPProcedure> ifStatementInStartOfWhileProc =
      parseProcedureForTestCase(ifStatementInStartOfWhileSourceCode,
                                "ifStatementInStartOfWhileProc");
  std::unordered_map<int, std::unordered_set<int>>
      ifStatementInStartOfWhileParsedEdges =
          ifStatementInStartOfWhileProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      ifStatementInStartOfWhileExpectedEdges = {
          {1, {2}}, {2, {3, 8}}, {3, {4, 5}}, {4, {6}},
          {5, {6}}, {6, {7}},    {7, {2}},    {8, {0}},
      };

  std::string ifStatementInMiddleOfWhileSourceCode =
      "procedure ifStatementInMiddleOfWhileProc {"
      "	read a;"                  // 1
      "   while (1 == 1) {"       // 2
      "       read x;"            // 3
      "       if (1 == 1) then {" // 4
      "           read x;"        // 5
      "       } else {"
      "           read x;" // 6
      "       }"
      "       read x;" // 7
      "   }"
      "   read b;" // 8
      "}";

  std::shared_ptr<SPProcedure> ifStatementInMiddleOfWhileProc =
      parseProcedureForTestCase(ifStatementInMiddleOfWhileSourceCode,
                                "ifStatementInMiddleOfWhileProc");
  std::unordered_map<int, std::unordered_set<int>>
      ifStatementInMiddleOfWhileParsedEdges =
          ifStatementInMiddleOfWhileProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      ifStatementInMiddleOfWhileExpectedEdges = {
          {1, {2}}, {2, {3, 8}}, {3, {4}}, {4, {5, 6}},
          {5, {7}}, {6, {7}},    {7, {2}}, {8, {0}},
      };
  REQUIRE(ifStatementInMiddleOfWhileParsedEdges ==
          ifStatementInMiddleOfWhileExpectedEdges);

  std::string ifStatementInEndOfWhileSourceCode =
      "procedure ifStatementInEndOfWhileProc {"
      "	read a;"                  // 1
      "   while (1 == 1) {"       // 2
      "       read x;"            // 3
      "       read y;"            // 4
      "       if (1 == 1) then {" // 5
      "           read x;"        // 6
      "           read x;"        // 7
      "           read x;"        // 8
      "       } else {"
      "           read y;" // 9
      "           read y;" // 10
      "           read y;" // 11
      "       }"
      "   }"
      "   read b;" // 12
      "}";

  std::shared_ptr<SPProcedure> ifStatementInEndOfWhileProc =
      parseProcedureForTestCase(ifStatementInEndOfWhileSourceCode,
                                "ifStatementInEndOfWhileProc");
  std::unordered_map<int, std::unordered_set<int>>
      ifStatementInEndOfWhileParsedEdges =
          ifStatementInEndOfWhileProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      ifStatementInEndOfWhileExpectedEdges = {
          {1, {2}}, {2, {3, 12}}, {3, {4}},  {4, {5}},   {5, {6, 9}}, {6, {7}},
          {7, {8}}, {8, {2}},     {9, {10}}, {10, {11}}, {11, {2}},   {12, {0}},
      };
}

TEST_CASE("process_ProcedureWithWhileInIf_CheckNextParsing") {

  std::string whileStatementsInIfSourceCode =
      "procedure whileStatementsInIfProc {"
      "	read a;"                // 1
      "   if (1 == 1) then {"   // 2
      "       read x;"          // 3
      "       while (1 == 1) {" // 4    // While Statement with 2 lines, in
                                // middle of THEN block
      "           read x;"      // 5
      "           read y;"      // 6
      "       }"
      "       while (1 == 1) {" // 7    // While Statement with 3 lines, in
                                // middle of THEN block
      "           read x;"      // 8
      "           read y;"      // 9
      "           read z;"      // 10
      "       }"
      "       read x;" // 11
      "   } else {"
      "       while (1 == 1) {" // 12    // While Statement with 2 lines, in
                                // start of ELSE block
      "           read x;"      // 13
      "           read y;"      // 14
      "       }"
      "       read x;" // 15
      "   }"
      "   if (1 == 1) then {"   // 16
      "       while (1 == 1) {" // 17    // While Statement with 1 line, in
                                // start of THEN block
      "           read x;"      // 18
      "       }"
      "       read x;" // 19
      "   } else {"
      "       read x;"          // 20
      "       while (1 == 1) {" // 21    // While Statement with 1 line, in end
                                // of ELSE block
      "           read x;"      // 22
      "       }"
      "   }"
      "   read b;" // 23
      "}";

  std::shared_ptr<SPProcedure> whileStatementsInIfProc =
      parseProcedureForTestCase(whileStatementsInIfSourceCode,
                                "whileStatementsInIfProc");
  std::unordered_map<int, std::unordered_set<int>>
      whileStatementsInIfParsedEdges = whileStatementsInIfProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      whileStatementsInIfExpectedEdges = {
          {1, {2}},       {2, {3, 12}},   {3, {4}},   {4, {5, 7}}, {5, {6}},
          {6, {4}},       {7, {8, 11}},   {8, {9}},   {9, {10}},   {10, {7}},
          {11, {16}},     {12, {13, 15}}, {13, {14}}, {14, {12}},  {15, {16}},
          {16, {17, 20}}, {17, {18, 19}}, {18, {17}}, {19, {23}},  {20, {21}},
          {21, {22, 23}}, {22, {21}},     {23, {0}},
      };

  REQUIRE(whileStatementsInIfParsedEdges == whileStatementsInIfExpectedEdges);
}

TEST_CASE("process_ProcedureWithIfInIf_CheckNextParsing") {

  std::string IfStatementsInIfSourceCode =
      "procedure IfStatementsInIfProc {"
      "	read a;"                  // 1
      "   if (1 == 1) then {"     // 2
      "       read x;"            // 3
      "       if (1 == 1) then {" // 4    // If Statement in middle of THEN
                                  // block
      "           read x;"        // 5
      "           read y;"        // 6
      "           read z;"        // 7
      "       } else {"
      "           read x;" // 8
      "           read y;" // 9
      "           read z;" // 10
      "       }"
      "       read x;" // 11
      "   } else {"
      "       if (1 == 1) then {" // 12    // If Statement with 2 lines, in
                                  // start of ELSE block
      "           read x;"        // 13
      "           read y;"        // 14
      "       } else {"
      "           read x;" // 15
      "       }"
      "       read x;" // 16
      "   }"
      "   if (1 == 1) then {"     // 17
      "       if (1 == 1) then {" // 18    // If Statement with 1 line, in start
                                  // of THEN block
      "           read x;"        // 19
      "       } else {"
      "           read x;" // 20
      "       }"
      "       read x;" // 21
      "   } else {"
      "       read x;"            // 22
      "       if (1 == 1) then {" // 23    // If Statement with 1 line, in end
                                  // of ELSE block
      "           read x;"        // 24
      "       } else {"
      "           read x;" // 25
      "       }"
      "   }"
      "   read b;" // 26
      "}";

  std::shared_ptr<SPProcedure> IfStatementsInIfProc = parseProcedureForTestCase(
      IfStatementsInIfSourceCode, "IfStatementsInIfProc");
  std::unordered_map<int, std::unordered_set<int>> IfStatementsInIfParsedEdges =
      IfStatementsInIfProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      IfStatementsInIfExpectedEdges = {
          {1, {2}},       {2, {3, 12}},   {3, {4}},           {4, {5, 8}},
          {5, {6}},       {6, {7}},       {7, {11}},          {8, {9}},
          {9, {10}},      {10, {11}},     {11, {17}},         {12, {13, 15}},
          {13, {14}},     {14, {16}},     {15, {16}},         {16, {17}},
          {17, {18, 22}}, {18, {19, 20}}, {19, {21}},         {20, {21}},
          {21, {26}},     {22, {23}},     {23, {24, 25}},     {24, {26}},
          {25, {26}},     {26, {0}},
      };

  REQUIRE(IfStatementsInIfParsedEdges == IfStatementsInIfExpectedEdges);
}

TEST_CASE("process_ProcedureWithWhileInWhile_CheckNextParsing") {

  std::string whileStatementsInWhileSourceCode =
      "procedure whileStatementsInWhileProc {"
      "	read a;"                // 1
      "   while (1 == 1) {"     // 2
      "       read x;"          // 3
      "       while (1 == 1) {" // 4    // While Statement with 3 lines, in
                                // middle of BODY block
      "           read x;"      // 5
      "           read y;"      // 6
      "           read z;"      // 7
      "       }"
      "       read x;" // 8
      "   }"
      "   while (1 == 1) {"     // 9
      "       while (1 == 1) {" // 10    // While Statement with 2 lines, in
                                // start of BODY block
      "           read x;"      // 11
      "           read y;"      // 12
      "       }"
      "       read x;" // 13
      "       read x;" // 14
      "   }"
      "   while (1 == 1) {"     // 15
      "       read x;"          // 16
      "       read x;"          // 17
      "       while (1 == 1) {" // 18    // While Statement with 1 line, in end
                                // of BODY block
      "           read x;"      // 19
      "       }"
      "   }"
      "   read b;" // 20
      "}";

  std::shared_ptr<SPProcedure> whileStatementsInWhileProc =
      parseProcedureForTestCase(whileStatementsInWhileSourceCode,
                                "whileStatementsInWhileProc");
  std::unordered_map<int, std::unordered_set<int>>
      whileStatementsInWhileParsedEdges =
          whileStatementsInWhileProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      whileStatementsInWhileExpectedEdges = {
          {1, {2}},      {2, {3, 9}},    {3, {4}},       {4, {5, 8}},
          {5, {6}},      {6, {7}},       {7, {4}},       {8, {2}},
          {9, {10, 15}}, {10, {11, 13}}, {11, {12}},     {12, {10}},
          {13, {14}},    {14, {9}},      {15, {16, 20}}, {16, {17}},
          {17, {18}},    {18, {15, 19}}, {19, {18}},     {20, {0}},
      };

  REQUIRE(whileStatementsInWhileParsedEdges ==
          whileStatementsInWhileExpectedEdges);
}

TEST_CASE("process_ProcedureWithNestedIfs_CheckNextParsing") {

  std::string nestedIfsSourceCode =
      "procedure nestedIfsProc {"
      "	  read a;"                    // 1
      "   if (1 == 1) then {"         // 2
      "       read x;"                // 3
      "       if (1 == 1) then {"     // 4    
      "           read x;"            // 5
      "           read x;"            // 6
      "       } else {"
      "           read x;"            // 7
      "           if (1 == 1) then {" // 8
      "               read x;"        // 9
      "               read x;"        // 10
      "           } else {"
      "               read x;"        // 11
      "               read x;"        // 12
      "           }"
      "       }"
      "    } else { "
      "       read x;"                // 13
      "       if (1 == 1) then {"     // 14
      "           read x;"            // 15
      "       } else { "
      "           read x;"            // 16
      "       }"
      "    }"
      "    read x;"                   // 17
      "}";

  std::shared_ptr<SPProcedure> nestedIfsProc =
      parseProcedureForTestCase(nestedIfsSourceCode,
                                "nestedIfsProc");
  std::unordered_map<int, std::unordered_set<int>>
      nestedIfParsedEdges =
          nestedIfsProc->getEdges();

  std::unordered_map<int, std::unordered_set<int>>
      nestedIfExpectedEdges = {
          {1, {2}},      {2, {3, 13}},   {3, {4}},       {4, {5, 7}},
          {5, {6}},      {6, {17}},      {7, {8}},       {8, {9, 11}},
          {9, {10}},     {10, {17}},     {11, {12}},     {12, {17}},
          {13, {14}},    {14, {15, 16}}, {15, {17}},     {16, {17}},
          {17, {0}},
      };

  REQUIRE(nestedIfParsedEdges == nestedIfExpectedEdges);
    
}
