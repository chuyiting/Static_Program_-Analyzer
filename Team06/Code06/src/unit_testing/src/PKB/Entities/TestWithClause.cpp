//
// Created by Stephen Tan  Hin Khai on 31/10/22.
//
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "PKB/type/pkb_types.h"
#include "catch.hpp"
#include "unordered_map"
#include "unordered_set"
using namespace pkb_types;
using stmts = std::unordered_set<Statement>;

/**
 *
 * 1. procedure p { if (1 == 2) then {
 * 2.      while (3 == 4) {
 * 3.          x = 1; } }
 *    else {
 * 4.      if (x == 1) then {
 * 5.          while (y == x) {
 * 6.              z = 2; } }
        else {
 * 7.          y = 3; }
 * 8.     call foo } } }
 *
 * 9. procedure foo { print v;
 * 10. read x;
 * 11. print v;}
 */
TEST_CASE("With Clause") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);
  setter.addProcedure("p", stmts({"1", "2", "3", "4", "5", "6", "7"}));
  setter.addProcedure("foo", stmts({"9", "10"}));
  setter.addConstant("1");
  setter.addConstant("2");
  setter.addConstant("3");
  setter.addConstant("4");
  setter.addIfStmt("1");
  setter.addIfStmt("4");
  setter.addWhileStmt("2");
  setter.addWhileStmt("5");
  setter.addAssignmentStmt("3", "x");
  setter.addAssignmentStmt("6", "z");
  setter.addAssignmentStmt("7", "y");
  setter.addCallStmt("8", "foo");
  setter.addPrintStmt("9", "v");
  setter.addReadStmt("10", "x");
  setter.addPrintStmt("11", "v");

  SECTION("With Clause call.procName") {
    stmts stored = getter.getCallsByCalledProcName("foo");
    REQUIRE(stored == stmts({"8"}));
  }

  SECTION("WithClause read.varName") {
    stmts stored = getter.getReadsByVarName("x");
    REQUIRE(stored == stmts({"10"}));
  }

  SECTION("WithClause print.varName") {
    stmts stored = getter.getPrintsByVarName("x");
    REQUIRE(stored == stmts({}));

    // duplicate prints
    stored = getter.getPrintsByVarName("v");
    REQUIRE(stored == stmts({"9", "11"}));
  }

  SECTION("doesConstantExist") {
    REQUIRE(getter.doesConstantExist("3"));
    REQUIRE(getter.doesConstantExist("4"));

    // non existant constant
    REQUIRE_FALSE(getter.doesConstantExist("12"));
  }

  SECTION("isAssignmentStatement") {
    REQUIRE(getter.isAssignmentStatement("3"));
    REQUIRE(getter.isAssignmentStatement("6"));
    // other statement types
    REQUIRE_FALSE(getter.isAssignmentStatement("2"));
    REQUIRE_FALSE(getter.isAssignmentStatement("8"));
    // non existant statement
    REQUIRE_FALSE(getter.isAssignmentStatement("12"));
  }

  SECTION("isIfStatement") {
    REQUIRE(getter.isIfStatement("1"));
    REQUIRE(getter.isIfStatement("4"));
    REQUIRE_FALSE(getter.isIfStatement("2"));
    REQUIRE_FALSE(getter.isIfStatement("8"));
    // non existant statement
    REQUIRE_FALSE(getter.isIfStatement("12"));
  }

  SECTION("isWhileStatement") {
    REQUIRE(getter.isWhileStatement("2"));
    REQUIRE(getter.isWhileStatement("5"));
    REQUIRE_FALSE(getter.isWhileStatement("1"));
    REQUIRE_FALSE(getter.isWhileStatement("8"));
    // non existant statement
    REQUIRE_FALSE(getter.isWhileStatement("12"));
  }

  SECTION("isReadStatement") {
    REQUIRE(getter.isReadStatement("10"));
    // other statement types
    REQUIRE_FALSE(getter.isReadStatement("1"));
    REQUIRE_FALSE(getter.isReadStatement("8"));
    // non existant statement
    REQUIRE_FALSE(getter.isReadStatement("12"));
  }

  SECTION("isPrintStatement") {
    REQUIRE(getter.isPrintStatement("9"));
    REQUIRE(getter.isPrintStatement("11"));
    // other statement types
    REQUIRE_FALSE(getter.isPrintStatement("1"));
    REQUIRE_FALSE(getter.isPrintStatement("8"));
    // non existant statement
    REQUIRE_FALSE(getter.isPrintStatement("12"));
  }

  SECTION("isCallStatement") {
    REQUIRE(getter.isCallStatement("8"));
    // other statement types
    REQUIRE_FALSE(getter.isCallStatement("1"));
    REQUIRE_FALSE(getter.isCallStatement("2"));
    // non existant statement
    REQUIRE_FALSE(getter.isCallStatement("12"));
  }
}
