//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "catch.hpp"

// read, print, assign, if stmts
TEST_CASE("read_insertion_retrieval") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);
  std::string readVar = "x";

  setter.addReadStmt("3", readVar);
  setter.addReadStmt("1", readVar);
  setter.addReadStmt("2", readVar);
  setter.addReadStmt("5", readVar);
  setter.addReadStmt("8", readVar);
  setter.addReadStmt("10", readVar);
  setter.addReadStmt("12", readVar);
  setter.addReadStmt("60", readVar);
  setter.addReadStmt("100", readVar);

  std::unordered_set<Statement> readStmts = getter.getReadStmts();

  SECTION("addReadStmts_positive") {
    std::unordered_set<Statement> expectedReadStmts = {
        "3", "1", "2", "5", "8", "10", "12", "60", "100"};
    REQUIRE(readStmts == expectedReadStmts);
  }

  SECTION("addReadStmts_negative") {
    std::unordered_set<Statement> expectedReadStmts = {"3", "1", "2"};
    REQUIRE_FALSE(readStmts == expectedReadStmts);
  }
}

TEST_CASE("print_insertion_retrieval") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);
  std::string printedVar = "x";

  setter.addPrintStmt("3", printedVar);
  setter.addPrintStmt("1", printedVar);
  setter.addPrintStmt("2", printedVar);
  setter.addPrintStmt("5", printedVar);
  setter.addPrintStmt("8", printedVar);
  setter.addPrintStmt("10", printedVar);
  setter.addPrintStmt("12", printedVar);
  setter.addPrintStmt("60", printedVar);
  setter.addPrintStmt("100", printedVar);

  std::unordered_set<Statement> printStmts = getter.getPrintStmts();

  SECTION("addPrintStmts_positive") {
    std::unordered_set<Statement> expectedPrintStmts = {
        "3", "1", "2", "5", "8", "10", "12", "60", "100"};
    REQUIRE(printStmts == expectedPrintStmts);
  }

  SECTION("addPrintStmts_negative") {
    std::unordered_set<Statement> expectedPrintStmts = {"3", "1", "2"};
    REQUIRE_FALSE(printStmts == expectedPrintStmts);
  }
}

TEST_CASE("assign_insertion_retrieval") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);
  std::string assignedVar = "x";

  setter.addAssignmentStmt("3", assignedVar);
  setter.addAssignmentStmt("1", assignedVar);
  setter.addAssignmentStmt("2", assignedVar);
  setter.addAssignmentStmt("5", assignedVar);
  setter.addAssignmentStmt("8", assignedVar);
  setter.addAssignmentStmt("10", assignedVar);
  setter.addAssignmentStmt("12", assignedVar);
  setter.addAssignmentStmt("60", assignedVar);
  setter.addAssignmentStmt("100", assignedVar);

  std::unordered_set<Statement> assignStmts = getter.getAssignStmts();

  SECTION("addAssignStmts_positive") {
    std::unordered_set<Statement> expectedAssignStmts = {
        "3", "1", "2", "5", "8", "10", "12", "60", "100"};
    REQUIRE(assignStmts == expectedAssignStmts);
  }

  SECTION("addAssignStmts_negative") {
    std::unordered_set<Statement> expectedAssignStmts = {"3", "1", "2"};
    REQUIRE_FALSE(assignStmts == expectedAssignStmts);
  }
}

TEST_CASE("if_insertion_retrieval") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);

  setter.addIfStmt("3");
  setter.addIfStmt("1");
  setter.addIfStmt("2");
  setter.addIfStmt("5");
  setter.addIfStmt("8");
  setter.addIfStmt("10");
  setter.addIfStmt("12");
  setter.addIfStmt("60");
  setter.addIfStmt("100");

  std::unordered_set<Statement> ifStmts = getter.getIfStmts();

  SECTION("addIfStmts_positive") {
    std::unordered_set<Statement> expectedIfStmts = {
        "3", "1", "2", "5", "8", "10", "12", "60", "100"};
    REQUIRE(ifStmts == expectedIfStmts);
  }

  SECTION("addIfStmts_negative") {
    std::unordered_set<Statement> expectedIfStmts = {"3", "1", "2"};
    REQUIRE_FALSE(ifStmts == expectedIfStmts);
  }
}