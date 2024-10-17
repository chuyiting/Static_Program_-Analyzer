//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "catch.hpp"

// Constants
TEST_CASE("constant_insertion_retrieval") {
  PKB pkb;
  PKBSetter setter = PKBSetter(pkb);
  PKBGetter getter = PKBGetter(pkb);

  Constant c;

  SECTION("DigitMin") {
    c = "0";
    // negative
    std::unordered_set<Constant> stored = getter.getConstants();
    REQUIRE(stored.find(c) == stored.end());

    // Insert constant into PKB
    setter.addConstant(c);

    // Check insertion success
    stored = getter.getConstants();
    REQUIRE(stored.find(c) != stored.end());
    // Check size of insertion is expected
    REQUIRE(stored.size() == 1);
  }

  SECTION("DigitAboveMin") {
    c = "1";

    // negative
    std::unordered_set<Constant> stored = getter.getConstants();
    REQUIRE(stored.find(c) == stored.end());

    // Insert constant into PKB
    setter.addConstant(c);

    // Check insertion success
    stored = getter.getConstants();
    REQUIRE(stored.find(c) != stored.end());
  }

  SECTION("DigitNominal") {
    c = "5";

    // negative
    std::unordered_set<Constant> stored = getter.getConstants();
    REQUIRE(stored.find(c) == stored.end());

    // Insert constant into PKB
    setter.addConstant(c);

    // Check insertion success
    stored = getter.getConstants();
    REQUIRE(stored.find(c) != stored.end());
  }

  SECTION("TwoDigits") {
    c = "23";
    // negative
    std::unordered_set<Constant> stored = getter.getConstants();
    REQUIRE(stored.find(c) == stored.end());

    // Insert constant into PKB
    setter.addConstant(c);

    // Check insertion success
    stored = getter.getConstants();
    REQUIRE(stored.find(c) != stored.end());
  }

  SECTION("ThreeDigits") {
    c = "120";

    // negative
    std::unordered_set<Constant> stored = getter.getConstants();
    REQUIRE(stored.find(c) == stored.end());

    // Insert constant into PKB
    setter.addConstant(c);

    // Check insertion success
    stored = getter.getConstants();
    REQUIRE(stored.find(c) != stored.end());
  }

  SECTION("addMultipleConstants") {
    setter.addConstant("1");
    setter.addConstant("3");
    setter.addConstant("5");
    setter.addConstant("13");
    setter.addConstant("50");
    setter.addConstant("62");
    setter.addConstant("83");
    setter.addConstant("100");

    std::unordered_set<Constant> stored = getter.getConstants();

    SECTION("positive") {
      std::unordered_set<Constant> expected = {"1",  "3",  "5",  "13",
                                               "50", "62", "83", "100"};
      REQUIRE(stored == expected);
    }

    SECTION("negative") {
      std::unordered_set<Constant> expected = {"1", "3",
                                               "13"
                                               "62",
                                               "83", "100"};
      REQUIRE_FALSE(stored == expected);

      expected = {"1", "2", "3", "4", "5", "13", "50", "62", "83", "100"};
      REQUIRE_FALSE(stored == expected);
    }
  }
}