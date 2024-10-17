//
// Created by Chong Jun Wei on 9/10/22.
//
//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/Datastore/ParentStorage.h"
#include "PKB/Resolver/StatementResolver/Container/ParentResolver.h"
#include "PKB/Resolver/StatementResolver/Container/ParentTResolver.h"
#include "catch.hpp"

using tuple = std::pair<std::string, std::unordered_set<std::string>>;
using parentOf = std::unordered_set<std::string>;

TEST_CASE("Parents_1") {
  ParentStorage s = ParentStorage();
  ParentResolver p = ParentResolver(s);
  ParentTResolver pt = ParentTResolver(s);
  s.addParent("1", {"2"});

  REQUIRE(p.run("1", "2", ""));

  REQUIRE_FALSE(p.run("2", "1", ""));

  REQUIRE_FALSE(p.run("1", "3", ""));

  REQUIRE(pt.run("1", "2", ""));

  REQUIRE_FALSE(pt.run("2", "1", ""));

  REQUIRE_FALSE(pt.run("1", "3", ""));

}

TEST_CASE("Parents_2 Tests") {
  ParentStorage f = ParentStorage();
  ParentResolver p = ParentResolver(f);
  ParentTResolver pt = ParentTResolver(f);
  f.addParent("1", {"2"});
  f.addParent("5", {"6", "7"});
  f.addParent("7", {"8"});
  f.addParent("8", {"9"});

  parentOf s = {"2"};

  SECTION("direct/T") {
	REQUIRE(p.run("1", "2", ""));
	REQUIRE(p.run("5", "7", ""));
	// expected false
	REQUIRE_FALSE(p.run("2", "1", ""));
	REQUIRE_FALSE(p.run("1", "5", ""));
	// parent T
	REQUIRE(pt.run("1", "2", ""));
	REQUIRE(pt.run("5", "7", ""));

	// expected false
	REQUIRE_FALSE(pt.run("2", "1", ""));
	REQUIRE_FALSE(pt.run("23", "1", ""));
	REQUIRE_FALSE(pt.run("1", "5", ""));


  }

  SECTION("Wildcard right") {
	REQUIRE(p.run("1", "_", ""));
	REQUIRE(p.run("8", "_", ""));
	REQUIRE_FALSE(p.run("9", "_", ""));
	// parent T
	REQUIRE(pt.run("1", "_", ""));
	REQUIRE(pt.run("8", "_", ""));
	REQUIRE_FALSE(pt.run("9", "_", ""));

  }

  SECTION("Wildcard left") {
	REQUIRE_FALSE(p.run("_", "1", ""));
	REQUIRE_FALSE(p.run("_", "5", ""));
	// parent T
	REQUIRE_FALSE(pt.run("_", "1", ""));
	REQUIRE(pt.run("_", "8", ""));
  }

  SECTION("Wildcard, Wildcard") {
	REQUIRE(pt.run("_", "_", ""));
	REQUIRE(p.run("_", "_", ""));
  }

}
