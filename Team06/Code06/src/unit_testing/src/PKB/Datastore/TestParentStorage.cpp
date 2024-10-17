//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/Datastore/ParentStorage.h"
#include "catch.hpp"

using tuple = std::pair<std::string, std::unordered_set<std::string>>;
using parentOf = std::unordered_set<std::string>;
TEST_CASE("parents_1") {
  ParentStorage p = ParentStorage();
  p.addParent("1", {"2"});

  parentOf stored = {};
  parentOf ans = {"2"};

  REQUIRE(p.getParents("1") == ans);

  // yet to test (as above test case failed)
  REQUIRE(p.getParentT("2") == stored);
  REQUIRE(p.getParents("2") == stored);

  REQUIRE(p.getParentT("99") == stored);
  REQUIRE(p.getParentT("0") == stored);

  REQUIRE(p.isParentExists());
  REQUIRE(p.isParentDirect("1", "2"));
  REQUIRE_FALSE(p.isParentDirect("2", "1"));

  REQUIRE(p.isParentRight("1"));
  REQUIRE(p.isParentLeft("2"));
  REQUIRE(p.isParentTDirect("1", "2"));
  REQUIRE(p.isParentTRight("1"));
  REQUIRE(p.isParentTLeft("2"));

  REQUIRE_FALSE(p.isParentRight("2"));
  REQUIRE_FALSE(p.isParentLeft("1"));
  REQUIRE_FALSE(p.isParentTDirect("2", "1"));
  REQUIRE_FALSE(p.isParentTRight("2"));
  REQUIRE_FALSE(p.isParentTLeft("1"));
}

TEST_CASE("Parents_2") {
  ParentStorage f = ParentStorage();
  f.addParent("1", {"2"});
  f.addParent("5", {"6"});
  f.addParent("8", {"9"});

  parentOf s = {"2"};
  REQUIRE(f.getParents("1") == s);
  REQUIRE(f.getParentT("1") == s);

  s = {"3"};
  REQUIRE_FALSE(f.getParents("1") == s);
  REQUIRE_FALSE(f.getParentT("1") == s);

  s = {"6"};
  REQUIRE(f.getParents("5") == s);
  REQUIRE(f.getParentT("5") == s);

  // haven't tested (as above test cases failed)
  s = {"9"};
  REQUIRE(f.getParents("8") == s);
  REQUIRE(f.getParentT("8") == s);
  REQUIRE_FALSE(f.getParents("6") == s);
  REQUIRE_FALSE(f.getParentT("6") == s);

  REQUIRE(f.isParentExists());
  REQUIRE(f.isParentDirect("1", "2"));
  REQUIRE(f.isParentDirect("5", "6"));
  REQUIRE(f.isParentDirect("8", "9"));

  REQUIRE(f.isParentRight("1"));
  REQUIRE(f.isParentRight("5"));
  REQUIRE(f.isParentRight("8"));
  REQUIRE_FALSE(f.isParentRight("2"));
  REQUIRE_FALSE(f.isParentRight("3"));

  REQUIRE(f.isParentLeft("2"));
  REQUIRE(f.isParentTDirect("1", "2"));
  REQUIRE(f.isParentTRight("1"));
  REQUIRE(f.isParentTLeft("2"));

  REQUIRE_FALSE(f.isParentDirect("2", "1"));

  REQUIRE_FALSE(f.isParentRight("2"));
  REQUIRE_FALSE(f.isParentLeft("1"));
  REQUIRE_FALSE(f.isParentTDirect("2", "1"));
  REQUIRE_FALSE(f.isParentTRight("2"));
  REQUIRE_FALSE(f.isParentTLeft("1"));
}