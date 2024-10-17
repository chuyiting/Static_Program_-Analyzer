//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "catch.hpp"

// variables
TEST_CASE("variable_insertion_retrieval") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);

  SECTION("single_var") {
    Variable a = "a";

    // Insert variable into PKB
    setter.addVariable(a);

    // Check insertion success
    std::unordered_set<Variable> storedVariables = getter.getVariables();
    REQUIRE(storedVariables.find(a) != storedVariables.end());
  }

  SECTION("multiple_vars") {
    Variable a = "a";
    Variable b = "b";
    Variable c = "c";

    // Insert variables into PKB
    setter.addVariable(a);
    setter.addVariable(b);
    setter.addVariable(c);

    // Check insertion success
    std::unordered_set<Variable> storedVariables = getter.getVariables();
    REQUIRE(storedVariables.find(a) != storedVariables.end());
    REQUIRE(storedVariables.find(b) != storedVariables.end());
    REQUIRE(storedVariables.find(c) != storedVariables.end());

    // Check variable retrieval
    std::unordered_set<Variable> expectedVariables = {"a", "b", "c"};
    REQUIRE(storedVariables == expectedVariables);
  }

  SECTION("negative") {
    Variable a = "a";
    Variable b = "b";
    Variable c = "c";

    // Insert variables into PKB
    setter.addVariable(a);
    setter.addVariable(b);
    setter.addVariable(c);

    // Check insertion success
    std::unordered_set<Variable> storedVariables = getter.getVariables();
    REQUIRE(storedVariables.find(a) != storedVariables.end());
    REQUIRE(storedVariables.find(b) != storedVariables.end());
    REQUIRE(storedVariables.find(c) != storedVariables.end());

    // Check variable retrieval
    std::unordered_set<Variable> expectedVariables = {"a", "b", "d"};
    REQUIRE_FALSE(storedVariables == expectedVariables);
  }
}