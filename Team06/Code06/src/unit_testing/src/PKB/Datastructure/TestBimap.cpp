//
// Created by Stephen Tan  Hin Khai on 2/10/22.
//

// #include "PKB/Datastore/CallStorage.h"
#include "PKB/Datastructure/bimap.h"
#include "catch.hpp"

TEST_CASE("BIMAP") {
  bimap::unordered_bimap<std::string, int> bimap_1;

  bimap_1.insert({"foo", 1});
  bimap_1.insert({"bar", 2});
  bimap_1.insert({"rho", 3});
  bimap_1.insert({"tau", 4});
  bimap_1.insert({"soh", 5});

  REQUIRE(bimap_1.get_value("foo") == 1);
  REQUIRE(bimap_1.get_value("bar") == 2);
  REQUIRE(bimap_1.get_value("rho") == 3);
  REQUIRE(bimap_1.get_value("tau") == 4);
  REQUIRE(bimap_1.get_value("soh") == 5);

  REQUIRE(bimap_1.get_key(1) == "foo");
REQUIRE(bimap_1.get_key(2) == "bar");
REQUIRE(bimap_1.get_key(3) == "rho");
REQUIRE(bimap_1.get_key(4) == "tau");
REQUIRE(bimap_1.get_key(5) == "soh");
}
