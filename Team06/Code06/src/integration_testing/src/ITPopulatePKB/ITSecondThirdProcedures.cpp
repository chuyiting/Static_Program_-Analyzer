#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "SP/SourceProcessor.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using set = std::unordered_set<std::string>;

TEST_CASE("Second Third Procedures") {
	std::string sourceCode =
		"procedure Second {"
		"	x = 0;" // 1
		"	i = 5;" // 2
		"	while (i != 0) {" // 3
		"		x = x + 2 * y;" // 4
		"		call Third;" // 5
		"		i = i - 1;" // 6
		"	}"
		"	if (x == 1) then {" // 7
		"		x = x + 1;" // 8
		"	} else {"
		"		z = 1;" // 9
		"	}"
		"	z = z + x + i;" // 10
		"	y = z + 2;" // 11
		"	x = x * y + z;" // 12
		"}"
		"procedure Third {"
		"	z = 5;" // 13
		"	v = z;" // 14
		"	print v;" // 15
		"}";

	PKB pkb = PKB();
	std::shared_ptr<PKBSetter> setter = std::make_shared<PKBSetter>(PKBSetter(pkb));
	std::shared_ptr<PKBGetter> getter = std::make_shared<PKBGetter>(PKBGetter(pkb));
	SourceProcessor SP = SourceProcessor(setter);
	SP.processCode(sourceCode);
	setter->done();

	/* ============================== Follows ============================== */

	std::unordered_map<Statement, std::unordered_set<Statement>> follows = {
		// procedure Second
		{"1", {"2"}},
		{"2", {"3"}},
		{"3", {"7"}},
		{"7", {"10"}},
		{"10", {"11"}},
		{"11", {"12"}},
		// while loop
		{"4", {"5"}},
		{"5", {"6"}},
		// procedure Third
		{"13", {"14"}},
		{"14", {"15"}}
	};

	std::unordered_map<Statement, std::unordered_set<Statement>> followedBy = {
		// procedure Second
		{"2", {"1"}},
		{"3", {"2"}},
		{"7", {"3"}},
		{"10", {"7"}},
		{"11", {"10"}},
		{"12", {"11"}},
		// while loop
		{"5", {"4"}},
		{"6", {"5"}},
		// procedure Third
		{"14", {"13"}},
		{"15", {"14"}}
	};

	SECTION("Follows(numeric_ref, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : follows) {
				for (auto const& v : val) {
					REQUIRE(getter->isFollows(key, v));
				}
			}
		}

		SECTION("Negative") {
			// Test for different nesting levels
			REQUIRE_FALSE(getter->isFollows("2", "4"));
			REQUIRE_FALSE(getter->isFollows("6", "7"));
			REQUIRE_FALSE(getter->isFollows("8", "9"));
			REQUIRE_FALSE(getter->isFollows("6", "8"));
			REQUIRE_FALSE(getter->isFollows("9", "10"));
		}
	}

	SECTION("Follows(numeric_ref, wildcard)") {
		SECTION("Positive") {
			for (auto const& [key, val] : follows) {
				REQUIRE(getter->isFollows(key, "_"));
			}
		}

		SECTION("Negative") {
			// Test for nesting levels that only has one statement
			REQUIRE_FALSE(getter->isFollows("8", "_"));
			REQUIRE_FALSE(getter->isFollows("9", "_"));

			// Test for end of procedure
			REQUIRE_FALSE(getter->isFollows("12", "_"));
			REQUIRE_FALSE(getter->isFollows("15", "_"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : follows) {
				REQUIRE(getter->getFollows(key, true) == follows[key]);
			}
		}
	}

	SECTION("Follows(wildcard, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : followedBy) {
				REQUIRE(getter->isFollows("_", key));
			}
		}

		SECTION("Negative") {
			// Test for nesting levels that only has one statement
			REQUIRE_FALSE(getter->isFollows("_", "8"));
			REQUIRE_FALSE(getter->isFollows("_", "9"));

			// Test for start of procedure
			REQUIRE_FALSE(getter->isFollows("_", "1"));
			REQUIRE_FALSE(getter->isFollows("_", "13"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : followedBy) {
				REQUIRE(getter->getFollows(key, false) == followedBy[key]);
			}
		}
	}

	SECTION("Follows(wildcard, wildcard)") {
		REQUIRE(getter->isFollows("_", "_"));
	}

	/* ============================== FollowsT ============================== */

	std::unordered_map<std::string, std::unordered_set<std::string>> followsT = {
		{"1", {"2", "3", "7", "10", "11", "12"}},
		{"2", {"3", "7", "10", "11", "12"}},
		{"3", {"7", "10", "11", "12"}},
		{"7", {"10", "11", "12"}},
		{"10", {"11", "12"}},
		{"11", {"12"}},
		{"4", {"5", "6"}},
		{"5", {"6"}},
		{"13", {"14", "15"}},
		{"14", {"15"}}
	};

	std::unordered_map<std::string, std::unordered_set<std::string>> followedByT = {
		{"2", {"1"}},
		{"3", {"1", "2"}},
		{"7", {"1", "2", "3"}},
		{"10", {"1", "2", "3", "7"}},
		{"11", {"1", "2", "3", "7", "10"}},
		{"12", {"1", "2", "3", "7", "10", "11"}},
		{"5", {"4"}},
		{"6", {"4", "5"}},
		{"14", {"13"}},
		{"15", {"13", "14"}},
	};

	SECTION("FollowsT(numeric_ref, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : followsT) {
				for (auto const& v : val) {
					REQUIRE(getter->isFollowsT(key, v));
				}
			}
		}

		SECTION("Negative") {
			// Test for different nesting levels
			REQUIRE_FALSE(getter->isFollowsT("2", "4"));
			REQUIRE_FALSE(getter->isFollowsT("6", "7"));
			REQUIRE_FALSE(getter->isFollowsT("8", "9"));
			REQUIRE_FALSE(getter->isFollowsT("6", "8"));
			REQUIRE_FALSE(getter->isFollowsT("9", "10"));
		}
	}

	SECTION("FollowsT(numeric_ref, wildcard)") {
		SECTION("Positive") {
			for (auto const& [key, val] : followsT) {
				REQUIRE(getter->isFollowsT(key, "_"));
			}
		}

		SECTION("Negative") {
			// Test for nesting levels that only has one statement
			REQUIRE_FALSE(getter->isFollowsT("8", "_"));
			REQUIRE_FALSE(getter->isFollowsT("9", "_"));

			// Test for end of procedure
			REQUIRE_FALSE(getter->isFollowsT("12", "_"));
			REQUIRE_FALSE(getter->isFollowsT("15", "_"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : followsT) {
				REQUIRE(getter->getFollowsT(key, true) == followsT[key]);
			}
		}
	}

	SECTION("FollowsT(wildcard, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : followedByT) {
				REQUIRE(getter->isFollowsT("_", key));
			}
		}

		SECTION("Negative") {
			// Test for nesting levels that only has one statement
			REQUIRE_FALSE(getter->isFollowsT("_", "8"));
			REQUIRE_FALSE(getter->isFollowsT("_", "9"));

			// Test for start of procedure
			REQUIRE_FALSE(getter->isFollowsT("_", "1"));
			REQUIRE_FALSE(getter->isFollowsT("_", "13"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : followedByT) {
				REQUIRE(getter->getFollowsT(key, false) == followedByT[key]);
			}
		}
	}

	SECTION("FollowsT(wildcard, wildcard)") {
		REQUIRE(getter->isFollowsT("_", "_"));
	}

	/* ============================== Next ============================== */

	std::unordered_map<std::string, std::unordered_set<std::string>> next = {
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
		{"13", {"14"}},
		{"14", {"15"}}
	};

	std::unordered_map<std::string, std::unordered_set<std::string>> prev = {
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
		{"14", {"13"}},
		{"15", {"14"}}
	};

	SECTION("Next(numeric_ref, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : next) {
				for (auto const& v : val) {
					REQUIRE(getter->isNext(key, v));
				}
			}
		}

		SECTION("Negative") {
			// Test if !isNext(<consecutive statements on the same nesting level>)
			REQUIRE_FALSE(getter->isNext("2", "1"));
			REQUIRE_FALSE(getter->isNext("5", "4"));
			REQUIRE_FALSE(getter->isNext("6", "5"));

			// Test for !isNext(if statement, next statement in the same nesting level)
			REQUIRE_FALSE(getter->isNext("7", "10"));

			// Test for !isNext(then statement, else statement) and vice versa
			REQUIRE_FALSE(getter->isNext("8", "9"));
			REQUIRE_FALSE(getter->isNext("9", "8"));

			// Test for !isNext(statement inside while statement, while statement)
			REQUIRE_FALSE(getter->isNext("4", "3"));
			REQUIRE_FALSE(getter->isNext("5", "3"));

			// Test for !isNext(statement inside while statement, if statement)
			REQUIRE_FALSE(getter->isNext("4", "7"));
			REQUIRE_FALSE(getter->isNext("5", "7"));
		}
	}

	SECTION("Next(numeric_ref, wildcard)") {
		SECTION("Positive") {
			for (auto const& [key, val] : next) {
				REQUIRE(getter->isNext(key, "_"));
			}
		}

		SECTION("Negative") {
			// Test for end of procedure
			REQUIRE_FALSE(getter->isNext("12", "_"));
			REQUIRE_FALSE(getter->isNext("15", "_"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : next) {
				REQUIRE(getter->getNext(key, true) == next[key]);
			}
		}
	}

	SECTION("Next(wildcard, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : prev) {
				REQUIRE(getter->isNext("_", key));
			}
		}

		SECTION("Negative") {
			// Test for start of procedure
			REQUIRE_FALSE(getter->isNext("_", "1"));
			REQUIRE_FALSE(getter->isNext("_", "13"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : prev) {
				REQUIRE(getter->getNext(key, false) == prev[key]);
			}
		}
	}

	SECTION("Next(wildcard, wildcard)") {
		REQUIRE(getter->isNext("_", "_"));
	}

	/* ============================== NextT ============================== */

	std::unordered_map<std::string, std::unordered_set<std::string>> nextT = {
		{"1", {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}},
		{"2", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}},
		{"3", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}},
		{"4", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}},
		{"5", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}},
		{"6", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12"}},
		{"7", {"8", "9", "10", "11", "12"}},
		{"8", {"10", "11", "12"}},
		{"9", {"10", "11", "12"}},
		{"10", {"11", "12"}},
		{"11", {"12"}},
		{"13", {"14", "15"}},
		{"14", {"15"}}
	};

	std::unordered_map<std::string, std::unordered_set<std::string>> prevT = {
		{"2", {"1"}},
		{"3", {"1", "2", "3", "4", "5", "6"}},
		{"4", {"1", "2", "3", "4", "5", "6"}},
		{"5", {"1", "2", "3", "4", "5", "6"}},
		{"6", {"1", "2", "3", "4", "5", "6"}},
		{"7", {"1", "2", "3", "4", "5", "6"}},
		{"8", {"1", "2", "3", "4", "5", "6", "7"}},
		{"9", {"1", "2", "3", "4", "5", "6", "7"}},
		{"10", {"1", "2", "3", "4", "5", "6", "7", "8", "9"}},
		{"11", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}},
		{"12", {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"}},
		{"14", {"13"}},
		{"15", {"13", "14"}},
	};

	SECTION("NextT(numeric_ref, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : nextT) {
				for (auto const& v : val) {
					REQUIRE(getter->isNextT(key, v));
				}
			}
		}

		SECTION("Negative") {
			// Test if !isNext(<consecutive statements on the same nesting level>)
			REQUIRE_FALSE(getter->isNextT("2", "1"));
			REQUIRE_FALSE(getter->isNextT("10", "2"));
			REQUIRE_FALSE(getter->isNextT("11", "10"));
			REQUIRE_FALSE(getter->isNextT("12", "11"));

			// Test for !isNext(then statement, else statement) and vice versa
			REQUIRE_FALSE(getter->isNextT("8", "9"));
			REQUIRE_FALSE(getter->isNextT("9", "8"));
		}
	}

	SECTION("NextT(numeric_ref, wildcard)") {
		SECTION("Positive") {
			for (auto const& [key, val] : nextT) {
				REQUIRE(getter->isNextT(key, "_"));
			}
		}

		SECTION("Negative") {
			// Test for end of procedure
			REQUIRE_FALSE(getter->isNextT("12", "_"));
			REQUIRE_FALSE(getter->isNextT("15", "_"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : nextT) {
				REQUIRE(getter->getNextT(key, true) == nextT[key]);
			}
		}
	}

	SECTION("NextT(wildcard, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : prevT) {
				REQUIRE(getter->isNextT("_", key));
			}
		}

		SECTION("Negative") {
			// Test for start of procedure
			REQUIRE_FALSE(getter->isNextT("_", "1"));
			REQUIRE_FALSE(getter->isNextT("_", "13"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : prevT) {
				REQUIRE(getter->getNextT(key, false) == prevT[key]);
			}
		}
	}

	SECTION("NextT(wildcard, wildcard)") {
		REQUIRE(getter->isNextT("_", "_"));
	}

	/* ============================== Affects ============================== */
	
	std::unordered_map<std::string, std::unordered_set<std::string>> affects = {
		{"1", {"4", "8", "10", "12"}},
		{"2", {"6", "10"}},
		{"4", {"4", "8", "10", "12"}},
		{"6", {"6", "10"}},
		{"8", {"10", "12"}},
		{"9", {"10"}},
		{"10", {"11", "12"}},
		{"11", {"12"}},
		{"13", {"14"}}
	};

	std::unordered_map<std::string, std::unordered_set<std::string>> affectedBy = {
		{"4", {"1", "4"}},
		{"6", {"2", "6"}},
		{"8", {"1", "4"}},
		{"10", {"1", "2", "4", "6" ,"8", "9"}},
		{"11", {"10"}},
		{"12", {"1", "4", "8", "10", "11"}},
		{"14", {"13"}}
	};

	SECTION("Affects(numeric_ref, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : affects) {
				for (auto const& v : val) {
					REQUIRE(getter->isAffects(key, v));
				}
			}
		}

		SECTION("Negative") {
			// not assign stmt
			REQUIRE_FALSE(getter->isAffects("2", "3"));
			REQUIRE_FALSE(getter->isAffects("1", "7"));
			REQUIRE_FALSE(getter->isAffects("4", "7"));
			REQUIRE_FALSE(getter->isAffects("14", "15"));

			// different proc
			REQUIRE_FALSE(getter->isAffects("1", "13"));
			REQUIRE_FALSE(getter->isAffects("1", "14"));
			REQUIRE_FALSE(getter->isAffects("4", "13"));

			// no path
			REQUIRE_FALSE(getter->isAffects("6", "2"));

			// var modified in execution path
			REQUIRE_FALSE(getter->isAffects("9", "11"));

			// var not used
			REQUIRE_FALSE(getter->isAffects("1", "2"));
			REQUIRE_FALSE(getter->isAffects("4", "6"));

			// does not recursively call itself
			REQUIRE_FALSE(getter->isAffects("1", "1"));
			REQUIRE_FALSE(getter->isAffects("2", "2"));
			REQUIRE_FALSE(getter->isAffects("8", "8"));
			REQUIRE_FALSE(getter->isAffects("9", "9"));
			REQUIRE_FALSE(getter->isAffects("10", "10"));
			REQUIRE_FALSE(getter->isAffects("11", "11"));
			REQUIRE_FALSE(getter->isAffects("13", "13"));
		}
	}

	SECTION("Affects(numeric_ref, wildcard)") {
		SECTION("Positive") {
			for (auto const& [key, val] : affects) {
				REQUIRE(getter->isAffects(key, "_"));
			}
		}

		SECTION("Negative") {
			// not assign stmt
			REQUIRE_FALSE(getter->isAffects("3", "_"));
			REQUIRE_FALSE(getter->isAffects("5", "_"));
			REQUIRE_FALSE(getter->isAffects("7", "_"));
			REQUIRE_FALSE(getter->isAffects("15", "_"));

			// var not used in any RHS assignment stmt
			REQUIRE_FALSE(getter->isAffects("12", "_"));
			REQUIRE_FALSE(getter->isAffects("14", "_"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : affects) {
				REQUIRE(getter->getAffects(key, true) == affects[key]);
			}
		}
	}

	SECTION("Affects(wildcard, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : affectedBy) {
				REQUIRE(getter->isAffects("_", key));
			}
		}

		SECTION("Negative") {
			// not assign stmt
			REQUIRE_FALSE(getter->isAffects("_", "3"));
			REQUIRE_FALSE(getter->isAffects("_", "5"));
			REQUIRE_FALSE(getter->isAffects("_", "7"));
			REQUIRE_FALSE(getter->isAffects("_", "15"));

			// RHS stmt is first assignment of var
			REQUIRE_FALSE(getter->isAffects("_", "1"));
			REQUIRE_FALSE(getter->isAffects("_", "2"));
			REQUIRE_FALSE(getter->isAffects("_", "9"));
			REQUIRE_FALSE(getter->isAffects("_", "13"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : affectedBy) {
				REQUIRE(getter->getAffects(key, false) == affectedBy[key]);
			}
		}
	}

	SECTION("Affects(wildcard, wildcard)") {
		REQUIRE(getter->isAffects("_", "_"));
	}

	/* ============================== AffectsT ============================== */

	std::unordered_map<std::string, std::unordered_set<std::string>> affectsT = {
		{"1", {"4", "8", "10", "11", "12"}},
		{"2", {"6", "10", "11", "12"}},
		{"4", {"4", "8", "10", "11", "12"}},
		{"6", {"6", "10", "11", "12"}},
		{"8", {"10", "11", "12"}},
		{"9", {"10", "11", "12"}},
		{"10", {"11", "12"}},
		{"11", {"12"}},
		{"13", {"14"}}
	};

	std::unordered_map<std::string, std::unordered_set<std::string>> affectedByT = {
		{"4", {"1", "4"}},
		{"6", {"2", "6"}},
		{"8", {"1", "4"}},
		{"10", {"1", "2", "4", "6" ,"8", "9"}},
		{"11", {"1", "2", "4", "6" ,"8", "9", "10"}},
		{"12", {"1", "2", "4", "6" ,"8", "9", "10", "11"}},
		{"14", {"13"}}
	};

	SECTION("AffectsT(numeric_ref, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : affectsT) {
				for (auto const& v : val) {
					REQUIRE(getter->isAffectsT(key, v));
				}
			}
		}

		SECTION("Negative") {
			// not assign stmt
			REQUIRE_FALSE(getter->isAffectsT("2", "3"));
			REQUIRE_FALSE(getter->isAffectsT("1", "7"));
			REQUIRE_FALSE(getter->isAffectsT("4", "7"));
			REQUIRE_FALSE(getter->isAffectsT("14", "15"));

			// different proc
			REQUIRE_FALSE(getter->isAffectsT("1", "13"));
			REQUIRE_FALSE(getter->isAffectsT("1", "14"));
			REQUIRE_FALSE(getter->isAffectsT("4", "13"));

			// no path
			REQUIRE_FALSE(getter->isAffectsT("6", "2"));

			// var not used
			REQUIRE_FALSE(getter->isAffectsT("1", "2"));
			REQUIRE_FALSE(getter->isAffectsT("4", "6"));

			// does not recursively call itself
			REQUIRE_FALSE(getter->isAffectsT("1", "1"));
			REQUIRE_FALSE(getter->isAffectsT("2", "2"));
			REQUIRE_FALSE(getter->isAffectsT("8", "8"));
			REQUIRE_FALSE(getter->isAffectsT("9", "9"));
			REQUIRE_FALSE(getter->isAffectsT("10", "10"));
			REQUIRE_FALSE(getter->isAffectsT("11", "11"));
			REQUIRE_FALSE(getter->isAffectsT("13", "13"));
		}
	}

	SECTION("AffectsT(numeric_ref, wildcard)") {
		SECTION("Positive") {
			for (auto const& [key, val] : affectsT) {
				REQUIRE(getter->isAffectsT(key, "_"));
			}
		}

		SECTION("Negative") {
			// not assign stmt
			REQUIRE_FALSE(getter->isAffectsT("3", "_"));
			REQUIRE_FALSE(getter->isAffectsT("5", "_"));
			REQUIRE_FALSE(getter->isAffectsT("7", "_"));
			REQUIRE_FALSE(getter->isAffectsT("15", "_"));

			// var not used in any RHS assignment stmt
			REQUIRE_FALSE(getter->isAffectsT("12", "_"));
			REQUIRE_FALSE(getter->isAffectsT("14", "_"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : affectsT) {
				REQUIRE(getter->getAffectsT(key, true) == affectsT[key]);
			}
		}
	}

	SECTION("AffectsT(wildcard, numeric_ref)") {
		SECTION("Positive") {
			for (auto const& [key, val] : affectedByT) {
				REQUIRE(getter->isAffectsT("_", key));
			}
		}

		SECTION("Negative") {
			// not assign stmt
			REQUIRE_FALSE(getter->isAffectsT("_", "3"));
			REQUIRE_FALSE(getter->isAffectsT("_", "5"));
			REQUIRE_FALSE(getter->isAffectsT("_", "7"));
			REQUIRE_FALSE(getter->isAffectsT("_", "15"));

			// RHS stmt is first assignment of var
			REQUIRE_FALSE(getter->isAffectsT("_", "1"));
			REQUIRE_FALSE(getter->isAffectsT("_", "2"));
			REQUIRE_FALSE(getter->isAffectsT("_", "9"));
			REQUIRE_FALSE(getter->isAffectsT("_", "13"));
		}

		SECTION("Set Return") {
			for (auto const& [key, val] : affectedByT) {
				REQUIRE(getter->getAffectsT(key, false) == affectedByT[key]);
			}
		}
	}

	SECTION("AffectsT(wildcard, wildcard)") {
		REQUIRE(getter->isAffectsT("_", "_"));
	}
}
