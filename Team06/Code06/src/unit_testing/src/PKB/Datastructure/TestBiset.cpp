//
// Created by Stephen Tan  Hin Khai on 24/10/22.
//
#include "PKB/Datastructure/biset.h"
#include "catch.hpp"
#include "unordered_set"

using strSet = std::unordered_set<std::string>;
using intSet = std::unordered_set<int>;

TEST_CASE("BISET") {
  biset::unordered_biset<std::string, int> biset;

  biset.insert({"foo", {1, 2, 4}});
  biset.insert({"bar", {2, 3}});
  biset.insert({"rho", {3, 1}});
  biset.insert({"tau", {4, 2}});
  biset.insert({"soh", {51}});
  biset.insert({"toh", {}});

  SECTION("RETRIEVAL") {
    REQUIRE(biset.get_valueset("foo") == intSet({1, 2, 4}));
    REQUIRE(biset.get_valueset("bar") == intSet({2, 3}));
    REQUIRE(biset.get_valueset("rho") == intSet({3, 1}));
    REQUIRE(biset.get_valueset("tau") == intSet({4, 2}));
    REQUIRE(biset.get_valueset("soh") == intSet({51}));
    REQUIRE(biset.get_valueset("toh") == intSet({}));

    REQUIRE(biset.get_keyset(1) == strSet({"foo", "rho"}));
    REQUIRE(biset.get_keyset(2) == strSet({"foo", "bar", "tau"}));
  }

  SECTION("QUERYING") {
    REQUIRE(biset.has_key("foo"));
    REQUIRE(biset.has_key("bar"));
    REQUIRE(biset.has_key("rho"));
    REQUIRE(biset.has_key("tau"));
    REQUIRE(biset.has_key("soh"));
    REQUIRE(biset.has_value(1));
    REQUIRE(biset.has_value(2));
    REQUIRE_FALSE(biset.has_value(69));
    REQUIRE_FALSE(biset.has_key("foobar"));

    REQUIRE_FALSE(biset.has_key("toh"));
  }

  SECTION("QUERYING SPECIFIC MAPPING") {
    REQUIRE(biset.has_keymapping("foo", 1));
    REQUIRE(biset.has_keymapping("bar", 2));
    REQUIRE(biset.has_keymapping("rho", 1));
    REQUIRE_FALSE(biset.has_keymapping("foobar", 1));
    REQUIRE_FALSE(biset.has_keymapping("foo", 600));

    REQUIRE(biset.has_valuemapping(1, "foo"));
    REQUIRE_FALSE(biset.has_valuemapping(60, "foo"));
    REQUIRE_FALSE(biset.has_valuemapping(1, "foobar"));
  }
}
