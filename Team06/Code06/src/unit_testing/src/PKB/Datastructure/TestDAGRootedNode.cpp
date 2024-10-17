//
// Created by Stephen Tan  Hin Khai on 30/9/22.
//

#include "Exceptions/PKBException.h"
#include "PKB/Datastructure/dag/DAG.h"
#include "PKB/Datastructure/dag/RootedNode.h"
#include "catch.hpp"

using nodes = std::unordered_set<int>;
TEST_CASE("DAG<RootedNode> 1") {
  DAG<RootedNode> dag = DAG<RootedNode>();

  dag.addAncestors(6, std::vector({7, 8, 9, 11}));
  dag.addAncestors(11, std::vector({12, 16}));
  dag.addAncestors(12, std::vector({13, 14, 15}));
  dag.addAncestors(1, std::vector({2, 3, 4}));

  SECTION("getDirectAncestor") {
    REQUIRE(dag.getDirectAncestor(17) == nodes({}));
    REQUIRE(dag.getDirectAncestor(-1) == nodes({}));
    REQUIRE(dag.getDirectAncestor(0) == nodes({}));

    REQUIRE(dag.getDirectAncestor(6) == nodes({6}));
    REQUIRE(dag.getDirectAncestor(1) == nodes({1}));

    REQUIRE(dag.getDirectAncestor(13) == nodes({12}));
    REQUIRE(dag.getDirectAncestor(14) == nodes({12}));
    REQUIRE(dag.getDirectAncestor(12) == nodes({11}));
    REQUIRE(dag.getDirectAncestor(2) == nodes({1}));
  }

  SECTION("getAllAncestors") {
    REQUIRE(dag.getAllAncestors(6) == nodes({}));
    REQUIRE(dag.getAllAncestors(1) == nodes({}));

    REQUIRE(dag.getAllAncestors(13) == nodes({12, 11, 6}));
    REQUIRE(dag.getAllAncestors(14) == nodes({12, 11, 6}));
    REQUIRE(dag.getAllAncestors(12) == nodes({11, 6}));
    REQUIRE(dag.getAllAncestors(2) == nodes({1}));
  }

  SECTION("GetDirectChildren") {
    REQUIRE(dag.getDirectChildren(6) == nodes({7, 8, 9, 11}));
    REQUIRE(dag.getDirectChildren(1) == nodes({2, 3, 4}));
    REQUIRE(dag.getDirectChildren(6) == nodes({7, 8, 9, 11}));
    REQUIRE_FALSE(dag.getDirectChildren(6) ==
                  nodes({7, 8, 9, 11, 12, 16, 13, 14, 15}));

    REQUIRE(dag.getDirectChildren(15) == nodes({}));

    REQUIRE(dag.getDirectChildren(0) == nodes({}));
    REQUIRE(dag.getDirectChildren(-1) == nodes({}));
  }

  SECTION("GetIndirectChildren") {
    REQUIRE(dag.getAllChildren(1) == nodes({2, 3, 4}));
    REQUIRE(dag.getAllChildren(2) == nodes({}));

    REQUIRE(dag.getAllChildren(12) == nodes({13, 14, 15}));
    REQUIRE(dag.getAllChildren(11) == nodes({12, 16, 13, 14, 15}));
    REQUIRE(dag.getAllChildren(6) == nodes({7, 8, 9, 11, 12, 16, 13, 14, 15}));

    REQUIRE(dag.getAllChildren(0) == nodes({}));
    REQUIRE(dag.getAllChildren(-1) == nodes({}));
  }

  SECTION("findAncestor") {
    REQUIRE(dag.findAncestor(-1) == nodes({}));
    REQUIRE(dag.findAncestor(0) == nodes({}));
    REQUIRE(dag.findAncestor(18) == nodes({}));
    REQUIRE_FALSE(dag.findAncestor(6) == nodes({9}));
    REQUIRE_FALSE(dag.findAncestor(9) == nodes({9}));

    // Root to itself
    REQUIRE(dag.findAncestor(6) == nodes({6}));
    REQUIRE(dag.findAncestor(1) == nodes({1}));

    // Otherwise to its parent
    REQUIRE(dag.findAncestor(12) == nodes({11}));
    REQUIRE(dag.findAncestor(15) == nodes({12}));
    REQUIRE(dag.findAncestor(16) == nodes({11}));
  }

  SECTION("hasRootAncestor") {
    // boundary
    REQUIRE_FALSE(dag.hasRootAncestor(-1));
    REQUIRE_FALSE(dag.hasRootAncestor(18));

    // Root to Root
    REQUIRE_FALSE(dag.hasRootAncestor(6));
    REQUIRE_FALSE(dag.hasRootAncestor(0));
    REQUIRE_FALSE(dag.hasRootAncestor(1));

    // leaves and non-roots
    REQUIRE(dag.hasRootAncestor(9));
    REQUIRE(dag.hasRootAncestor(11));
    REQUIRE(dag.hasRootAncestor(15));
    REQUIRE(dag.hasRootAncestor(2));
  }

  SECTION("hasChild") {
    // boundary
    REQUIRE_FALSE(dag.hasChild(0));
    REQUIRE_FALSE(dag.hasChild(-1));
    REQUIRE_FALSE(dag.hasChild(18));

    // leaves
    REQUIRE_FALSE(dag.hasChild(7));
    REQUIRE_FALSE(dag.hasChild(16));
    REQUIRE_FALSE(dag.hasChild(13));

    // Root and non leaves
    REQUIRE(dag.hasChild(6));
    REQUIRE(dag.hasChild(11));
    REQUIRE(dag.hasChild(12));
    REQUIRE(dag.hasChild(6));
    REQUIRE(dag.hasChild(6));
  }

  SECTION("isDirectAncestor") {
    // invalid input
    REQUIRE_FALSE(dag.isDirectAncestor(0, 0));
    REQUIRE_FALSE(dag.isDirectAncestor(1, 0));
    REQUIRE_FALSE(dag.isDirectAncestor(1, 1));
    REQUIRE_FALSE(dag.isDirectAncestor(1, 18));
    REQUIRE_FALSE(dag.isDirectAncestor(-1, 1));

    // intended behavior
    REQUIRE(dag.isDirectAncestor(13, 12));
    REQUIRE(dag.isDirectAncestor(2, 1));
    REQUIRE(dag.isDirectAncestor(8, 6));

    REQUIRE_FALSE(dag.isDirectAncestor(1, 2));
    REQUIRE_FALSE(dag.isDirectAncestor(13, 11));
    REQUIRE_FALSE(dag.isDirectAncestor(15, 16));
  }

  SECTION("isDirectChild") {
    // invalid input
    REQUIRE_FALSE(dag.isDirectChild(0, 0));
    REQUIRE_FALSE(dag.isDirectChild(0, 1));
    REQUIRE_FALSE(dag.isDirectChild(1, 0));
    REQUIRE_FALSE(dag.isDirectChild(1, 1));
    REQUIRE_FALSE(dag.isDirectChild(1, 18));
    REQUIRE_FALSE(dag.isDirectChild(-1, 1));

    // intended behavior
    REQUIRE(dag.isDirectChild(12, 13));
    REQUIRE(dag.isDirectChild(1, 2));
    REQUIRE(dag.isDirectChild(6, 8));

    REQUIRE_FALSE(dag.isDirectChild(2, 1));
    REQUIRE_FALSE(dag.isDirectChild(11, 13));
    REQUIRE_FALSE(dag.isDirectChild(16, 15));
  }

  SECTION("hasAnyChild") { REQUIRE(dag.hasAnyChild()); }

  // if they have the same root
  SECTION("hasCommonAncestor") {
    // invalid input
    REQUIRE_FALSE(dag.hasCommonAncestor(-1, 1));
    REQUIRE_FALSE(dag.hasCommonAncestor(0, 1));

    // disconnected roots
    REQUIRE_FALSE(dag.hasCommonAncestor(1, 5));
    REQUIRE_FALSE(dag.hasCommonAncestor(6, 5));
    REQUIRE_FALSE(dag.hasCommonAncestor(4, 5));
    REQUIRE_FALSE(dag.hasCommonAncestor(6, 1));

    // connected roots
    REQUIRE(dag.hasCommonAncestor(7, 13));
    REQUIRE(dag.hasCommonAncestor(13, 15));
    REQUIRE(dag.hasCommonAncestor(15, 16));
    REQUIRE(dag.hasCommonAncestor(13, 9));
    REQUIRE(dag.hasCommonAncestor(9, 13));
    REQUIRE(dag.hasCommonAncestor(1, 3));
  }

  // Traversal from child to root does not encounter parent
  SECTION("isIndirectAncestor") {
    // invalid inputs
    REQUIRE_FALSE(dag.isIndirectAncestor(-1, 0));
    REQUIRE_FALSE(dag.isIndirectAncestor(0, 12));
    REQUIRE_FALSE(dag.isIndirectAncestor(18, 12));
    // intended behaviour
    REQUIRE(dag.isIndirectAncestor(1, 1));
    REQUIRE(dag.isIndirectAncestor(13, 12));
    REQUIRE(dag.isIndirectAncestor(13, 11));
    REQUIRE(dag.isIndirectAncestor(13, 6));
    REQUIRE(dag.isIndirectAncestor(12, 6));
    REQUIRE(dag.isIndirectAncestor(16, 6));

    // disjoint graphs
    REQUIRE_FALSE(dag.isIndirectAncestor(6, 1));
    REQUIRE_FALSE(dag.isIndirectAncestor(1, 6));
    // 6 is a parent
    REQUIRE_FALSE(dag.isIndirectAncestor(6, 16));
    // query parent not on path
    REQUIRE_FALSE(dag.isIndirectAncestor(9, 13));
    REQUIRE_FALSE(dag.isIndirectAncestor(13, 15));
    REQUIRE_FALSE(dag.isIndirectAncestor(13, 16));
  }

  // Traversal from child to root does not encounter parent
  SECTION("isIndirectChild") {
    // invalid inputs
    REQUIRE_FALSE(dag.isIndirectChild(-1, 0));
    REQUIRE_FALSE(dag.isIndirectChild(0, 12));
    REQUIRE_FALSE(dag.isIndirectChild(18, 12));

    // intended behaviour
    REQUIRE(dag.isIndirectChild(1, 1));
    REQUIRE(dag.isIndirectChild(12, 13));
    REQUIRE(dag.isIndirectChild(11, 13));
    REQUIRE(dag.isIndirectChild(6, 11));
    REQUIRE(dag.isIndirectChild(6, 13));
    REQUIRE(dag.isIndirectChild(6, 16));

    // disjoint graphs
    REQUIRE_FALSE(dag.isIndirectChild(6, 1));
    REQUIRE_FALSE(dag.isIndirectChild(1, 6));
    // 16 is a child
    REQUIRE_FALSE(dag.isIndirectChild(16, 6));
    // query parent is a leaf
    REQUIRE_FALSE(dag.isIndirectChild(9, 13));
    REQUIRE_FALSE(dag.isIndirectChild(13, 15));
    REQUIRE_FALSE(dag.isIndirectChild(13, 16));
  }

  // TODO: Add tests for Caching scenarios.
}

TEST_CASE("DAG Follow/FollowT - Case") {
  DAG<RootedNode> union_find = DAG<RootedNode>();
  // follows case
  std::vector<int> v = {1, 2, 3, 17, 18};
  std::vector<int> v1 = {4, 5, 6};
  std::vector<int> v2 = {7, 12, 13};
  std::vector<int> v3 = {8, 9};
  std::vector<int> v4 = {10, 11};
  // else block
  std::vector<int> v5 = {14, 15, 16};
  std::vector<std::vector<int>> vectors = {v, v1, v2, v3, v4, v5};

  for (std::vector<int> vec : vectors) {
    for (int i = 1; i < vec.size(); i++) {
      union_find.addAncestor(vec[i - 1], vec[i]);
    }
  }

  SECTION("isDirectAncestor") {

    REQUIRE(union_find.isDirectAncestor(13, 12));
    REQUIRE(union_find.isDirectAncestor(16, 15));

    REQUIRE_FALSE(union_find.isDirectAncestor(15, 16));
    REQUIRE_FALSE(union_find.isDirectAncestor(7, 6));
    REQUIRE_FALSE(union_find.isDirectAncestor(7, 4));
    REQUIRE_FALSE(union_find.isDirectAncestor(13, 7));
    REQUIRE_FALSE(union_find.isDirectAncestor(6, 1));

    // if else block
    REQUIRE_FALSE(union_find.hasCommonAncestor(6, 14));

    REQUIRE_FALSE(union_find.isDirectAncestor(0, 0));
    REQUIRE_FALSE(union_find.isDirectAncestor(1, 0));
  }

  // if they have the same root
  SECTION("hasCommonAncestor") {
    // different root nodes
    REQUIRE_FALSE(union_find.hasCommonAncestor(1, 7));
    // end of one to start of another
    REQUIRE_FALSE(union_find.hasCommonAncestor(13, 14));
    REQUIRE_FALSE(union_find.hasCommonAncestor(14, 13));

    REQUIRE(union_find.hasCommonAncestor(7, 13));
    // direct, transitive
    REQUIRE(union_find.hasCommonAncestor(4, 6));
    REQUIRE(union_find.hasCommonAncestor(6, 5));

    // if else block
    REQUIRE_FALSE(union_find.hasCommonAncestor(6, 14));

    //    REQUIRE_THROWS(union_find.hasCommonAncestor(0, 1),
    //		   PKBException(PKBStorageExceptionMessage::PKB_STORAGE_INVALID_INPUT));
  }

  SECTION("isIndirectAncestor") {
    REQUIRE(union_find.isIndirectAncestor(16, 14));
    REQUIRE(union_find.isIndirectAncestor(18, 3));

    REQUIRE_FALSE(union_find.isIndirectAncestor(1, 4));
    REQUIRE_FALSE(union_find.isIndirectAncestor(14, 18));
    REQUIRE_FALSE(union_find.isIndirectAncestor(6, 16));
  }

  //    REQUIRE(PKBParents1.isDirectParent(6, 7));
  //    REQUIRE(PKBParents1.isIndirectParent(6, 13));
  //    REQUIRE_FALSE(PKBParents1.isIndirectParent(13, 6));
  //    REQUIRE_FALSE(PKBParents1.isIndirectParent(7, 9));
  //    REQUIRE(PKBParents1.getDirectParent(13) == 12);
  //    REQUIRE(PKBParents1.getTransitiveParents(13) == std::unordered_set({6,
  //    11, 12}));
}