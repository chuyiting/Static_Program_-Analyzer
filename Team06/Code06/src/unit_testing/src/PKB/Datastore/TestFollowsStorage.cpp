//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/Datastore/FollowStorage.h"
#include "catch.hpp"

using tuple = std::pair<std::string, std::unordered_set<std::string>>;
using follows = std::unordered_set<std::string>;
TEST_CASE("followsStorage_1: simple case") {
  FollowStorage f = FollowStorage();
  f.addFollowT({"1", "2"});

  follows stored;
  follows expectedForward = {"2"};
  follows expectedBackward = {"1"};

  SECTION("getFollows") { REQUIRE(f.getFollows("1") == expectedForward); }

  SECTION("getFollowedBy") {
    REQUIRE(f.getFollowedBy("1") == follows({}));
    REQUIRE(f.getFollowedBy("2") == expectedBackward);
    // non existant
    REQUIRE(f.getFollowT("10") == follows({}));
  }

  SECTION("getFollowT") {
    REQUIRE(f.getFollowT("1") == expectedForward);
    // blank
    REQUIRE(f.getFollowT("2") == follows({}));
    // non existant
    REQUIRE(f.getFollowT("10") == follows({}));
  }

  SECTION("getFollowedByT") {
    REQUIRE(f.getFollowedByT("2") == expectedBackward);
    // blank case
    REQUIRE(f.getFollowedByT("1") == follows({}));
    // non existant
    REQUIRE(f.getFollowedBy("3") == follows({}));
  }

  SECTION("isFollowExists") { REQUIRE(f.isFollowExists()); }

  SECTION("isFollowDirect") {
    REQUIRE(f.isFollowDirect("1", "2"));
    REQUIRE_FALSE(f.isFollowDirect("2", "1"));
  }

  SECTION("isFollowRight") {
    REQUIRE(f.isFollowRight("1"));

    REQUIRE_FALSE(f.isFollowRight("2"));
  }

  SECTION("isFollowLeft") {
    REQUIRE(f.isFollowLeft("2"));
    // nothing is followed by 1
    REQUIRE_FALSE(f.isFollowLeft("1"));
  }

  SECTION("isFollowTDirect") {
    REQUIRE(f.isFollowTDirect("1", "2"));
    REQUIRE_FALSE(f.isFollowTDirect("2", "1"));
  }

  SECTION("isFollowTRight") {
    REQUIRE(f.isFollowTRight("1"));

    REQUIRE_FALSE(f.isFollowTRight("2"));
  }
  SECTION("isFollowTLeft") {
    REQUIRE(f.isFollowTLeft("2"));

    REQUIRE_FALSE(f.isFollowTLeft("1"));
  }
}

TEST_CASE("followsStorage_2") {
  FollowStorage f = FollowStorage();
  f.addFollowT({"1", "2", "3", "4"});
  f.addFollowT({"5", "6"});
  f.addFollowT({"8", "9"});

  std::unordered_set<Variable> stored;
  std::unordered_set<Variable> ans = {"2"};
  std::unordered_set<Variable> s = {"9"};

  SECTION("getFollows") {
    REQUIRE(f.getFollows("1") == ans);
    REQUIRE(f.getFollows("8") == s);
    REQUIRE_FALSE(f.getFollows("6") == s);
  }

  SECTION("getFollowedBy") {

    // non existant
    REQUIRE(f.getFollowedBy("10") == follows({}));
  }

  SECTION("getFollowT") {
    REQUIRE(f.getFollowT("8") == s);
    REQUIRE_FALSE(f.getFollowT("6") == s);
    // non existant
    REQUIRE(f.getFollowT("10") == follows({}));
  }

  SECTION("getFollowedByT") {
    // non existant
    REQUIRE(f.getFollowedBy("3") == follows({"2"}));
  }

  SECTION("isFollowExists") { REQUIRE(f.isFollowExists()); }

  SECTION("isFollowDirect") {
    REQUIRE(f.isFollowDirect("1", "2"));
    REQUIRE(f.isFollowDirect("5", "6"));
    REQUIRE(f.isFollowDirect("8", "9"));
    // check reverse order
    REQUIRE_FALSE(f.isFollowDirect("2", "1"));
  }

  SECTION("isFollowRight") {
    REQUIRE(f.isFollowRight("1"));
    REQUIRE(f.isFollowRight("5"));
    REQUIRE(f.isFollowRight("8"));
    REQUIRE_FALSE(f.isFollowRight("4"));
    // out of range
    REQUIRE_FALSE(f.isFollowRight("10"));
  }

  SECTION("isFollowLeft") {
    REQUIRE(f.isFollowLeft("2"));
    REQUIRE_FALSE(f.isFollowLeft("1"));
  }

  SECTION("isFollowTDirect") {
    REQUIRE(f.isFollowTDirect("1", "2"));
    REQUIRE_FALSE(f.isFollowTDirect("2", "1"));

    // disconnected statement on right
    REQUIRE_FALSE(f.isFollowTDirect("4", "5"));

    // invalid statement on left and right
    REQUIRE_FALSE(f.isFollowTDirect("10", "10"));
    REQUIRE_FALSE(f.isFollowTDirect("10", "11"));
    // invalid statement on left
    REQUIRE_FALSE(f.isFollowTDirect("9", "10"));
  }

  SECTION("isFollowTRight") {
    REQUIRE(f.isFollowTRight("1"));
    REQUIRE_FALSE(f.isFollowTRight("4"));
    REQUIRE_FALSE(f.isFollowTRight("9"));
    REQUIRE_FALSE(f.isFollowTRight("10"));
  }
  SECTION("isFollowTLeft") {
    REQUIRE(f.isFollowTLeft("2"));
    REQUIRE_FALSE(f.isFollowTLeft("1"));
  }
}
