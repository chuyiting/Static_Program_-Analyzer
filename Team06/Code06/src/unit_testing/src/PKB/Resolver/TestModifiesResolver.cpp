//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/Resolver/EntityResolver/SideEffect/ModifiesPResolver.h"
#include "PKB/Resolver/EntityResolver/SideEffect/ModifiesSResolver.h"
#include "StubModifiesStorage.h"
#include "catch.hpp"

TEST_CASE("modifies_1") {
  StubModifiesStorage ModifiesStorage = StubModifiesStorage();
  ModifiesSResolver s = ModifiesSResolver(ModifiesStorage);
  ModifiesPResolver p = ModifiesPResolver(ModifiesStorage);

  REQUIRE(s.run("2", "x", ""));

  REQUIRE_FALSE(s.run("2", "s", ""));

  REQUIRE_FALSE(s.run("3", "x", ""));

  REQUIRE(p.run("foo", "x", ""));

  REQUIRE_FALSE(p.run("foo", "b", ""));
}
