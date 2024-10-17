#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "SP/SourceProcessor.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

TEST_CASE("Follows Abstractions") {
  std::string sourceCode = "procedure foo {"
                           "	a = 1;"                   // 1
                           "	b = 2;"                   // 2
                           "	c = 3;"                   // 3
                           "	while (x == 0) {"         // 4
                           "		while (y == 0) {" // 5
                           "			d = 4;"   // 6
                           "			e = 5;"   // 7
                           "		}"
                           "	}"
                           "}";

  PKB pkb = PKB();
  std::shared_ptr<PKBSetter> setter =
      std::make_shared<PKBSetter>(PKBSetter(pkb));
  std::shared_ptr<PKBGetter> getter =
      std::make_shared<PKBGetter>(PKBGetter(pkb));
  SourceProcessor SP = SourceProcessor(setter);
  SP.processCode(sourceCode);
  setter->done();

  SECTION("Follows") {
    std::unordered_map<Statement, Statement> actualAbstractions =
        getter->getFollows();
    std::unordered_map<Statement, Statement> expectedAbstractions = {
        // stmtLst under procedure foo
        {"1", "2"},
        {"2", "3"},
        {"3", "4"},
        // stmtLst under second while loop
        {"6", "7"},
    };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("FollowsT") {
    std::unordered_map<Statement, std::unordered_set<Statement>>
        actualAbstractions = getter->getFollowsT();
    std::unordered_map<Statement, std::unordered_set<Statement>>
        expectedAbstractions = {
            // stmtLst under procedure foo
            {"0", {}},
            {"1", {"2", "3", "4"}},
            {"2", {"3", "4"}},
            {"3", {"4"}},
            {"4", {}},
            {"5", {}},
            // stmtLst under second while loop
            {"6", {"7"}},
            {"7", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }
}

TEST_CASE("Parent Abstractions") {
  std::string sourceCode = "procedure foo {"
                           "	while (x == 0) {"         // 1
                           "		a = 1;"           // 2
                           "		b = 2;"           // 3
                           "		while (y == 0) {" // 4
                           "			c = 3;"   // 5
                           "			d = 4;"   // 6
                           "		}"
                           "	}"
                           "}";

  PKB pkb = PKB();
  std::shared_ptr<PKBSetter> setter =
      std::make_shared<PKBSetter>(PKBSetter(pkb));
  std::shared_ptr<PKBGetter> getter =
      std::make_shared<PKBGetter>(PKBGetter(pkb));
  SourceProcessor SP = SourceProcessor(setter);
  SP.processCode(sourceCode);
  setter->done();

  SECTION("Parent") {
    std::unordered_map<Statement, std::unordered_set<Statement>>
        actualAbstractions = getter->getParent();
    std::unordered_map<Statement, std::unordered_set<Statement>>
        expectedAbstractions = {
            // first while loop
            {"0", {}},
            {"1", {"2", "3", "4"}},
            {"2", {}},
            {"3", {}},
            // second while loop
            {"4", {"5", "6"}},
            {"5", {}},
            {"6", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ParentT") {
    std::unordered_map<Statement, std::unordered_set<Statement>>
        actualAbstractions = getter->getParentT();
    std::unordered_map<Statement, std::unordered_set<Statement>>
        expectedAbstractions = {
            // first while loop
            {"0", {}},
            {"1", {"2", "3", "4", "5", "6"}},
            {"2", {}},
            {"3", {}},
            // second while loop
            {"4", {"5", "6"}},
            {"5", {}},
            {"6", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }
}

TEST_CASE("Uses/Modifies Abstractions") {
  std::string sourceCode = "procedure foo {"
                           "	while ((x == m) && (y == n)) {"         // 1
                           "		print a;"                       // 2
                           "		b = a + d * 2;"                 // 3
                           "		while ((p == q) || (r == s)) {" // 4
                           "			read c;"                // 5
                           "			d = c - b / 4;"         // 6
                           "		}"
                           "	}"
                           "}";

  PKB pkb = PKB();
  std::shared_ptr<PKBSetter> setter =
      std::make_shared<PKBSetter>(PKBSetter(pkb));
  std::shared_ptr<PKBGetter> getter =
      std::make_shared<PKBGetter>(PKBGetter(pkb));
  SourceProcessor SP = SourceProcessor(setter);
  SP.processCode(sourceCode);
  setter->done();

  SECTION("UsesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"1",
             {"x", "m", "y", "n", "a", "d", "p", "q", "r", "s", "c",
              "b"}},                               // container
            {"2", {"a"}},                          // print
            {"3", {"a", "d"}},                     // assignment
            {"4", {"p", "q", "r", "s", "c", "b"}}, // container
            {"5", {}},                             // -
            {"6", {"c", "b"}},                     // assignment
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("UsesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesP();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"foo",
             {"x", "m", "y", "n", "a", "d", "p", "q", "r", "s", "c", "b"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"1", {"b", "c", "d"}}, // container
            {"3", {"b"}},           // assignment
            {"4", {"c", "d"}},      // container
            {"5", {"c"}},           // read
            {"6", {"d"}},           // assignment
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesP();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"foo", {"b", "c", "d"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }
}

TEST_CASE("Uses/Modifies Abstractions ONLY CONSTANTS") {
  std::string sourceCode = "procedure foo {"
                           "	while ((1 == 2) && (2 == 3)) {"         // 1
                           "		b = 2;"                         // 2
                           "		while ((4 == 5) || (5 == 5)) {" // 3
                           "			d = 7 - 8 / 4;"         // 4
                           "		}"
                           "	}"
                           "}";

  PKB pkb = PKB();
  std::shared_ptr<PKBSetter> setter =
      std::make_shared<PKBSetter>(PKBSetter(pkb));
  std::shared_ptr<PKBGetter> getter =
      std::make_shared<PKBGetter>(PKBGetter(pkb));
  SourceProcessor SP = SourceProcessor(setter);
  SP.processCode(sourceCode);
  setter->done();

  SECTION("UsesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"1", {}}, // container
            {"2", {}}, // assignment
            {"3", {}}, // container
            {"4", {}}, // assignment
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }
  SECTION("UsesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getUsesP();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"foo", {}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesS();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"1", {"b", "d"}}, // container
            {"2", {"b"}},      // assignment
            {"3", {"d"}},      // container
            {"4", {"d"}},      // assignment
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }

  SECTION("ModifiesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        actualAbstractions = getter->getModifiesP();
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedAbstractions = {
            {"foo", {"b", "d"}},
        };
    REQUIRE(actualAbstractions == expectedAbstractions);
  }
}
