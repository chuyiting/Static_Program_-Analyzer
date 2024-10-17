#include "PKB/Datastore/ModifiesStorage.h"
#include "catch.hpp"
#include <unordered_map>
#include <unordered_set>
#include <set>

TEST_CASE("ModifiesS") {
  ModifiesStorage modifies_storage = ModifiesStorage();
  modifies_storage.addModifiesS("1", "count");
  modifies_storage.addModifiesS("2", "cenX");
  modifies_storage.addModifiesS("3", "cenY");
  modifies_storage.addModifiesS("4", "x");
  modifies_storage.addModifiesS("4", "y");
  modifies_storage.addModifiesS("5", "count");
  modifies_storage.addModifiesS("5", "cenX");
  modifies_storage.addModifiesS("5", "cenY");
  modifies_storage.addModifiesS("5", "x");
  modifies_storage.addModifiesS("5", "y");
  modifies_storage.addModifiesS("6", "count");
  modifies_storage.addModifiesS("7", "cenX");
  modifies_storage.addModifiesS("8", "cenY");
  modifies_storage.addModifiesS("9", "x");
  modifies_storage.addModifiesS("9", "y");
  modifies_storage.addModifiesS("10", "flag");
  modifies_storage.addModifiesS("10", "cenX");
  modifies_storage.addModifiesS("10", "cenY");
  modifies_storage.addModifiesS("11", "flag");
  modifies_storage.addModifiesS("12", "cenX");
  modifies_storage.addModifiesS("13", "cenY");
  modifies_storage.addModifiesS("14", "normSq");
  std::unordered_map<Statement, std::unordered_set<Variable>> modifiesS =
      modifies_storage.getModifiesS();

  SECTION("Positive test for addModifiesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedModifiesS = {
            {"1", {"count"}},
            {"2", {"cenX"}},
            {"3", {"cenY"}},
            {"4", {"x", "y"}},
            {"5", {"count", "cenX", "cenY", "x", "y"}},
            {"6", {"count"}},
            {"7", {"cenX"}},
            {"8", {"cenY"}},
            {"9", {"x", "y"}},
            {"10", {"flag", "cenX", "cenY"}},
            {"11", {"flag"}},
            {"12", {"cenX"}},
            {"13", {"cenY"}},
            {"14", {"normSq"}},
        };
    REQUIRE(modifiesS == expectedModifiesS);
  }

  SECTION("Negative test for addModifiesS") {
    ModifiesStorage ms = ModifiesStorage();
    ms.addModifiesS("1", "a");
    ms.addModifiesS("1", "b");
    ms.addModifiesS("1", "c");

    std::unordered_map<Statement, std::unordered_set<Variable>>
        nonExpectedModifiesS = {{"1", {"c"}}};

    REQUIRE(ms.getModifiesS() != nonExpectedModifiesS);
  }

	SECTION("Tests for getModifiesS") {
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedModifiesS = {
		{"1", {"count"}},
		{"2", {"cenX"}},
		{"3", {"cenY"}},
		{"4", {"x", "y"}},
		{"5", {"count", "cenX", "cenY", "x", "y"}},
		{"6", {"count"}},
		{"7", {"cenX"}},
		{"8", {"cenY"}},
		{"9", {"x", "y"}},
		{"10", {"flag", "cenX", "cenY"}},
		{"11", {"flag"}},
		{"12", {"cenX"}},
		{"13", {"cenY"}},
		{"14", {"normSq"}},
		};
		REQUIRE(modifies_storage.getStatementModifies("1") == expectedModifiesS["1"]);
		REQUIRE(modifies_storage.getStatementModifies("2") == expectedModifiesS["2"]);
		REQUIRE(modifies_storage.getStatementModifies("10") == expectedModifiesS["10"]);
		REQUIRE(modifies_storage.getStatementModifies("-1").empty());
	}

	SECTION("Positive test for isModifiesS") {
		REQUIRE(modifies_storage.isModifiesS("1", "count"));
		REQUIRE(modifies_storage.isModifiesS("2", "cenX"));
		REQUIRE(modifies_storage.isModifiesS("3", "cenY"));
		REQUIRE(modifies_storage.isModifiesS("4", "x"));
		REQUIRE(modifies_storage.isModifiesS("4", "y"));
		REQUIRE(modifies_storage.isModifiesS("5", "count"));
		REQUIRE(modifies_storage.isModifiesS("5", "cenX"));
		REQUIRE(modifies_storage.isModifiesS("5", "cenY"));
		REQUIRE(modifies_storage.isModifiesS("5", "x"));
		REQUIRE(modifies_storage.isModifiesS("5", "y"));
		REQUIRE(modifies_storage.isModifiesS("6", "count"));
		REQUIRE(modifies_storage.isModifiesS("7", "cenX"));
		REQUIRE(modifies_storage.isModifiesS("8", "cenY"));
		REQUIRE(modifies_storage.isModifiesS("9", "x"));
		REQUIRE(modifies_storage.isModifiesS("9", "y"));
		REQUIRE(modifies_storage.isModifiesS("10", "flag"));
		REQUIRE(modifies_storage.isModifiesS("10", "cenX"));
		REQUIRE(modifies_storage.isModifiesS("10", "cenY"));
		REQUIRE(modifies_storage.isModifiesS("11", "flag"));
		REQUIRE(modifies_storage.isModifiesS("12", "cenX"));
		REQUIRE(modifies_storage.isModifiesS("13", "cenY"));
		REQUIRE(modifies_storage.isModifiesS("14", "normSq"));
	}

	SECTION("Negative test for isModifiesS") {
		REQUIRE_FALSE(modifies_storage.isModifiesS("15", "x"));
		REQUIRE_FALSE(modifies_storage.isModifiesS("100", "y"));
	}

	SECTION("Positive test for isModifiesSRight") {
		REQUIRE(modifies_storage.isModifiesSRight("1"));
		REQUIRE(modifies_storage.isModifiesSRight("2"));
		REQUIRE(modifies_storage.isModifiesSRight("3"));
		REQUIRE(modifies_storage.isModifiesSRight("4"));
		REQUIRE(modifies_storage.isModifiesSRight("5"));
		REQUIRE(modifies_storage.isModifiesSRight("6"));
		REQUIRE(modifies_storage.isModifiesSRight("7"));
		REQUIRE(modifies_storage.isModifiesSRight("8"));
		REQUIRE(modifies_storage.isModifiesSRight("9"));
		REQUIRE(modifies_storage.isModifiesSRight("10"));
		REQUIRE(modifies_storage.isModifiesSRight("11"));
		REQUIRE(modifies_storage.isModifiesSRight("12"));
		REQUIRE(modifies_storage.isModifiesSRight("13"));
		REQUIRE(modifies_storage.isModifiesSRight("14"));
	}
	
	SECTION("Negative test for isModifiesSRight") {
		REQUIRE_FALSE(modifies_storage.isModifiesSRight("15"));
		REQUIRE_FALSE(modifies_storage.isModifiesSRight("100"));
	}
}

TEST_CASE("ModifiesP") {
  ModifiesStorage modifies_storage = ModifiesStorage();
  modifies_storage.addModifiesP(
      "computeCentroid", {"count", "cenX", "cenY", "x", "y", "flag", "normSq"});
  modifies_storage.addModifiesP("computeCentroid",
                                {"count", "cenX", "cenY", "x", "y"});
  modifies_storage.addModifiesP("computeCentroid", {"flag", "cenX", "cenY"});
  modifies_storage.addModifiesP("main", {"flag", "count", "cenX", "cenY"});
  modifies_storage.addModifiesP("main", {"cenY", "x", "y", "normSq"});
  modifies_storage.addModifiesP("readPoint", {"x", "y"});
  modifies_storage.addModifiesP("printResults", {});
  std::unordered_map<Procedure, std::unordered_set<Variable>> modifiesP =
      modifies_storage.getModifiesP();

  SECTION("Positive test for addModifiesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>>
        expectedModifiesP = {
            {"computeCentroid",
             {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}},
            {"main", {"flag", "count", "cenX", "cenY", "x", "y", "normSq"}},
            {"readPoint", {"x", "y"}},
            {"printResults", {}},
        };
    REQUIRE(modifiesP == expectedModifiesP);
  }

  SECTION("Negative test for addModifiesP") {
    ModifiesStorage ms = ModifiesStorage();
    ms.addModifiesP("count", {"a"});
    ms.addModifiesP("count", {"b"});
    ms.addModifiesP("count", {"c"});

    std::unordered_map<Statement, std::unordered_set<Variable>>
        nonExpectedModifiesP = {{"count", {"c"}}};

    REQUIRE(ms.getModifiesP() != nonExpectedModifiesP);
  }

	SECTION("tests for getModifiesProcedure") {
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedModifiesP = {
		{"computeCentroid", {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}},
		{"main", {"flag", "count", "cenX", "cenY", "x", "y", "normSq"}},
		{"readPoint", {"x","y"}},
		{"printResults", {}},
		};
		auto i = modifies_storage.getProcedureModifies("main");
		REQUIRE(modifies_storage.getProcedureModifies("foo").empty());
		REQUIRE(modifies_storage.getProcedureModifies("main") == expectedModifiesP["main"]);
		REQUIRE(modifies_storage.getProcedureModifies("printResults") == expectedModifiesP["printResults"]);
	}

	SECTION("tests for getVariableModifiedP") {
		std::unordered_map<Variable, std::unordered_set<Statement >> expectedModifiesP = {
		{"count", {"computeCentroid", "main"}},
		{"x", {"readPoint", "main",  "computeCentroid"}},
		{"y", {"readPoint", "main",  "computeCentroid"}},
		{"cenX", {"computeCentroid", "main"}},
		{"cenY", {"computeCentroid", "main"}},
		{"normSq", {"computeCentroid", "main"}},
		{"flag", {"main",  "computeCentroid"}},
		};
		REQUIRE(modifies_storage.getVariableModifiedP("foo").empty());
		REQUIRE(modifies_storage.getVariableModifiedP("count") == expectedModifiesP["count"]);
		REQUIRE(modifies_storage.getVariableModifiedP("y") == expectedModifiesP["y"]);
	}
	SECTION("Positive test for isModifiesP") {
		REQUIRE(modifies_storage.isModifiesP("computeCentroid", "count"));
		REQUIRE(modifies_storage.isModifiesP("computeCentroid", "cenX"));
		REQUIRE(modifies_storage.isModifiesP("computeCentroid", "cenY"));
		REQUIRE(modifies_storage.isModifiesP("computeCentroid", "x"));
		REQUIRE(modifies_storage.isModifiesP("computeCentroid", "y"));
		REQUIRE(modifies_storage.isModifiesP("computeCentroid", "flag"));
		REQUIRE(modifies_storage.isModifiesP("computeCentroid", "normSq"));
		REQUIRE(modifies_storage.isModifiesP("main", "flag"));
		REQUIRE(modifies_storage.isModifiesP("main", "count"));
		REQUIRE(modifies_storage.isModifiesP("main", "cenX"));
		REQUIRE(modifies_storage.isModifiesP("main", "cenY"));
		REQUIRE(modifies_storage.isModifiesP("main", "x"));
		REQUIRE(modifies_storage.isModifiesP("main", "y"));
		REQUIRE(modifies_storage.isModifiesP("readPoint", "x"));
		REQUIRE(modifies_storage.isModifiesP("readPoint", "y"));
	}

	SECTION("Negative test for isModifiesP") {
		REQUIRE_FALSE(modifies_storage.isModifiesP("printResults", "x"));
		REQUIRE_FALSE(modifies_storage.isModifiesP("main", "lala"));
	}

	SECTION("Positive test for isModifiesPRight") {
		REQUIRE(modifies_storage.isModifiesPRight("computeCentroid"));
		REQUIRE(modifies_storage.isModifiesPRight("main"));
		REQUIRE(modifies_storage.isModifiesPRight("readPoint"));
	}

	SECTION("Negative test for isModifiesPRight") {
		REQUIRE_FALSE(modifies_storage.isModifiesPRight("printResults"));
	}
}