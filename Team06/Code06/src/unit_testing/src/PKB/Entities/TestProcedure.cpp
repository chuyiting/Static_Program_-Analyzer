//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "catch.hpp"

// procedures, callStmts (currently not used by QPS)
TEST_CASE("procedure_insertion_retrieval") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);

  setter.addProcedure("foo", {"1", "2", "3"});
  setter.addProcedure("bar", {"4", "5"});
  setter.addProcedure("boo", {"6", "7", "8"});

  std::unordered_set<Procedure> procedures = getter.getProcedures();

  SECTION("addProcedureStmts_positive") {
    std::unordered_set<Statement> expectedProcedures = {"foo", "bar", "boo"};
    REQUIRE(procedures == expectedProcedures);
  }

  SECTION("addProcedureStmts_negative") {
    std::unordered_set<Statement> expectedProcedures = {"bee"};
    REQUIRE_FALSE(procedures == expectedProcedures);
  }
}