#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "SP/SourceProcessor.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

TEST_CASE("Variety Of Procedures") {
  std::string sourceCode = "procedure main {"
                           "  call assigner;"  // 1
                           "  call printer;"   // 2
                           "  call reader;"    // 3
                           "  call container;" // 4
                           "  call caller;"    // 5
                           "}"
                           "procedure assigner {"
                           "  add = a/b + c/d;"          // 6
                           "  sub = a/b - c/d;"          // 7
                           "  mul = a/b * c/d;"          // 8
                           "  div = a/b / c/d;"          // 9
                           "  mod = a/b % c/d;"          // 10
                           "  brackets = ((a*b)+(c*d));" // 11
                           "}"
                           "procedure printer {"
                           "  print cs3203;"  // 12
                           "  print cs2103t;" // 13
                           "  print cs1101s;" // 14
                           "}"
                           "procedure reader {"
                           "  read badminton;"  // 15
                           "  read volleyball;" // 16
                           "  read fencing;"    // 17
                           "}"
                           "procedure container {"
                           "  if ((p != 10) && (q != 20)) then {" // 18
                           "    while ((r > 30) && (s > 40)) {"   // 19
                           "      val1 = 50;"                     // 20
                           "    }"
                           "  } else {"
                           "    while ((t < 60) && (u < 70)) {" // 21
                           "      val2 = 80;"                   // 22
                           "    }"
                           "  }"
                           "  while ((v == 90) && (w == 100)) {"      // 23
                           "    if ((p != 110) && (q != 120)) then {" // 24
                           "      while ((r > 130) && (s > 140)) {"   // 25
                           "        val3 = 150;"                      // 26
                           "      }"
                           "    } else {"
                           "      while ((t < 160) && (u < 170)) {" // 27
                           "        val4 = 180;"                    // 28
                           "      }"
                           "    }"
                           "  }"
                           "}"
                           "procedure caller {"
                           "  call alpha;" // 29
                           "}"
                           "procedure alpha {"
                           "  valAlpha = lambda + 1;" // 30
                           "  call beta;"             // 31
                           "}"
                           "procedure beta {"
                           "  valBeta = sigma + 2;" // 32
                           "  call charlie;"        // 33
                           "}"
                           "procedure charlie {"
                           "  valCharlie = gamma + 3;" // 34
                           "}";

  PKB pkb = PKB();
  std::shared_ptr<PKBSetter> setter =
      std::make_shared<PKBSetter>(PKBSetter(pkb));
  std::shared_ptr<PKBGetter> getter =
      std::make_shared<PKBGetter>(PKBGetter(pkb));
  SourceProcessor SP = SourceProcessor(setter);
  SP.processCode(sourceCode);
  setter->done();

  SECTION("Procedures") {
    std::unordered_set<Procedure> actualAbstractions = getter->getProcedures();
    std::unordered_set<Procedure> expectedAbstractions = {
        "main",   "assigner", "printer", "reader", "container",
        "caller", "alpha",    "beta",    "charlie"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Constants") {
    std::unordered_set<Constant> actualAbstractions = getter->getConstants();
    std::unordered_set<Constant> expectedAbstractions = {
        "1",   "2",   "3",   "10",  "20",  "30",  "40",
        "50",  "60",  "70",  "80",  "90",  "100", "110",
        "120", "130", "140", "150", "160", "170", "180"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Variables") {
    std::unordered_set<Variable> actualAbstractions = getter->getVariables();
    std::unordered_set<Variable> expectedAbstractions = {
        "add",        "sub",     "mul",     "div",       "mod",
        "brackets",   "a",       "b",       "c",         "d",
        "cs3203",     "cs2103t", "cs1101s", "badminton", "volleyball",
        "fencing",    "p",       "q",       "r",         "s",
        "t",          "u",       "v",       "w",         "val1",
        "val2",       "val3",    "val4",    "valAlpha",  "valBeta",
        "valCharlie", "lambda",  "sigma",   "gamma"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Read Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getReadStmts();
    std::unordered_set<Statement> expectedAbstractions = {"15", "16", "17"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Print Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getPrintStmts();
    std::unordered_set<Statement> expectedAbstractions = {"12", "13", "14"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Assign Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getAssignStmts();
    std::unordered_set<Statement> expectedAbstractions = {
        "6",  "7",  "8",  "9",  "10", "11", "20",
        "22", "26", "28", "30", "32", "34"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("If Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getIfStmts();
    std::unordered_set<Statement> expectedAbstractions = {"18", "24"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("While Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getWhileStmts();
    std::unordered_set<Statement> expectedAbstractions = {"19", "21", "23",
                                                          "25", "27"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Call Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getCallStmts();
    std::unordered_set<Statement> expectedAbstractions = {
        "1", "2", "3", "4", "5", "29", "31", "33"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("All Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getStatements();
    std::unordered_set<Statement> expectedAbstractions = {
        "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11", "12",
        "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24",
        "25", "26", "27", "28", "29", "30", "31", "32", "33", "34"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("UsesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            // main
            {"1", {"a", "b", "c", "d"}},
            {"2", {"cs3203", "cs2103t", "cs1101s"}},
            {"3", {}},
            {"4", {"v", "w", "p", "q", "r", "s", "t", "u"}},
            {"5", {"lambda", "sigma", "gamma"}},
            // assigner
            {"6", {"a", "b", "c", "d"}},
            {"7", {"a", "b", "c", "d"}},
            {"8", {"a", "b", "c", "d"}},
            {"9", {"a", "b", "c", "d"}},
            {"10", {"a", "b", "c", "d"}},
            {"11", {"a", "b", "c", "d"}},
            // printer
            {"12", {"cs3203"}},
            {"13", {"cs2103t"}},
            {"14", {"cs1101s"}},
            // reader
            {"15", {}},
            {"16", {}},
            {"17", {}},
            // container
            {"18", {"p", "q", "r", "s", "t", "u"}},
            {"19", {"r", "s"}},
            {"20", {}},
            {"21", {"t", "u"}},
            {"22", {}},
            {"23", {"v", "w", "p", "q", "r", "s", "t", "u"}},
            {"24", {"p", "q", "r", "s", "t", "u"}},
            {"25", {"r", "s"}},
            {"26", {}},
            {"27", {"t", "u"}},
            {"28", {}},
            // caller
            {"29", {"lambda", "sigma", "gamma"}},
            // alpha
            {"30", {"lambda"}},
            {"31", {"sigma", "gamma"}},
            // beta
            {"32", {"sigma"}},
            {"33", {"gamma"}},
            // charlie
            {"34", {"gamma"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("UsesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesP();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"main",
             {"a", "b", "c", "d", "v", "w", "p", "q", "r", "s", "t", "u",
              "cs3203", "cs2103t", "cs1101s", "lambda", "sigma", "gamma"}},
            {"assigner", {"a", "b", "c", "d"}},
            {"printer", {"cs3203", "cs2103t", "cs1101s"}},
            {"reader", {}},
            {"container", {"p", "q", "r", "s", "t", "u", "v", "w"}},
            {"caller", {"lambda", "sigma", "gamma"}},
            {"alpha", {"lambda", "sigma", "gamma"}},
            {"beta", {"sigma", "gamma"}},
            {"charlie", {"gamma"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            // main
            {"1", {"add", "sub", "mul", "div", "mod", "brackets"}},
            {"3", {"badminton", "volleyball", "fencing"}},
            {"4", {"val1", "val2", "val3", "val4"}},
            {"5", {"valAlpha", "valBeta", "valCharlie"}},
            // assigner
            {"6", {"add"}},
            {"7", {"sub"}},
            {"8", {"mul"}},
            {"9", {"div"}},
            {"10", {"mod"}},
            {"11", {"brackets"}},
            // printer (none)
            // reader
            {"15", {"badminton"}},
            {"16", {"volleyball"}},
            {"17", {"fencing"}},
            // container
            {"18", {"val1", "val2"}},
            {"19", {"val1"}},
            {"20", {"val1"}},
            {"21", {"val2"}},
            {"22", {"val2"}},
            {"23", {"val3", "val4"}},
            {"24", {"val3", "val4"}},
            {"25", {"val3"}},
            {"26", {"val3"}},
            {"27", {"val4"}},
            {"28", {"val4"}},
            // caller
            {"29", {"valAlpha", "valBeta", "valCharlie"}},
            // alpha
            {"30", {"valAlpha"}},
            {"31", {"valBeta", "valCharlie"}},
            // beta
            {"32", {"valBeta"}},
            {"33", {"valCharlie"}},
            // charlie
            {"34", {"valCharlie"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesP();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"main",
             {"add", "sub", "mul", "div", "mod", "brackets", "badminton",
              "volleyball", "fencing", "val1", "val2", "val3", "val4",
              "valAlpha", "valBeta", "valCharlie"}},
            {"assigner", {"add", "sub", "mul", "div", "mod", "brackets"}},
            {"printer", {}},
            {"reader", {"badminton", "volleyball", "fencing"}},
            {"container", {"val1", "val2", "val3", "val4"}},
            {"caller", {"valAlpha", "valBeta", "valCharlie"}},
            {"alpha", {"valAlpha", "valBeta", "valCharlie"}},
            {"beta", {"valBeta", "valCharlie"}},
            {"charlie", {"valCharlie"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  /* =========================================== NEXT
   * =========================================== */

  SECTION("Positive: isNext(numeric_ref, numeric_ref)") {
    // main
    REQUIRE(getter->isNext("1", "2"));
    REQUIRE(getter->isNext("2", "3"));
    REQUIRE(getter->isNext("3", "4"));
    REQUIRE(getter->isNext("4", "5"));

    // assigner
    REQUIRE(getter->isNext("6", "7"));
    REQUIRE(getter->isNext("7", "8"));
    REQUIRE(getter->isNext("8", "9"));
    REQUIRE(getter->isNext("9", "10"));
    REQUIRE(getter->isNext("10", "11"));

    // printer
    REQUIRE(getter->isNext("12", "13"));
    REQUIRE(getter->isNext("13", "14"));

    // reader
    REQUIRE(getter->isNext("15", "16"));
    REQUIRE(getter->isNext("16", "17"));

    // container
    REQUIRE(getter->isNext("18", "19"));
    REQUIRE(getter->isNext("19", "20"));
    REQUIRE(getter->isNext("20", "19"));
    REQUIRE(getter->isNext("19", "23"));

    REQUIRE(getter->isNext("18", "21"));
    REQUIRE(getter->isNext("21", "22"));
    REQUIRE(getter->isNext("22", "21"));
    REQUIRE(getter->isNext("21", "23"));

    REQUIRE(getter->isNext("24", "25"));
    REQUIRE(getter->isNext("25", "26"));
    REQUIRE(getter->isNext("26", "25"));

    REQUIRE(getter->isNext("24", "27"));
    REQUIRE(getter->isNext("27", "28"));
    REQUIRE(getter->isNext("28", "27"));

    REQUIRE(getter->isNext("23", "24"));
    REQUIRE(getter->isNext("25", "23"));
    REQUIRE(getter->isNext("27", "23"));

    // caller (none)

    // alpha
    REQUIRE(getter->isNext("30", "31"));

    // beta
    REQUIRE(getter->isNext("32", "33"));

    // charlie (none)
  }

  SECTION("Negative: isNext(numeric_ref, numeric_ref)") {
    // Test if !isNext(<consecutive statements on the same nesting level>)
    REQUIRE_FALSE(getter->isNext("2", "1"));

    // Test for !isNext(if statement, next statement in the same nesting level)
    REQUIRE_FALSE(getter->isNext("18", "23"));

    // Test for !isNext(then statement, else statement) and vice versa
    REQUIRE_FALSE(getter->isNext("19", "21"));
    REQUIRE_FALSE(getter->isNext("21", "19"));

    REQUIRE_FALSE(getter->isNext("25", "27"));
    REQUIRE_FALSE(getter->isNext("27", "25"));
  }

  SECTION("Positive: isNext(numeric_ref, wildcard)") {
    REQUIRE(getter->isNext("1", "_"));
    REQUIRE(getter->isNext("2", "_"));
    REQUIRE(getter->isNext("3", "_"));
    REQUIRE(getter->isNext("4", "_"));
    REQUIRE(getter->isNext("6", "_"));
    REQUIRE(getter->isNext("7", "_"));
    REQUIRE(getter->isNext("8", "_"));
    REQUIRE(getter->isNext("9", "_"));
    REQUIRE(getter->isNext("10", "_"));
    REQUIRE(getter->isNext("12", "_"));
    REQUIRE(getter->isNext("13", "_"));
    REQUIRE(getter->isNext("15", "_"));
    REQUIRE(getter->isNext("16", "_"));
    REQUIRE(getter->isNext("18", "_"));
    REQUIRE(getter->isNext("19", "_"));
    REQUIRE(getter->isNext("20", "_"));
    REQUIRE(getter->isNext("21", "_"));
    REQUIRE(getter->isNext("22", "_"));
    REQUIRE(getter->isNext("23", "_"));
    REQUIRE(getter->isNext("24", "_"));
    REQUIRE(getter->isNext("25", "_"));
    REQUIRE(getter->isNext("26", "_"));
    REQUIRE(getter->isNext("27", "_"));
    REQUIRE(getter->isNext("28", "_"));
    REQUIRE(getter->isNext("30", "_"));
    REQUIRE(getter->isNext("32", "_"));
  }

  SECTION("Negative: isNext(numeric_ref, wildcard)") {
    REQUIRE_FALSE(getter->isNext("5", "_"));
    REQUIRE_FALSE(getter->isNext("11", "_"));
    REQUIRE_FALSE(getter->isNext("14", "_"));
    REQUIRE_FALSE(getter->isNext("17", "_"));
    REQUIRE_FALSE(getter->isNext("29", "_"));
    REQUIRE_FALSE(getter->isNext("31", "_"));
    REQUIRE_FALSE(getter->isNext("33", "_"));
    REQUIRE_FALSE(getter->isNext("34", "_"));
  }

  SECTION("Positive: isNext(wildcard, numeric_ref)") {
    REQUIRE(getter->isNext("_", "2"));
    REQUIRE(getter->isNext("_", "3"));
    REQUIRE(getter->isNext("_", "4"));
    REQUIRE(getter->isNext("_", "5"));
    REQUIRE(getter->isNext("_", "7"));
    REQUIRE(getter->isNext("_", "8"));
    REQUIRE(getter->isNext("_", "9"));
    REQUIRE(getter->isNext("_", "10"));
    REQUIRE(getter->isNext("_", "11"));
    REQUIRE(getter->isNext("_", "13"));
    REQUIRE(getter->isNext("_", "14"));
    REQUIRE(getter->isNext("_", "16"));
    REQUIRE(getter->isNext("_", "17"));
    REQUIRE(getter->isNext("_", "19"));
    REQUIRE(getter->isNext("_", "20"));
    REQUIRE(getter->isNext("_", "21"));
    REQUIRE(getter->isNext("_", "22"));
    REQUIRE(getter->isNext("_", "23"));
    REQUIRE(getter->isNext("_", "24"));
    REQUIRE(getter->isNext("_", "25"));
    REQUIRE(getter->isNext("_", "26"));
    REQUIRE(getter->isNext("_", "27"));
    REQUIRE(getter->isNext("_", "28"));
    REQUIRE(getter->isNext("_", "31"));
    REQUIRE(getter->isNext("_", "33"));
  }

  SECTION("Negative: isNext(wildcard, numeric_ref)") {
    REQUIRE_FALSE(getter->isNext("_", "1"));
    REQUIRE_FALSE(getter->isNext("_", "6"));
    REQUIRE_FALSE(getter->isNext("_", "12"));
    REQUIRE_FALSE(getter->isNext("_", "15"));
    REQUIRE_FALSE(getter->isNext("_", "18"));
    REQUIRE_FALSE(getter->isNext("_", "29"));
    REQUIRE_FALSE(getter->isNext("_", "30"));
    REQUIRE_FALSE(getter->isNext("_", "32"));
    REQUIRE_FALSE(getter->isNext("_", "34"));
  }

  SECTION("isNext(numeric_ref, wildcard)") {
    REQUIRE(getter->isNext("_", "_"));
  }
}