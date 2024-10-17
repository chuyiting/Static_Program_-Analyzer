#include "catch.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "utils/CycleDetector/CycleDetector.h"

void testValidEdges(std::vector<std::pair<std::string, std::string>> edges) {
  CycleDetector cycleDetector(edges.size());
  for (auto edge : edges) {
    cycleDetector.addEdge(edge.first, edge.second);
  }

  // compared to boolean, instead of directly returning
  // !cycleDetector.findCycle(), for clarity
  REQUIRE(cycleDetector.findCycle() == false);
}

void testInvalidEdges(std::vector<std::pair<std::string, std::string>> edges) {
  CycleDetector cycleDetector(edges.size());
  for (auto edge : edges) {
    cycleDetector.addEdge(edge.first, edge.second);
  }

  // compared to boolean, instead of directly returning
  // cycleDetector.findCycle(), for clarity
  REQUIRE(cycleDetector.findCycle() == true);
}

TEST_CASE("CycleDetector") {
  SECTION("CyclicGraph_ShouldThrowException") {
    testInvalidEdges({{"a", "b"}, {"b", "c"}, {"c", "a"}});
    testInvalidEdges({
        {"a", "b"}, {"b", "c"}, {"c", "d"}, {"d", "e"}, {"e", "f"}, {"f", "g"},
        {"g", "h"}, {"h", "i"}, {"i", "j"}, {"j", "k"}, {"k", "l"}, {"l", "m"},
        {"m", "n"}, {"n", "o"}, {"o", "p"}, {"p", "q"}, {"q", "r"}, {"r", "s"},
        {"s", "t"}, {"t", "u"}, {"u", "v"}, {"v", "w"}, {"w", "x"}, {"x", "y"},
        {"y", "z"}, {"z", "a"},
    });
    testValidEdges({{"a", "b"}});
    testValidEdges({{"a", "b"}, {"b", "c"}, {"c", "d"}, {"d", "e"}});
  }
}
