//
// Created by Chong Jun Wei on 8/10/22.
//

#include "PKB/Resolver/Pattern/AssignPatternResolver.h"
#include "StubPatternStorage1.h"
#include "catch.hpp"

// assign pattern
TEST_CASE("pattern_1") {
  // 3 c = s + 3 * 2
  StubPatternStorage1 patternStorage = StubPatternStorage1();
  AssignPatternResolver p = AssignPatternResolver(patternStorage);
  Statement s = "3";

  SECTION("entity_direct") {
    // pattern('c', 's+3*2')
    Expression LHS = "c";
    Expression RHS = "s+3*2";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('c', 's')
    LHS = "c";
    RHS = "s";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern('c', 'c')
    LHS = "c";
    RHS = "c";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }

  SECTION("entity_wildcard") {
    // pattern('c', '_')
    Expression LHS = "c";
    Expression RHS = "_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('s', '_')
    LHS = "s";
    RHS = "_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }

  SECTION("entity_subexpr") {
    // pattern ('c', '_s + 3_')
    Expression LHS = "c";
    Expression RHS = "_s+3_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern ('c', '_3 * 2_')
    LHS = "c";
    RHS = "_3*2_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ('c', '_s + 3 * 2_')
    LHS = "c";
    RHS = "_s+3*2_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ('s', '_s + 3 * 2_')
    LHS = "s";
    RHS = "_s+3*2_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }

  SECTION("wildcard_entity") {
    // pattern('_', '_s_')
    Expression LHS = "_";
    Expression RHS = "_s_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '_c_')
    LHS = "_";
    RHS = "_c_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern('_', '_3_')
    LHS = "_";
    RHS = "_3_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '_2_')
    LHS = "_";
    RHS = "_2_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '_1_')
    LHS = "_";
    RHS = "_1_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }

  SECTION("wildcard_wildcard") {
    // wildcard, wildcard
    // pattern('_', '_')
    Expression LHS = "_";
    Expression RHS = "_";

    REQUIRE(p.run(LHS, RHS, s));

    REQUIRE_FALSE(p.run(LHS, RHS, "4"));
  }

  SECTION("wildcard, subexpr") {
    // wildcard, subexpr
    // pattern ( _ , '_s+3_')
    Expression LHS = "_";
    Expression RHS = "_s+3_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern ( _ , '_3*2_')
    LHS = "_";
    RHS = "_3*2_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ( _ , '_s+3*2_')
    LHS = "_";
    RHS = "_s+3*2_";

    REQUIRE(p.run(LHS, RHS, s));
  }

  SECTION("brackets") {
    // brackets
    // pattern ('c', '_s+(3*2)_')
    Expression LHS = "c";
    Expression RHS = "_s+(3*2)_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ('c', '_s+(3)*2_')
    LHS = "c";
    RHS = "_s+(3)*2_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ('c', '_(s+(3*2))_')
    LHS = "c";
    RHS = "_(s+3*2)_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ('c', '_(s+(3*2))_')
    LHS = "c";
    RHS = "_(s+(3*2))_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ('c', '_((s)+(3*2))_')
    LHS = "c";
    RHS = "_((s)+(3*2))_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern ('c', '_((s+3)*2)_')
    LHS = "c";
    RHS = "_((s+3)*2)_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern ('c', '_(((s+3)*2))_')
    LHS = "c";
    RHS = "_(((s+3)*2))_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }
}

// assign pattern
TEST_CASE("pattern_2") {
  // 5 s = var + 3 * foo * (s + 3)
  StubPatternStorage1 patternStorage = StubPatternStorage1();
  AssignPatternResolver p = AssignPatternResolver(patternStorage);
  Statement s = "5";

  SECTION("entity_direct") {
    // pattern('s', 'var + 3 * foo * (s + 3)')
    Expression LHS = "s";
    Expression RHS = "var+3*foo*(s+3)";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('s', 'var + 3 * (foo * s) + 3')
    LHS = "s";
    RHS = "var+3*(foo*s)+3";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }

  SECTION("entity_wildcard") {
    // entity, wildcard
    // pattern('s', '_')
    Expression LHS = "s";
    Expression RHS = "_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('c', '_')
    LHS = "c";
    RHS = "_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }

  SECTION("entity_subexpr") {
    // entity, subexpr
    // pattern('s', '_var + 3 * foo * (s + 3)_')
    Expression LHS = "s";
    Expression RHS = "_var+3*foo*(s+3)_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('s', '_var + 3 * (foo * s) + 3_')
    LHS = "s";
    RHS = "_var+3*(foo*s)+3_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern('s', '_var + 3_')
    LHS = "s";
    RHS = "_var+3_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern('s', '_3 * foo_')
    LHS = "s";
    RHS = "_3*foo_";

    REQUIRE(p.run(LHS, RHS, s));
  }

  SECTION("wildcard_wildcard") {
    // wildcard, wildcard
    // pattern('_', '_')
    Expression LHS = "_";
    Expression RHS = "_";

    REQUIRE(p.run(LHS, RHS, s));

    REQUIRE_FALSE(p.run(LHS, RHS, "2"));
  }

  SECTION("wildcard_subexpr") {
    // wildcard, subexpr
    // pattern('_', '_var + 3 * foo * (s + 3)_')
    Expression LHS = "_";
    Expression RHS = "_var+3*foo*(s+3)_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '_var + 3 * (foo * s) + 3_')
    LHS = "_";
    RHS = "_var+3*(foo*s)+3_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern('_', '_var + 3_')
    LHS = "_";
    RHS = "_var+3_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern('_', '_3 * foo_')
    LHS = "_";
    RHS = "_3*foo_";

    REQUIRE(p.run(LHS, RHS, s));
  }
}

// assign pattern (parentheses)
TEST_CASE("pattern_parentheses_1") {
  // 7 a = (x+y)
  StubPatternStorage1 patternStorage = StubPatternStorage1();
  AssignPatternResolver p = AssignPatternResolver(patternStorage);
  Statement s = "7";

  SECTION("direct_expr") {
    // pattern('_', 'x+y')
    Expression LHS = "_";
    Expression RHS = "x+y";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '(x+y)')
    LHS = "_";
    RHS = "(x+y)";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '((x+y))')
    LHS = "_";
    RHS = "((x+y))";

    REQUIRE(p.run(LHS, RHS, s));
  }

  SECTION("sub_expr") {
    // pattern('_', '_(x+y)_')
    Expression LHS = "_";
    Expression RHS = "_(x+y)_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '_((x+y))_')
    LHS = "_";
    RHS = "((x+y))";

    REQUIRE(p.run(LHS, RHS, s));
  }
}

TEST_CASE("pattern_parentheses_2") {
  // 8 c = (x+y)*2

  StubPatternStorage1 patternStorage = StubPatternStorage1();
  AssignPatternResolver p = AssignPatternResolver(patternStorage);
  Statement s = "8";

  SECTION("direct_expr") {
    // pattern('_', '(x+y)*2')
    Expression LHS = "_";
    Expression RHS = "(x+y)*2";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern('_', '(x+(y*2))')
    LHS = "_";
    RHS = "(x+(y*2))";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }

  SECTION("sub_expr") {
    // pattern('_', '_(x+(y*2))_')
    Expression LHS = "_";
    Expression RHS = "_(x+(y*2))_";

    REQUIRE_FALSE(p.run(LHS, RHS, s));

    // pattern('_', '_(x+y)*2_')
    LHS = "_";
    RHS = "_(x+y)*2_";

    REQUIRE(p.run(LHS, RHS, s));
  }
}

TEST_CASE("pattern_lecture1") {
  // 9 x = v + x * y + z * t

  StubPatternStorage1 patternStorage = StubPatternStorage1();
  AssignPatternResolver p = AssignPatternResolver(patternStorage);
  Statement s = "9";

  // pattern a('x', '_') returns true
  Expression LHS = "x";
  Expression RHS = "_";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('x', 'v + x * y + z * t') returns true
  LHS = "x";
  RHS = "v+x*y+z*t";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('_', 'v + x * y + z * t') returns true
  LHS = "_";
  RHS = "v+x*y+z*t";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('x', 'v') returns false
  LHS = "x";
  RHS = "v";

  REQUIRE_FALSE(p.run(LHS, RHS, s));

  // pattern a('x', '_v_') returns true
  LHS = "x";
  RHS = "_v_";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('x', '_x*y_') returns true
  LHS = "x";
  RHS = "_x*y_";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('x', '_v+x_') returns false
  LHS = "x";
  RHS = "_v+x_";

  REQUIRE_FALSE(p.run(LHS, RHS, s));

  // pattern a('x', '_v+x*y_') returns true
  LHS = "x";
  RHS = "_v+x*y_";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('x', '_v+x*y_') returns false
  LHS = "x";
  RHS = "_y+z*t_";

  REQUIRE_FALSE(p.run(LHS, RHS, s));

  // pattern a('_', '_x * y + z * t_') returns false
  LHS = "_";
  RHS = "_x*y+z*t_";

  REQUIRE_FALSE(p.run(LHS, RHS, s));

  // pattern a('_', '_v+x*y+z*t_"') returns true
  LHS = "_";
  RHS = "_v+x*y+z*t_";

  REQUIRE(p.run(LHS, RHS, s));

  SECTION("whitespaces") {
    // whitespaces
    // pattern a('_', '_v         +x         *y          +z                *t_')
    // returns true
    LHS = "_";
    RHS = "_v         +x         *y          +z                *t_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a('_', '_ v + x * y + z * t_') returns true
    LHS = "_";
    RHS = "_ v + x * y + z * t_";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a('_', ' _v+x*y+z*t_ ') returns true
    LHS = "_";
    RHS = " _v+x*y+z*t_ ";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a('_', ' _ ') returns true
    LHS = "_";
    RHS = " _ ";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a('x', ' _ ') returns true
    LHS = "x";
    RHS = " _ ";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a(' x', ' _ ') returns true
    LHS = " x";
    RHS = " _ ";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a('x ', ' _ ') returns true
    LHS = "x ";
    RHS = " _ ";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a(' x ', ' _ ') returns true
    LHS = " x ";
    RHS = " _ ";

    REQUIRE(p.run(LHS, RHS, s));

    // pattern a('s', ' _ ') returns false
    LHS = "s";
    RHS = " _ ";

    REQUIRE_FALSE(p.run(LHS, RHS, s));
  }
}

// expr = entity
TEST_CASE("concrete_entity") {
  // 10 m = d

  StubPatternStorage1 patternStorage = StubPatternStorage1();
  AssignPatternResolver p = AssignPatternResolver(patternStorage);
  Statement s = "10";

  // pattern a('m', 'd') returns true
  Expression LHS = "m";
  Expression RHS = "d";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('m', ' d ') returns true
  LHS = "m";
  RHS = " d ";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('m', '_d_') returns true
  LHS = "m";
  RHS = "_d_";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('m', '_d _') returns true
  LHS = "m";
  RHS = "_d _";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('m', '_') returns true
  LHS = "m";
  RHS = "_";

  REQUIRE(p.run(LHS, RHS, s));

  // pattern a('m', 'd+3') returns false
  LHS = "m";
  RHS = "d+3";

  REQUIRE_FALSE(p.run(LHS, RHS, s));
}

// expr = number
TEST_CASE("concrete_number") {
  // 13 v = 3

  StubPatternStorage1 patternStorage = StubPatternStorage1();
  AssignPatternResolver p = AssignPatternResolver(patternStorage);

  // pattern a('v', '3') returns true
  Expression LHS = "v";
  Expression RHS = "3";

  REQUIRE(p.run(LHS, RHS, "13"));

  // pattern a('v', ' 3 ') returns true
  LHS = "v";
  RHS = " 3 ";

  REQUIRE(p.run(LHS, RHS, "13"));

  // pattern a('v', '_3_') returns true
  LHS = "v";
  RHS = "_3_";

  REQUIRE(p.run(LHS, RHS, "13"));

  // pattern a('v', '_3 _') returns true
  LHS = "v";
  RHS = "_3 _";

  REQUIRE(p.run(LHS, RHS, "13"));

  // pattern a('v', '_') returns true
  LHS = "v";
  RHS = "_";

  REQUIRE(p.run(LHS, RHS, "13"));

  // pattern a('v', 'd+3') returns false
  LHS = "v";
  RHS = "d+3";

  REQUIRE_FALSE(p.run(LHS, RHS, "13"));
}