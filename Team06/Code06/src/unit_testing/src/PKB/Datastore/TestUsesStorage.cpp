#include "PKB/Datastore/UsesStorage.h"
#include "catch.hpp"
#include <unordered_map>
#include <unordered_set>

TEST_CASE("UsesS") {
  UsesStorage uses_storage = UsesStorage();
  uses_storage.addUsesS("1", {});
  uses_storage.addUsesS("2", {});
  uses_storage.addUsesS("3", {});
  uses_storage.addUsesS("4", {});
  uses_storage.addUsesS("5", {"x", "y", "count", "cenX", "cenY"});
  uses_storage.addUsesS("5", {"count", "cenX", "cenY"});
  uses_storage.addUsesS("6", {"count"});
  uses_storage.addUsesS("7", {"cenX"});
  uses_storage.addUsesS("8", {"cenY"});
  uses_storage.addUsesS("9", {});
  uses_storage.addUsesS("10", {"count", "cenX", "count", "cenY"});
  uses_storage.addUsesS("11", {});
  uses_storage.addUsesS("12", {"cenX", "count"});
  uses_storage.addUsesS("13", {"cenY", "count"});
  uses_storage.addUsesS("14", {"cenX", "cenY"});
  std::unordered_map<Statement, std::unordered_set<Variable>> usesS =
      uses_storage.getUsesS();

  SECTION("Positive test for addUsesS") {
    std::unordered_map<Statement, std::unordered_set<Variable>> expectedUsesS =
        {
            {"1", {}},
            {"2", {}},
            {"3", {}},
            {"4", {}},
            {"5", {"x", "y", "count", "cenX", "cenY"}},
            {"6", {"count"}},
            {"7", {"cenX"}},
            {"8", {"cenY"}},
            {"9", {}},
            {"10", {"count", "cenX", "count", "cenY"}},
            {"11", {}},
            {"12", {"cenX", "count"}},
            {"13", {"cenY", "count"}},
            {"14", {"cenX", "cenY"}},
        };
    REQUIRE(usesS == expectedUsesS);
  }

  SECTION("Tests for getStatementUses") {
    std::unordered_map<Statement, std::unordered_set<Variable>> expectedUsesS =
        {
            {"1", {}},
            {"2", {}},
            {"3", {}},
            {"4", {}},
            {"5", {"x", "y", "count", "cenX", "cenY"}},
            {"6", {"count"}},
            {"7", {"cenX"}},
            {"8", {"cenY"}},
            {"9", {}},
            {"10", {"count", "cenX", "count", "cenY"}},
            {"11", {}},
            {"12", {"cenX", "count"}},
            {"13", {"cenY", "count"}},
            {"14", {"cenX", "cenY"}},
        };
    auto i = uses_storage.getStatementUses("5");

    REQUIRE(uses_storage.getStatementUses("1") == expectedUsesS["1"]);
    REQUIRE(uses_storage.getStatementUses("0") == expectedUsesS["1"]);
    REQUIRE(uses_storage.getStatementUses("-1") == expectedUsesS["1"]);
    REQUIRE(uses_storage.getStatementUses("5") == expectedUsesS["5"]);
    REQUIRE(uses_storage.getStatementUses("14") == expectedUsesS["14"]);
  }

  SECTION("Negative test for addUsesS") {
    UsesStorage us = UsesStorage();
    us.addUsesS("1", {"a"});
    us.addUsesS("1", {"b"});
    us.addUsesS("1", {"c"});

    std::unordered_map<Statement, std::unordered_set<Variable>>
        nonExpectedUsesS = {{"1", {"c"}}};

    REQUIRE(us.getUsesS() != nonExpectedUsesS);
  }

  SECTION("Positive test for isUsesS") {
    REQUIRE(uses_storage.isUsesS("5", "x"));
    REQUIRE(uses_storage.isUsesS("5", "y"));
    REQUIRE(uses_storage.isUsesS("5", "count"));
    REQUIRE(uses_storage.isUsesS("5", "cenX"));
    REQUIRE(uses_storage.isUsesS("5", "cenY"));
    REQUIRE(uses_storage.isUsesS("6", "count"));
    REQUIRE(uses_storage.isUsesS("7", "cenX"));
    REQUIRE(uses_storage.isUsesS("8", "cenY"));
    REQUIRE(uses_storage.isUsesS("10", "count"));
    REQUIRE(uses_storage.isUsesS("10", "cenX"));
    REQUIRE(uses_storage.isUsesS("10", "cenY"));
    REQUIRE(uses_storage.isUsesS("12", "cenX"));
    REQUIRE(uses_storage.isUsesS("12", "count"));
    REQUIRE(uses_storage.isUsesS("13", "cenY"));
    REQUIRE(uses_storage.isUsesS("13", "count"));
    REQUIRE(uses_storage.isUsesS("14", "cenX"));
    REQUIRE(uses_storage.isUsesS("14", "cenY"));
  }

  SECTION("Negative test for isUsesS") {
    REQUIRE_FALSE(uses_storage.isUsesS("1", "x"));
    REQUIRE_FALSE(uses_storage.isUsesS("2", "x"));
    REQUIRE_FALSE(uses_storage.isUsesS("3", "x"));
    REQUIRE_FALSE(uses_storage.isUsesS("4", "x"));
    REQUIRE_FALSE(uses_storage.isUsesS("9", "x"));
    REQUIRE_FALSE(uses_storage.isUsesS("11", "x"));
    REQUIRE_FALSE(uses_storage.isUsesS("100", "y"));
  }

  SECTION("Positive test for isUsesSRight") {
    REQUIRE(uses_storage.isUsesSRight("5"));
    REQUIRE(uses_storage.isUsesSRight("6"));
    REQUIRE(uses_storage.isUsesSRight("7"));
    REQUIRE(uses_storage.isUsesSRight("8"));
    REQUIRE(uses_storage.isUsesSRight("10"));
    REQUIRE(uses_storage.isUsesSRight("12"));
    REQUIRE(uses_storage.isUsesSRight("13"));
    REQUIRE(uses_storage.isUsesSRight("14"));
  }

  SECTION("Negative test for isUsesSRight") {
    REQUIRE_FALSE(uses_storage.isUsesSRight("1"));
    REQUIRE_FALSE(uses_storage.isUsesSRight("2"));
    REQUIRE_FALSE(uses_storage.isUsesSRight("3"));
    REQUIRE_FALSE(uses_storage.isUsesSRight("4"));
    REQUIRE_FALSE(uses_storage.isUsesSRight("9"));
    REQUIRE_FALSE(uses_storage.isUsesSRight("11"));
    REQUIRE_FALSE(uses_storage.isUsesSRight("100"));
  }
}

TEST_CASE("UsesP") {
  UsesStorage uses_storage = UsesStorage();
  uses_storage.addUsesP("computeCentroid", {"x", "y", "count", "cenX", "cenY"});
  uses_storage.addUsesP("main", {"x", "y", "count", "cenX", "cenY"});
  uses_storage.addUsesP("main", {"flag", "cenX", "cenY", "normSq"});
  uses_storage.addUsesP("readPoint", {});
  uses_storage.addUsesP("printResults", {"flag", "cenX", "cenY", "normSq"});
  std::unordered_map<Procedure, std::unordered_set<Variable>> usesP =
      uses_storage.getUsesP();

  SECTION("Positive test for addUsesP") {
    std::unordered_map<Statement, std::unordered_set<Variable>> expectedUsesP =
        {
            {"computeCentroid", {"x", "y", "count", "cenX", "cenY"}},
            {"main", {"x", "y", "count", "cenX", "cenY", "flag", "normSq"}},
            {"readPoint", {}},
            {"printResults", {"flag", "cenX", "cenY", "normSq"}},
        };
    REQUIRE(usesP == expectedUsesP);
  }

  SECTION("Negative test for addUsesP") {
    UsesStorage us = UsesStorage();
    us.addUsesP("count", {"a"});
    us.addUsesP("count", {"b"});
    us.addUsesP("count", {"c"});

    std::unordered_map<Statement, std::unordered_set<Variable>>
        nonExpectedUsesP = {{"count", {"c"}}};

    REQUIRE(us.getUsesP() != nonExpectedUsesP);
  }

  SECTION("tests for getProcedureUses") {
    std::unordered_map<Statement, std::unordered_set<Variable>> expectedUsesP =
        {
            {"computeCentroid", {"x", "y", "count", "cenX", "cenY"}},
            {"main", {"x", "y", "count", "cenX", "cenY", "flag", "normSq"}},
            {"readPoint", {}},
            {"printResults", {"flag", "cenX", "cenY", "normSq"}},
        };
    REQUIRE(uses_storage.getProcedureUses("readPoint") ==
            expectedUsesP["readPoint"]);
    REQUIRE(uses_storage.getProcedureUses("printResults") ==
            expectedUsesP["printResults"]);
    REQUIRE(uses_storage.getProcedureUses("main") == expectedUsesP["main"]);
    REQUIRE(uses_storage.getProcedureUses("foo").empty());
  }

  SECTION("Positive test for isUsesP") {
    REQUIRE(uses_storage.isUsesP("computeCentroid", "x"));
    REQUIRE(uses_storage.isUsesP("computeCentroid", "y"));
    REQUIRE(uses_storage.isUsesP("computeCentroid", "count"));
    REQUIRE(uses_storage.isUsesP("computeCentroid", "cenX"));
    REQUIRE(uses_storage.isUsesP("computeCentroid", "cenY"));
    REQUIRE(uses_storage.isUsesP("main", "x"));
    REQUIRE(uses_storage.isUsesP("main", "y"));
    REQUIRE(uses_storage.isUsesP("main", "count"));
    REQUIRE(uses_storage.isUsesP("main", "cenX"));
    REQUIRE(uses_storage.isUsesP("main", "cenY"));
    REQUIRE(uses_storage.isUsesP("main", "flag"));
    REQUIRE(uses_storage.isUsesP("main", "normSq"));
    REQUIRE(uses_storage.isUsesP("printResults", "flag"));
    REQUIRE(uses_storage.isUsesP("printResults", "cenX"));
    REQUIRE(uses_storage.isUsesP("printResults", "cenY"));
    REQUIRE(uses_storage.isUsesP("printResults", "normSq"));
  }

  SECTION("Negative test for isUsesP") {
    REQUIRE_FALSE(uses_storage.isUsesP("readPoint", "x"));
    REQUIRE_FALSE(uses_storage.isUsesP("main", "lala"));
  }

  SECTION("Positive test for isUsesPRight") {
    REQUIRE(uses_storage.isUsesPRight("computeCentroid"));
    REQUIRE(uses_storage.isUsesPRight("main"));
    REQUIRE(uses_storage.isUsesPRight("printResults"));
  }

  SECTION("Negative test for isUsesPRight") {
    REQUIRE_FALSE(uses_storage.isUsesPRight("readPoint"));
  }
}
