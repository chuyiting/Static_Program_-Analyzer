//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/Datastore/FollowStorage.h"
#include "PKB/Resolver/StatementResolver/Container/FollowResolver.h"
#include "PKB/Resolver/StatementResolver/Container/FollowTResolver.h"
#include "catch.hpp"

TEST_CASE("follows_1") {
  FollowStorage s = FollowStorage();
  FollowResolver f = FollowResolver(s);
  FollowTResolver ft = FollowTResolver(s);
  s.addFollowT({"1", "2"});

  SECTION("follows_direct") {
    REQUIRE(f.run("1", "2", ""));

    REQUIRE_FALSE(f.run("2", "1", ""));

    REQUIRE_FALSE(f.run("1", "3", ""));
  }

  SECTION("follows_transitive") {
    REQUIRE(ft.run("1", "2", ""));

    REQUIRE_FALSE(ft.run("2", "1", ""));

    REQUIRE_FALSE(ft.run("1", "3", ""));
  }
}