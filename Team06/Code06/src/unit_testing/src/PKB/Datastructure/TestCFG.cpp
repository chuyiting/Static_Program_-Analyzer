#include "PKB/Datastructure/CFG.h"
#include "catch.hpp"
#include <string>

TEST_CASE("CFG") {
  CFG cfg = CFG();
  cfg.addEdge("1", "2");
  cfg.addEdge("2", "3");
  cfg.addEdge("3", "4");
  cfg.addEdge("4", "5");
  cfg.addEdge("5", "6");
  cfg.addEdge("6", "3");
  cfg.addEdge("3", "7");
  cfg.addEdge("7", "8");
  cfg.addEdge("7", "9");
  cfg.addEdge("8", "10");
  cfg.addEdge("9", "10");
  cfg.addEdge("10", "11");
  cfg.addEdge("11", "12");
  cfg.addEdge("12", "0");

	SECTION("getNextGraph") {
		std::unordered_map<std::string, std::unordered_set<std::string>> expectedNextGraph = {
			{"1", {"2"}},
			{"2", {"3"}},
			{"3", {"4", "7"}},
			{"4", {"5"}},
			{"5", {"6"}},
			{"6", {"3"}},
			{"7", {"8", "9"}},
			{"8", {"10"}},
			{"9", {"10"}},
			{"10", {"11"}},
			{"11", {"12"}},
			{"12", {"0"}}
		};
		REQUIRE(cfg.getNextGraph() == expectedNextGraph);
	}

	SECTION("getPrevGraph") {
		std::unordered_map<std::string, std::unordered_set<std::string>> expectedPrevGraph = {
			{"2", {"1"}},
			{"3", {"2", "6"}},
			{"4", {"3"}},
			{"5", {"4"}},
			{"6", {"5"}},
			{"7", {"3"}},
			{"8", {"7"}},
			{"9", {"7"}},
			{"10", {"8", "9"}},
			{"11", {"10"}},
			{"12", {"11"}},
			{"0", {"12"}}
		};
		REQUIRE(cfg.getPrevGraph() == expectedPrevGraph);
	}

	SECTION("Positive: isNodeDirectlyNext") {
		REQUIRE(cfg.isNodeDirectlyNext("1", "2"));
		REQUIRE(cfg.isNodeDirectlyNext("2", "3"));
		REQUIRE(cfg.isNodeDirectlyNext("3", "4"));
		REQUIRE(cfg.isNodeDirectlyNext("4", "5"));
		REQUIRE(cfg.isNodeDirectlyNext("5", "6"));
		REQUIRE(cfg.isNodeDirectlyNext("6", "3"));
		REQUIRE(cfg.isNodeDirectlyNext("3", "7"));
		REQUIRE(cfg.isNodeDirectlyNext("7", "8"));
		REQUIRE(cfg.isNodeDirectlyNext("7", "9"));
		REQUIRE(cfg.isNodeDirectlyNext("8", "10"));
		REQUIRE(cfg.isNodeDirectlyNext("9", "10"));
		REQUIRE(cfg.isNodeDirectlyNext("10", "11"));
		REQUIRE(cfg.isNodeDirectlyNext("11", "12"));
	}

	SECTION("Negative: isNodeDirectlyNext") {
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("2", "1"));
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("4", "3"));
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("5", "3"));
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("4", "7"));
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("5", "7"));
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("8", "9"));
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("9", "8"));
		REQUIRE_FALSE(cfg.isNodeDirectlyNext("7", "10"));
	}

	SECTION("Positive: hasNext") {
		REQUIRE(cfg.hasNext("1"));
		REQUIRE(cfg.hasNext("2"));
		REQUIRE(cfg.hasNext("3"));
		REQUIRE(cfg.hasNext("4"));
		REQUIRE(cfg.hasNext("5"));
		REQUIRE(cfg.hasNext("6"));
		REQUIRE(cfg.hasNext("7"));
		REQUIRE(cfg.hasNext("8"));
		REQUIRE(cfg.hasNext("9"));
		REQUIRE(cfg.hasNext("10"));
		REQUIRE(cfg.hasNext("11"));
	}

	SECTION("Negative: hasNext") {
		REQUIRE_FALSE(cfg.hasNext("12"));
	}

	SECTION("Positive: hasPrev") {
		REQUIRE(cfg.hasPrev("2"));
		REQUIRE(cfg.hasPrev("3"));
		REQUIRE(cfg.hasPrev("4"));
		REQUIRE(cfg.hasPrev("5"));
		REQUIRE(cfg.hasPrev("6"));
		REQUIRE(cfg.hasPrev("7"));
		REQUIRE(cfg.hasPrev("8"));
		REQUIRE(cfg.hasPrev("9"));
		REQUIRE(cfg.hasPrev("10"));
		REQUIRE(cfg.hasPrev("11"));
		REQUIRE(cfg.hasPrev("12"));
	}

	SECTION("Negative: hasPrev") {
		REQUIRE_FALSE(cfg.hasPrev("1"));
	}

	SECTION("Positive: isAnyNextPresent") {
		REQUIRE(cfg.isAnyNextPresent());
	}

	SECTION("Negative: isAnyNextPresent") {
		CFG c = CFG();
		REQUIRE_FALSE(c.isAnyNextPresent());
	}

	SECTION("getNextTStatements") {
		std::unordered_set<std::string> actualDownstreamGraph;
		std::unordered_set<std::string> expectedDownstreamGraph;

		actualDownstreamGraph = cfg.getNextTStatements("1");
		expectedDownstreamGraph = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("2");
		expectedDownstreamGraph = { "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("3");
		expectedDownstreamGraph = { "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("4");
		expectedDownstreamGraph = { "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("5");
		expectedDownstreamGraph = { "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("6");
		expectedDownstreamGraph = { "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("7");
		expectedDownstreamGraph = { "8", "9", "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("8");
		expectedDownstreamGraph = { "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("9");
		expectedDownstreamGraph = { "10", "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("10");
		expectedDownstreamGraph = { "11", "12", "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("11");
		expectedDownstreamGraph = { "12", "0"};
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);

		actualDownstreamGraph = cfg.getNextTStatements("12");
		expectedDownstreamGraph = { "0" };
		REQUIRE(actualDownstreamGraph == expectedDownstreamGraph);
	}

	SECTION("getPrevTStatements") {
		std::unordered_set<std::string> actualUpstreamGraph;
		std::unordered_set<std::string> expectedUpstreamGraph;

		actualUpstreamGraph = cfg.getPrevTStatements("1");
		expectedUpstreamGraph = { };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("2");
		expectedUpstreamGraph = { "1" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("3");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("4");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("5");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("6");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("7");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("8");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6", "7" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("9");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6", "7" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("10");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("11");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);

		actualUpstreamGraph = cfg.getPrevTStatements("12");
		expectedUpstreamGraph = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
		REQUIRE(actualUpstreamGraph == expectedUpstreamGraph);
	}
}
