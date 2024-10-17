//
// Created by Chong Jun Wei on 9/10/22.
//
#include "PKB/Resolver/Pattern/IfPatternResolver.h"
#include "PKB/Resolver/Pattern/WhilePatternResolver.h"
#include "StubPatternStorage2.h"
#include "catch.hpp"

// if pattern
TEST_CASE("if") {
  StubPatternStorage2 patternStorage = StubPatternStorage2();
  IfPatternResolver i = IfPatternResolver(patternStorage);

  SECTION("wildcard") {
    // pattern ifs(_,_,_)
    Expression LHS = "_";
    Expression RHS = "_";

    REQUIRE(i.run(LHS, RHS, "4"));

    REQUIRE_FALSE(i.run(LHS, RHS, "2"));

    REQUIRE_FALSE(i.run(LHS, RHS, "5"));
  }

  SECTION("concrete") {
    // pattern ifs(x,_)
    Expression LHS = "x";
    Expression RHS = "_";

    REQUIRE(i.run(LHS, RHS, "4"));

    REQUIRE_FALSE(i.run(LHS, RHS, "3"));

    REQUIRE_FALSE(i.run(LHS, RHS, "5"));

    // pattern ifs(y,_)
    LHS = "y";
    RHS = "_";

    REQUIRE_FALSE(i.run(LHS, RHS, "4"));

    REQUIRE_FALSE(i.run(LHS, RHS, "3"));

    REQUIRE_FALSE(i.run(LHS, RHS, "5"));
  }
}

// while pattern
TEST_CASE("while") {
  StubPatternStorage2 patternStorage = StubPatternStorage2();
  WhilePatternResolver w = WhilePatternResolver(patternStorage);

  SECTION("wildcard") {
    // pattern w(_,_)
    Expression LHS = "_";
    Expression RHS = "_";

    REQUIRE(w.run(LHS, RHS, "5"));

    REQUIRE_FALSE(w.run(LHS, RHS, "1"));

    REQUIRE_FALSE(w.run(LHS, RHS, "4"));
  }

  SECTION("concrete") {
    // pattern w(x,_)
    Expression LHS = "x";
    Expression RHS = "_";

    REQUIRE(w.run(LHS, RHS, "5"));

    REQUIRE_FALSE(w.run(LHS, RHS, "4"));

    REQUIRE_FALSE(w.run(LHS, RHS, "2"));

    // pattern w(y,_)
    LHS = "y";
    RHS = "_";

    REQUIRE(w.run(LHS, RHS, "5"));

    REQUIRE_FALSE(w.run(LHS, RHS, "4"));

    REQUIRE_FALSE(w.run(LHS, RHS, "2"));

    // pattern w(s,_)
    LHS = "s";
    RHS = "_";

    REQUIRE_FALSE(w.run(LHS, RHS, "5"));
  }
}