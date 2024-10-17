#include "PKB/Datastore/NextStorage.h"
#include "catch.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>

TEST_CASE("Next Storage") {
  NextStorage next_storage = NextStorage();
  next_storage.addNext("1", "2");
  next_storage.addNext("2", "3");
  next_storage.addNext("3", "4");
  next_storage.addNext("4", "5");
  next_storage.addNext("5", "6");
  next_storage.addNext("6", "3");
  next_storage.addNext("3", "7");
  next_storage.addNext("7", "8");
  next_storage.addNext("7", "9");
  next_storage.addNext("8", "10");
  next_storage.addNext("9", "10");
  next_storage.addNext("10", "11");
  next_storage.addNext("11", "12");
  next_storage.addNext("12", "0");

  SECTION("Positive test for addNext") {
    std::unordered_map<Statement, std::unordered_set<Statement>> expectedNext =
        {{"1", {"2"}},  {"2", {"3"}},   {"3", {"4", "7"}}, {"4", {"5"}},
         {"5", {"6"}},  {"6", {"3"}},   {"7", {"8", "9"}}, {"8", {"10"}},
         {"9", {"10"}}, {"10", {"11"}}, {"11", {"12"}},    {"12", {"0"}}};
    REQUIRE(next_storage.getNext() == expectedNext);
  }

  SECTION("Negative test for addNext") {
    NextStorage ns = NextStorage();
    ns.addNext("1", "2");
    ns.addNext("1", "3");
    ns.addNext("1", "4");

    std::unordered_map<Statement, std::unordered_set<Variable>>
        nonExpectedNext = {{"1", {"4"}}};

    REQUIRE(ns.getNext() != nonExpectedNext);
  }

  SECTION("Positive test for isNextDirect") {
    REQUIRE(next_storage.isNextDirect("1", "2"));
    REQUIRE(next_storage.isNextDirect("2", "3"));
    REQUIRE(next_storage.isNextDirect("3", "4"));
    REQUIRE(next_storage.isNextDirect("4", "5"));
    REQUIRE(next_storage.isNextDirect("5", "6"));
    REQUIRE(next_storage.isNextDirect("6", "3"));
    REQUIRE(next_storage.isNextDirect("3", "7"));
    REQUIRE(next_storage.isNextDirect("7", "8"));
    REQUIRE(next_storage.isNextDirect("7", "9"));
    REQUIRE(next_storage.isNextDirect("8", "10"));
    REQUIRE(next_storage.isNextDirect("9", "10"));
    REQUIRE(next_storage.isNextDirect("10", "11"));
    REQUIRE(next_storage.isNextDirect("11", "12"));
  }

  SECTION("Negative test for isNextDirect") {
    REQUIRE_FALSE(next_storage.isNextDirect("2", "1"));
    REQUIRE_FALSE(next_storage.isNextDirect("4", "3"));
    REQUIRE_FALSE(next_storage.isNextDirect("5", "3"));
    REQUIRE_FALSE(next_storage.isNextDirect("4", "7"));
    REQUIRE_FALSE(next_storage.isNextDirect("5", "7"));
    REQUIRE_FALSE(next_storage.isNextDirect("8", "9"));
    REQUIRE_FALSE(next_storage.isNextDirect("9", "8"));
    REQUIRE_FALSE(next_storage.isNextDirect("7", "10"));
  }

  SECTION("Positive test for isNextRight") {
    REQUIRE(next_storage.isNextRight("1"));
    REQUIRE(next_storage.isNextRight("2"));
    REQUIRE(next_storage.isNextRight("3"));
    REQUIRE(next_storage.isNextRight("4"));
    REQUIRE(next_storage.isNextRight("5"));
    REQUIRE(next_storage.isNextRight("6"));
    REQUIRE(next_storage.isNextRight("7"));
    REQUIRE(next_storage.isNextRight("8"));
    REQUIRE(next_storage.isNextRight("9"));
    REQUIRE(next_storage.isNextRight("10"));
    REQUIRE(next_storage.isNextRight("11"));
  }

  SECTION("Negative test for isNextRight") {
    REQUIRE_FALSE(next_storage.isNextRight("12"));
  }

  SECTION("Positive test for isNextLeft") {
    REQUIRE(next_storage.isNextLeft("2"));
    REQUIRE(next_storage.isNextLeft("3"));
    REQUIRE(next_storage.isNextLeft("4"));
    REQUIRE(next_storage.isNextLeft("5"));
    REQUIRE(next_storage.isNextLeft("6"));
    REQUIRE(next_storage.isNextLeft("7"));
    REQUIRE(next_storage.isNextLeft("8"));
    REQUIRE(next_storage.isNextLeft("9"));
    REQUIRE(next_storage.isNextLeft("10"));
    REQUIRE(next_storage.isNextLeft("11"));
    REQUIRE(next_storage.isNextLeft("12"));
  }

  SECTION("Negative test for isNextLeft") {
    REQUIRE_FALSE(next_storage.isNextLeft("1"));
  }

  SECTION("Positive test for isNextExists") {
    REQUIRE(next_storage.isNextExists());
  }

  SECTION("Negative test for isNextExists") {
    NextStorage ns = NextStorage();
    REQUIRE_FALSE(ns.isNextExists());
  }
}