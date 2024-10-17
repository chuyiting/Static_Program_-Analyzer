//
// Created by Stephen Tan  Hin Khai on 7/10/22.
//
#include "PKB/Resolver/EntityResolver/SideEffect/UsesPResolver.h"
#include "PKB/Resolver/EntityResolver/SideEffect/UsesSResolver.h"
#include "StubUsesStorage.h"
#include "catch.hpp"

TEST_CASE("ONLY CONSTANTS") {
  StubUsesStorageOnlyConst usesStorage = StubUsesStorageOnlyConst();
  UsesPResolver usesP = UsesPResolver(usesStorage);
  UsesSResolver usesS = UsesSResolver(usesStorage);

  SECTION("UsesP Variables") {
    REQUIRE_FALSE(usesP.run("foo", "a", ""));
    REQUIRE_FALSE(usesP.run("foo", "b", ""));
  }

  SECTION("UsesS Variables") {
    REQUIRE_FALSE(usesS.run("1", "a", ""));
    REQUIRE_FALSE(usesS.run("2", "b", ""));
  }
}
