#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "SP/SourceProcessor.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

TEST_CASE("Nested While Statements") {
  std::string sourceCode = "procedure foo {"
                           "	while (x == 10) {"   // 1
                           "    while (x == 20) {"   // 2
                           "      read a;"           // 3
                           "      while (x == 30) {" // 4
                           "        print b;"        // 5
                           "        c = 40;"         // 6
                           "      }"
                           "      call bar;" // 7
                           "    }"
                           "    read a;"             // 8
                           "    while (x == 50) {"   // 9
                           "      print b;"          // 10
                           "      while (x == 60) {" // 11
                           "        c = 70;"         // 12
                           "        call bar;"       // 13
                           "      }"
                           "      read a;" // 14
                           "    }"
                           "    print b;"             // 15
                           "    while (x == 80) {"    // 16
                           "      c = 90;"            // 17
                           "      while (x == 100) {" // 18
                           "        call bar;"        // 19
                           "        read a;"          // 20
                           "      }"
                           "      print b;" // 21
                           "    }"
                           "	}"
                           "}"
                           "procedure bar {"
                           "  z = 110;" // 22
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
    std::unordered_set<Procedure> expectedAbstractions = {"foo", "bar"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Constants") {
    std::unordered_set<Constant> actualAbstractions = getter->getConstants();
    std::unordered_set<Constant> expectedAbstractions = {
        "10", "20", "30", "40", "50", "60", "70", "80", "90", "100", "110"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Variables") {
    std::unordered_set<Variable> actualAbstractions = getter->getVariables();
    std::unordered_set<Variable> expectedAbstractions = {"x", "a", "b", "c",
                                                         "z"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Read Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getReadStmts();
    std::unordered_set<Statement> expectedAbstractions = {"3", "8", "14", "20"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Print Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getPrintStmts();
    std::unordered_set<Statement> expectedAbstractions = {"5", "10", "15",
                                                          "21"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Assign Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getAssignStmts();
    std::unordered_set<Statement> expectedAbstractions = {"6", "12", "17",
                                                          "22"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("If Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getIfStmts();
    std::unordered_set<Statement> expectedAbstractions = {};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("While Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getWhileStmts();
    std::unordered_set<Statement> expectedAbstractions = {"1",  "2",  "4", "9",
                                                          "11", "16", "18"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Call Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getCallStmts();
    std::unordered_set<Statement> expectedAbstractions = {"7", "13", "19"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("All Statements") {
    std::unordered_set<Statement> actualAbstractions = getter->getStatements();
    std::unordered_set<Statement> expectedAbstractions = {
        "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11",
        "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"};
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Follows") {
    std::unordered_map<Statement, Statement> actualAbstractions =
        getter->getFollows();
    std::unordered_map<Statement, Statement> expectedAbstractions = {
        // Follows 2-8-9-15-16
        {"2", "8"},
        {"8", "9"},
        {"9", "15"},
        {"15", "16"},
        // Follows 3-4-7
        {"3", "4"},
        {"4", "7"},
        // Follows 5-6
        {"5", "6"},
        // Follows 10-11-14
        {"10", "11"},
        {"11", "14"},
        // Follows 12-13
        {"12", "13"},
        // Follows 17-18-21
        {"17", "18"},
        {"18", "21"},
        // Follows 19-20
        {"19", "20"},
    };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("FollowsT") {
    std::unordered_map<Statement, std::unordered_set<Statement>>
        actualAbstractions = getter->getFollowsT();
    std::unordered_map<Statement, std::unordered_set<Statement>>
        expectedAbstractions = {
            // Follows 2-8-9-15-16
            {"1", {}},
            {"2", {"8", "9", "15", "16"}},
            {"0", {}},
            // Follows 3-4-7
            {"3", {"4", "7"}},
            {"4", {"7"}},
            // Follows 5-6
            {"5", {"6"}},
            {"6", {}},
            {"7", {}},
            {"8", {"9", "15", "16"}},
            {"9", {"15", "16"}},
            // Follows 10-11-14
            {"10", {"11", "14"}},
            {"11", {"14"}},
            // Follows 12-13
            {"12", {"13"}},
            {"13", {}},
            {"14", {}},
            {"15", {"16"}},
            {"16", {}},
            // Follows 17-18-21
            {"17", {"18", "21"}},
            {"18", {"21"}},
            // Follows 19-20
            {"19", {"20"}},
            {"20", {}},
            {"21", {}},
            {"22", {}},

        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("Parent") {
    std::unordered_map<Statement, std::unordered_set<Statement>>
        actualAbstractions = getter->getParent();
    std::unordered_map<Statement, std::unordered_set<Statement>>
        expectedAbstractions = {
            // Parent 1
            {"1", {"2", "8", "9", "15", "16"}},
            {"0", {}},
            // Parent 2
            {"2", {"3", "4", "7"}},
            {"3", {}},
            // Parent 4
            {"4", {"5", "6"}},
            {"5", {}},
            {"6", {}},
            {"7", {}},
            {"8", {}},
            // Parent 9
            {"9", {"10", "11", "14"}},
            {"10", {}},
            // Parent 11
            {"11", {"12", "13"}},
            {"12", {}},
            {"13", {}},
            {"14", {}},
            {"15", {}},
            // Parent 16
            {"16", {"17", "18", "21"}},
            {"17", {}},
            // Parent 18
            {"18", {"19", "20"}},
            {"19", {}},
            {"20", {}},
            {"21", {}},
            {"22", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ParentT") {
    std::unordered_map<Statement, std::unordered_set<Statement>>
        actualAbstractions = getter->getParentT();
    std::unordered_map<Statement, std::unordered_set<Statement>>
        expectedAbstractions = {
            // Parent 1
            {"1", {"2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "10", "11",
                   "12", "13", "14", "15", "16", "17", "18", "19", "20", "21"}},
            {"0", {}},
            // Parent 2
            {"2", {"3", "4", "5", "6", "7"}},
            // Parent 4
            {"3", {}},
            {"4", {"5", "6"}},
            {"5", {}},
            {"6", {}},
            {"7", {}},
            {"8", {}},
            // Parent 9
            {"9", {"10", "11", "12", "13", "14"}},
            // Parent 11
            {"10", {}},
            {"11", {"12", "13"}},
            {"12", {}},
            {"13", {}},
            {"14", {}},
            {"15", {}},
            // Parent 16
            {"16", {"17", "18", "19", "20", "21"}},
            // Parent 18
            {"17", {}},
            {"18", {"19", "20"}},
            {"19", {}},
            {"20", {}},
            {"21", {}},
            {"22", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("UsesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            // foo
            {"1", {"x", "b"}},
            {"2", {"x", "b"}},
            {"3", {}},
            {"4", {"x", "b"}},
            {"5", {"b"}},
            {"6", {}},
            {"7", {}},
            {"8", {}},
            {"9", {"x", "b"}},
            {"10", {"b"}},
            {"11", {"x"}},
            {"12", {}},
            {"13", {}},
            {"14", {}},
            {"15", {"b"}},
            {"16", {"x", "b"}},
            {"17", {}},
            {"18", {"x"}},
            {"19", {}},
            {"20", {}},
            {"21", {"b"}},
            // bar
            {"22", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("UsesP") {
    std::unordered_map<Procedure, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesP();
    std::unordered_map<Procedure, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"foo", {"x", "b"}},
            {"bar", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            // foo
            {"1", {"a", "c", "z"}},
            {"2", {"a", "c", "z"}},
            {"3", {"a"}},
            {"4", {"c"}},
            {"6", {"c"}},
            {"7", {"z"}},
            {"8", {"a"}},
            {"9", {"a", "c", "z"}},
            {"11", {"c", "z"}},
            {"12", {"c"}},
            {"13", {"z"}},
            {"14", {"a"}},
            {"16", {"a", "c", "z"}},
            {"17", {"c"}},
            {"18", {"a", "z"}},
            {"19", {"z"}},
            {"20", {"a"}},
            // bar
            {"22", {"z"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesP();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"foo", {"a", "c", "z"}},
            {"bar", {"z"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  /* =========================================== NEXT
   * =========================================== */

  SECTION("Positive: isNext(numeric_ref, numeric_ref)") {
    // foo
    // start of while1
    REQUIRE(getter->isNext("1", "2"));
    // start of while2
    REQUIRE(getter->isNext("2", "3"));
    REQUIRE(getter->isNext("3", "4"));
    // start of while3
    REQUIRE(getter->isNext("4", "5"));
    REQUIRE(getter->isNext("5", "6"));
    REQUIRE(getter->isNext("6", "4"));
    // end of while3
    REQUIRE(getter->isNext("4", "7"));
    REQUIRE(getter->isNext("7", "2"));
    // end of while2
    REQUIRE(getter->isNext("2", "8"));
    REQUIRE(getter->isNext("8", "9"));
    // start of while4
    REQUIRE(getter->isNext("9", "10"));
    REQUIRE(getter->isNext("10", "11"));
    // start of while5
    REQUIRE(getter->isNext("11", "12"));
    REQUIRE(getter->isNext("12", "13"));
    REQUIRE(getter->isNext("13", "11"));
    // end of while5
    REQUIRE(getter->isNext("11", "14"));
    REQUIRE(getter->isNext("14", "9"));
    // end of while4
    REQUIRE(getter->isNext("9", "15"));
    REQUIRE(getter->isNext("15", "16"));
    // start of while6
    REQUIRE(getter->isNext("16", "17"));
    REQUIRE(getter->isNext("17", "18"));
    // start of while7
    REQUIRE(getter->isNext("18", "19"));
    REQUIRE(getter->isNext("19", "20"));
    REQUIRE(getter->isNext("20", "18"));
    // end of while7
    REQUIRE(getter->isNext("18", "21"));
    REQUIRE(getter->isNext("21", "16"));
    // end of while6
    REQUIRE(getter->isNext("16", "1"));
    // end of while 1

    // bar (none)
  }

  SECTION("Negative: isNext(numeric_ref, numeric_ref)") {
    // Test if !isNext(<consecutive statements on the same nesting level>)
    REQUIRE_FALSE(getter->isNext("6", "5"));
    REQUIRE_FALSE(getter->isNext("13", "12"));
    REQUIRE_FALSE(getter->isNext("20", "19"));

    // Test for !isNext(statement inside while statement, while statement)
    REQUIRE_FALSE(getter->isNext("2", "1"));
    REQUIRE_FALSE(getter->isNext("8", "1"));
    REQUIRE_FALSE(getter->isNext("9", "1"));
    REQUIRE_FALSE(getter->isNext("15", "1"));
    REQUIRE_FALSE(getter->isNext("3", "2"));
    REQUIRE_FALSE(getter->isNext("4", "2"));
    REQUIRE_FALSE(getter->isNext("5", "4"));
    REQUIRE_FALSE(getter->isNext("10", "9"));
    REQUIRE_FALSE(getter->isNext("11", "9"));
    REQUIRE_FALSE(getter->isNext("12", "11"));
    REQUIRE_FALSE(getter->isNext("17", "16"));
    REQUIRE_FALSE(getter->isNext("18", "16"));
    REQUIRE_FALSE(getter->isNext("19", "18"));
  }

  SECTION("Positive: isNext(numeric_ref, wildcard)") {
    REQUIRE(getter->isNext("1", "_"));
    REQUIRE(getter->isNext("2", "_"));
    REQUIRE(getter->isNext("3", "_"));
    REQUIRE(getter->isNext("4", "_"));
    REQUIRE(getter->isNext("5", "_"));
    REQUIRE(getter->isNext("6", "_"));
    REQUIRE(getter->isNext("7", "_"));
    REQUIRE(getter->isNext("8", "_"));
    REQUIRE(getter->isNext("9", "_"));
    REQUIRE(getter->isNext("10", "_"));
    REQUIRE(getter->isNext("11", "_"));
    REQUIRE(getter->isNext("12", "_"));
    REQUIRE(getter->isNext("13", "_"));
    REQUIRE(getter->isNext("14", "_"));
    REQUIRE(getter->isNext("15", "_"));
    REQUIRE(getter->isNext("16", "_"));
    REQUIRE(getter->isNext("17", "_"));
    REQUIRE(getter->isNext("18", "_"));
    REQUIRE(getter->isNext("19", "_"));
    REQUIRE(getter->isNext("20", "_"));
    REQUIRE(getter->isNext("21", "_"));
  }

  SECTION("Negative: isNext(numeric_ref, wildcard)") {
    REQUIRE_FALSE(getter->isNext("22", "_"));
  }

  SECTION("Positive: isNext(wildcard, numeric_ref)") {
    REQUIRE(getter->isNext("_", "1"));
    REQUIRE(getter->isNext("_", "2"));
    REQUIRE(getter->isNext("_", "3"));
    REQUIRE(getter->isNext("_", "4"));
    REQUIRE(getter->isNext("_", "5"));
    REQUIRE(getter->isNext("_", "6"));
    REQUIRE(getter->isNext("_", "7"));
    REQUIRE(getter->isNext("_", "8"));
    REQUIRE(getter->isNext("_", "9"));
    REQUIRE(getter->isNext("_", "10"));
    REQUIRE(getter->isNext("_", "11"));
    REQUIRE(getter->isNext("_", "12"));
    REQUIRE(getter->isNext("_", "13"));
    REQUIRE(getter->isNext("_", "14"));
    REQUIRE(getter->isNext("_", "15"));
    REQUIRE(getter->isNext("_", "16"));
    REQUIRE(getter->isNext("_", "17"));
    REQUIRE(getter->isNext("_", "18"));
    REQUIRE(getter->isNext("_", "19"));
    REQUIRE(getter->isNext("_", "20"));
    REQUIRE(getter->isNext("_", "21"));
  }

  SECTION("Negative: isNext(wildcard, numeric_ref)") {
    REQUIRE_FALSE(getter->isNext("_", "22"));
  }

  SECTION("isNext(numeric_ref, wildcard)") {
    REQUIRE(getter->isNext("_", "_"));
  }
}