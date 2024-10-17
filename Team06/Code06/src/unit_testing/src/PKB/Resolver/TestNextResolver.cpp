#include "PKB/Datastore/NextStorage.h"
#include "PKB/Resolver/StatementResolver/CFG/NextResolver.h"
#include "catch.hpp"

TEST_CASE("Next Resolver") {
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

  NextResolver next = NextResolver(next_storage);

  SECTION("Positive: run(numeric_ref, numeric_ref)") {
    REQUIRE(next.run("1", "2", ""));
    REQUIRE(next.run("2", "3", ""));
    REQUIRE(next.run("3", "4", ""));
    REQUIRE(next.run("4", "5", ""));
    REQUIRE(next.run("5", "6", ""));
    REQUIRE(next.run("6", "3", ""));
    REQUIRE(next.run("3", "7", ""));
    REQUIRE(next.run("7", "8", ""));
    REQUIRE(next.run("7", "9", ""));
    REQUIRE(next.run("8", "10", ""));
    REQUIRE(next.run("9", "10", ""));
    REQUIRE(next.run("10", "11", ""));
    REQUIRE(next.run("11", "12", ""));
  }

  SECTION("Negative: run(numeric_ref, numeric_ref)") {
    REQUIRE_FALSE(next.run("2", "1", ""));
    REQUIRE_FALSE(next.run("4", "3", ""));
    REQUIRE_FALSE(next.run("5", "3", ""));
    REQUIRE_FALSE(next.run("4", "7", ""));
	REQUIRE_FALSE(next.run("3", "6", ""));

    REQUIRE_FALSE(next.run("5", "7", ""));

    REQUIRE_FALSE(next.run("8", "9", ""));

    REQUIRE_FALSE(next.run("9", "8", ""));
    REQUIRE_FALSE(next.run("7", "10", ""));
  }

  SECTION("Positive: run(numeric_ref, wildcard)") {

    REQUIRE(next.run("1", "_", ""));

    REQUIRE(next.run("2", "_", ""));

    REQUIRE(next.run("3", "_", ""));

    REQUIRE(next.run("4", "_", ""));

    REQUIRE(next.run("5", "_", ""));

    REQUIRE(next.run("6", "_", ""));

    REQUIRE(next.run("7", "_", ""));

    REQUIRE(next.run("8", "_", ""));

    REQUIRE(next.run("9", "_", ""));

    REQUIRE(next.run("10", "_", ""));

    REQUIRE(next.run("11", "_", ""));
  }

  SECTION("Negative: run(numeric_ref, wildcard)") {

    REQUIRE_FALSE(next.run("12", "_", ""));
  }

  SECTION("Positive: run(wildcard, numeric_ref)") {

    REQUIRE(next.run("_", "2", ""));

    REQUIRE(next.run("_", "3", ""));

    REQUIRE(next.run("_", "4", ""));

    REQUIRE(next.run("_", "5", ""));

    REQUIRE(next.run("_", "6", ""));

    REQUIRE(next.run("_", "7", ""));

    REQUIRE(next.run("_", "8", ""));

    REQUIRE(next.run("_", "9", ""));

    REQUIRE(next.run("_", "10", ""));

    REQUIRE(next.run("_", "11", ""));

    REQUIRE(next.run("_", "12", ""));
  }

  SECTION("Negative: run(wildcard, numeric_ref)") {

    REQUIRE_FALSE(next.run("_", "1", ""));
  }

  SECTION("Positive: run(wildcard, wildcard)") {

    REQUIRE(next.run("_", "_", ""));
  }

  SECTION("Negative: run(wildcard, wildcard)") {
    NextStorage ns = NextStorage();
    NextResolver n = NextResolver(ns);
    REQUIRE_FALSE(n.run("_", "_", ""));
  }
}
