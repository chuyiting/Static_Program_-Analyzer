//
// Created by Stephen Tan  Hin Khai on 11/10/22.
//

#include "Exceptions/PKBException.h"
#include "PKB/Datastructure/dag/DAG.h"
#include "PKB/Datastructure/dag/TreeNode.h"
#include "catch.hpp"

using nodes = std::unordered_set<int>;
using adj = std::vector<int>;
// A TreeNode should be able to implement as expected of a RootedNode in order
// to follow LSP
TEST_CASE("DAG<TreeNode> Regression Test") {
  DAG<TreeNode> dag = DAG<TreeNode>();

  dag.addAncestors(6, std::vector({7, 8, 9, 11}));
  dag.addAncestors(11, std::vector({12, 16}));
  dag.addAncestors(12, std::vector({13, 14, 15}));
  dag.addAncestors(1, std::vector({2, 3, 4}));

  SECTION("findAncestor") {
    // invalid inputs go to 0 node and return null set
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
    dag.isIndirectAncestor(1, 1);
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

  SECTION("getDirectAncestor") {
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
  // TODO: Add tests for Caching scenarios.
}

TEST_CASE("DAG <TreeNode> Extension for Calls") {
  DAG<TreeNode> dag = DAG<TreeNode>();

  dag.addAncestors(1, adj({2, 3, 14}));
  dag.addAncestors(2, adj({4}));
  dag.addAncestors(3, adj({2, 5}));
  dag.addAncestors(5, adj({4}));
  dag.addAncestors(4, adj({7}));
  dag.addAncestors(8, adj({7, 13}));
  dag.addAncestors(7, adj({9}));
  dag.addAncestors(14, adj({15}));
  dag.addAncestors(13, adj({9}));
  dag.addAncestors(12, adj({13}));
  dag.addAncestors(10, adj({8, 12}));
  dag.addAncestors(6, adj({10, 3}));

  SECTION("getDirectAncestor") {
    REQUIRE(dag.getDirectAncestor(-1) == nodes({}));
    REQUIRE(dag.getDirectAncestor(99) == nodes({}));
    REQUIRE(dag.getDirectAncestor(0) == nodes({}));

    REQUIRE(dag.getDirectAncestor(1) == nodes({1}));
    REQUIRE(dag.getDirectAncestor(2) == nodes({1, 3}));
    REQUIRE(dag.getDirectAncestor(11) == nodes({11}));
    REQUIRE(dag.getDirectAncestor(10) == nodes({6}));

    REQUIRE(dag.getDirectAncestor(9) == nodes({7, 13}));
  }

  SECTION("getAllAncestors") {
    REQUIRE(dag.getAllAncestors(-1) == nodes({}));
    REQUIRE(dag.getAllAncestors(0) == nodes({}));
    REQUIRE(dag.getAllAncestors(99) == nodes({}));

    REQUIRE(dag.getAllAncestors(1) == nodes({}));
    REQUIRE(dag.getAllAncestors(2) == nodes({1, 3, 6}));
    REQUIRE(dag.getAllAncestors(11) == nodes({}));
    REQUIRE(dag.getAllAncestors(10) == nodes({6}));
    REQUIRE(dag.getAllAncestors(15) == nodes({14, 1}));

    REQUIRE(dag.getAllAncestors(9) == nodes({
                                          1,
                                          2,
                                          3,
                                          4,
                                          5,
                                          6,
                                          7,
                                          8,
                                          10,
                                          12,
                                          13,
                                      }));
  }

  SECTION("GetDirectChildren") {
    REQUIRE(dag.getDirectChildren(-1) == nodes({}));
    REQUIRE(dag.getDirectChildren(99) == nodes({}));

    REQUIRE(dag.getDirectChildren(2) == nodes({4}));
    REQUIRE(dag.getDirectChildren(3) == nodes({2, 5}));
    REQUIRE(dag.getDirectChildren(1) == nodes({2, 3, 14}));
    REQUIRE(dag.getDirectChildren(8) == nodes({7, 13}));
    REQUIRE(dag.getDirectChildren(7) == nodes({9}));
    REQUIRE(dag.getDirectChildren(12) == nodes({13}));

    REQUIRE(dag.getDirectChildren(9) == nodes({}));
  }

  SECTION("GetIndirectChildren") {

    REQUIRE(dag.getAllChildren(1) == nodes({2, 3, 4, 5, 7, 9, 14, 15}));
    REQUIRE(dag.getAllChildren(6) == nodes({2, 3, 4, 5, 7, 9, 10, 8, 12, 13}));
    REQUIRE(dag.getAllChildren(11) == nodes({}));

    REQUIRE(dag.getAllChildren(3) == nodes({2, 5, 4, 7, 9}));
    REQUIRE(dag.getAllChildren(7) == nodes({9}));
    REQUIRE(dag.getAllChildren(12) == nodes({13, 9}));
    REQUIRE(dag.getAllChildren(8) == nodes({7, 13, 9}));
    REQUIRE(dag.getAllChildren(9) == nodes({}));
  }

  SECTION("getRoots") {
    REQUIRE(dag.getRoots(9) == nodes({1, 8}));
    REQUIRE(dag.getRoots(5) == nodes({1}));
    REQUIRE(dag.getRoots(2) == nodes({1}));
    REQUIRE(dag.getRoots(10) == nodes({6}));
    REQUIRE(dag.getRoots(8) == nodes({10}));
    REQUIRE(dag.getRoots(7) == nodes({1, 8}));
    // This is necessary in order to re-root deterministically
    dag.reroot();
    REQUIRE(dag.getRoots(9) == nodes({1, 6}));
    REQUIRE(dag.getRoots(5) == nodes({1, 6}));
    REQUIRE(dag.getRoots(2) == nodes({1, 6}));
    REQUIRE(dag.getRoots(10) == nodes({6}));
    REQUIRE(dag.getRoots(8) == nodes({6}));
    REQUIRE(dag.getRoots(7) == nodes({1, 6}));
  }

  SECTION("findAncestor") {
    dag.reroot();
    REQUIRE(dag.findAncestor(0) == nodes({}));
    REQUIRE(dag.findAncestor(-1) == nodes({}));
    REQUIRE(dag.findAncestor(99) == nodes({}));

    REQUIRE(dag.findAncestor(1) == nodes({1}));
    REQUIRE(dag.findAncestor(2) == nodes({1, 3}));
    REQUIRE(dag.findAncestor(11) == nodes({11}));
    REQUIRE(dag.findAncestor(10) == nodes({6}));
    REQUIRE(dag.findAncestor(6) == nodes({6}));
    REQUIRE(dag.findAncestor(9) == nodes({13, 7}));
  }

  SECTION("hasRootAncestor") {
    dag.reroot();
    REQUIRE_FALSE(dag.hasRootAncestor(1));
    REQUIRE_FALSE(dag.hasRootAncestor(11));
    REQUIRE_FALSE(dag.hasRootAncestor(6));
    REQUIRE_FALSE(dag.hasRootAncestor(-1));

    REQUIRE(dag.hasRootAncestor(2));
    REQUIRE(dag.hasRootAncestor(3));
    REQUIRE(dag.hasRootAncestor(9));
  }

  SECTION("hasChild") {
    dag.reroot();
    REQUIRE_FALSE(dag.hasChild(-1));
    REQUIRE_FALSE(dag.hasChild(0));
    REQUIRE_FALSE(dag.hasChild(99));

    REQUIRE_FALSE(dag.hasChild(9));
    REQUIRE_FALSE(dag.hasChild(11));

    REQUIRE(dag.hasChild(2));
    REQUIRE(dag.hasChild(3));
    REQUIRE(dag.hasChild(1));
    REQUIRE(dag.hasChild(12));
  }

  SECTION("isDirectAncestor") {
    dag.reroot();
    // invalid inputs
    REQUIRE_FALSE(dag.isDirectAncestor(0, 0));
    REQUIRE_FALSE(dag.isDirectAncestor(1, 0));
    REQUIRE_FALSE(dag.isDirectAncestor(1, 1));
    REQUIRE_FALSE(dag.isDirectAncestor(1, 14));
    REQUIRE_FALSE(dag.isDirectAncestor(-1, 1));

    // intended behavior
    REQUIRE(dag.isDirectAncestor(13, 12));
    REQUIRE(dag.isDirectAncestor(2, 1));
    REQUIRE(dag.isDirectAncestor(8, 10));

    REQUIRE_FALSE(dag.isDirectAncestor(1, 2));
    REQUIRE_FALSE(dag.isDirectAncestor(13, 11));
    REQUIRE_FALSE(dag.isDirectAncestor(5, 8));
  }

  SECTION("isDirectChild") {
    dag.reroot();
    // invalid input
    REQUIRE_FALSE(dag.isDirectChild(0, 0));
    REQUIRE_FALSE(dag.isDirectChild(0, 1));
    REQUIRE_FALSE(dag.isDirectChild(1, 0));
    REQUIRE_FALSE(dag.isDirectChild(1, 1));
    REQUIRE_FALSE(dag.isDirectChild(1, 99));
    REQUIRE_FALSE(dag.isDirectChild(-1, 1));
    REQUIRE_FALSE(dag.isDirectChild(1, -1));

    // intended behavior
    REQUIRE(dag.isDirectChild(12, 13));
    REQUIRE(dag.isDirectChild(8, 7));
    REQUIRE(dag.isDirectChild(8, 13));
    REQUIRE(dag.isDirectChild(1, 2));

    REQUIRE_FALSE(dag.isDirectChild(8, 5));
    REQUIRE_FALSE(dag.isDirectChild(5, 8));
    REQUIRE_FALSE(dag.isDirectChild(6, 8));
    REQUIRE_FALSE(dag.isDirectChild(2, 1));
    REQUIRE_FALSE(dag.isDirectChild(11, 13));
  }

  SECTION("hasAnyChild") {
    dag.reroot();
    REQUIRE(dag.hasAnyChild());
  }

  SECTION("hasCommonAncestor") {
    dag.reroot();
    // invalid input
    REQUIRE_FALSE(dag.hasCommonAncestor(-1, 1));
    REQUIRE_FALSE(dag.hasCommonAncestor(0, 1));
    REQUIRE_FALSE(dag.hasCommonAncestor(99, 1));

    // disconnected roots
    REQUIRE_FALSE(dag.hasCommonAncestor(14, 10));
    REQUIRE_FALSE(dag.hasCommonAncestor(11, 12));
    REQUIRE_FALSE(dag.hasCommonAncestor(15, 10));
    REQUIRE_FALSE(dag.hasCommonAncestor(13, 14));
    REQUIRE_FALSE(dag.hasCommonAncestor(14, 12));

    // connected roots
    REQUIRE(dag.hasCommonAncestor(4, 7));
    REQUIRE(dag.hasCommonAncestor(8, 5));
    REQUIRE(dag.hasCommonAncestor(5, 8));
    REQUIRE(dag.hasCommonAncestor(7, 13)); // 6
    REQUIRE(dag.hasCommonAncestor(13, 5));
    REQUIRE(dag.hasCommonAncestor(5, 13));
    REQUIRE(dag.hasCommonAncestor(9, 6));
    REQUIRE(dag.hasCommonAncestor(6, 9));
    REQUIRE(dag.hasCommonAncestor(1, 9));
    REQUIRE(dag.hasCommonAncestor(9, 1));
  }

  SECTION("isIndirectAncestor") {
    dag.reroot();
    // invalid inputs
    REQUIRE_FALSE(dag.isIndirectAncestor(-1, 0));
    REQUIRE_FALSE(dag.isIndirectAncestor(0, 12));
    REQUIRE_FALSE(dag.isIndirectAncestor(14, 12));
    REQUIRE_FALSE(dag.isIndirectAncestor(16, 6));

    // intended behaviour
    REQUIRE(dag.isIndirectAncestor(1, 1));
    REQUIRE(dag.isIndirectAncestor(13, 12));
    REQUIRE(dag.isIndirectAncestor(13, 6));
    REQUIRE(dag.isIndirectAncestor(12, 6));
    // leaf node
    REQUIRE(dag.isIndirectAncestor(9, 13));

    // disconnected roots
    REQUIRE_FALSE(dag.isIndirectAncestor(6, 1));
    REQUIRE_FALSE(dag.isIndirectAncestor(1, 6));
    // 6 is a parent
    REQUIRE_FALSE(dag.isIndirectAncestor(6, 11));
    // query parent not on path
    REQUIRE_FALSE(dag.isIndirectAncestor(13, 11));
  }

  SECTION("isIndirectChild") {
    dag.reroot();
    // invalid inputs
    REQUIRE_FALSE(dag.isIndirectChild(-1, 0));
    REQUIRE_FALSE(dag.isIndirectChild(0, 12));
    REQUIRE_FALSE(dag.isIndirectChild(18, 12));

    // intended behaviour
    REQUIRE(dag.isIndirectChild(1, 1));
    REQUIRE(dag.isIndirectChild(12, 13));
    REQUIRE(dag.isIndirectChild(6, 9));
    REQUIRE(dag.isIndirectChild(6, 2));
    REQUIRE(dag.isIndirectChild(10, 7));

    // disjoint graphs
    REQUIRE_FALSE(dag.isIndirectChild(11, 13));
    // both roots
    REQUIRE_FALSE(dag.isIndirectChild(6, 1));
    REQUIRE_FALSE(dag.isIndirectChild(1, 6));
    // query parent is a leaf
    REQUIRE_FALSE(dag.isIndirectChild(9, 13));
    REQUIRE_FALSE(dag.isIndirectChild(13, 12));
  }
}
