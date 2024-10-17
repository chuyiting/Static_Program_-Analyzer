#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "SP/SourceProcessor.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

TEST_CASE("Compute Centroid Procedures") {
	std::string sourceCode =
		"procedure computeCentroid {"
		"	count = 0;" // 1
		"	cenX = 0;" // 2
		"	cenY = 0;" // 3
		"	call readPoint;" // 4
		"	while ((x != 0) && (y != 0)) {" // 5
		"		count = count + 1;" // 6
		"		cenX = cenX + x;" // 7
		"		cenY = cenY + y;" // 8
		"		call readPoint;" // 9
		"	}"
		"	if (count == 0) then {" // 10
		"		flag = 1;" // 11
		"	} else {"
		"		cenX = cenX / count;" // 12
		"		cenY = cenY / count;" // 13
		"	}"
		"	normSq = cenX * cenX + cenY * cenY;" // 14
		"}"
		"procedure main {"
		"	flag = 0;" // 15
		"	call computeCentroid;" // 16
		"	call printResults;" // 17
		"}"
		"procedure readPoint {"
		"	read x;" // 18
		"	read y;" // 19
		"}"
		"procedure printResults {"
		"	print flag;" // 20
		"	print cenX;" // 21
		"	print cenY;" // 22
		"	print normSq;" // 23
		"}";

	PKB pkb = PKB();
	std::shared_ptr<PKBSetter> setter = std::make_shared<PKBSetter>(PKBSetter(pkb));
	std::shared_ptr<PKBGetter> getter = std::make_shared<PKBGetter>(PKBGetter(pkb));
	SourceProcessor SP = SourceProcessor(setter);
	SP.processCode(sourceCode);
	setter->done();

	SECTION("Procedures") {
		std::unordered_set<Procedure> actualAbstractions = getter->getProcedures();
		std::unordered_set<Procedure> expectedAbstractions = {
			"computeCentroid", "main", "readPoint", "printResults"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Constants") {
		std::unordered_set<Constant> actualAbstractions = getter->getConstants();
		std::unordered_set<Constant> expectedAbstractions = {
			"0", "1"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Variables") {
		std::unordered_set<Variable> actualAbstractions = getter->getVariables();
		std::unordered_set<Variable> expectedAbstractions = {
			"count", "cenX", "cenY", "x", "y", "flag", "normSq"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Read Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getReadStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"18", "19"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Print Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getPrintStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"20", "21", "22", "23"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Assign Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getAssignStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"1", "2", "3", "6", "7", "8", "11", "12", "13", "14", "15"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("If Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getIfStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"10"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("While Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getWhileStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"5"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Call Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getCallStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"4", "9", "16", "17"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("All Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getStatements();
		std::unordered_set<Statement> expectedAbstractions = {
			"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14",
			"15", "16", "17", "18", "19", "20", "21", "22", "23"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Follows") {
		std::unordered_map<Statement, Statement> actualAbstractions = getter->getFollows();
		std::unordered_map<Statement, Statement> expectedAbstractions = {
			// Follows 1-2-3-4-5-10-14
			{"1", "2"}, {"2", "3"}, {"3", "4"}, {"4", "5"}, {"5", "10"}, {"10", "14"},
			// Follows 6-7-8-9
			{"6", "7"}, {"7", "8"}, {"8", "9"},
			// Follows 12-13
			{"12", "13"},
			// Follows 15-16-17
			{"15", "16"}, {"16", "17"},
			// Follows 18-19
			{"18", "19"},
			// Follows 20-21-22-23
			{"20", "21"}, {"21", "22"}, {"22", "23"}
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("FollowsT") {
		std::unordered_map<Statement, std::unordered_set<Statement>> actualAbstractions = getter->getFollowsT();
		std::unordered_map<Statement, std::unordered_set<Statement>> expectedAbstractions = {
			// Follows 1-2-3-4-5-10-14
		    	{"0", {}},
			{"1", {"2", "3", "4", "5", "10", "14"}},
			{"2", {"3", "4", "5", "10", "14"}},
			{"3", {"4", "5", "10", "14"}},
			{"4", {"5", "10", "14"}},
			{"5", {"10", "14"}},
			// Follows 6-7-8-9
			{"6", {"7", "8", "9"}},
			{"7", {"8", "9"}},
			{"8", {"9"}},
			// Follows 12-13
			{"9", {}},
			{"10", {"14"}},
			{"11", {}},
			{"12", {"13"}},
			{"13", {}},
			// Follows 15-16-17
			{"14", {}},
			{"15", {"16", "17"}},
			{"16", {"17"}},
			{"17", {}},
			// Follows 18-19
			{"18", {"19"}},
			{"19", {}},
		    	// Follows 20-21-22-23
			{"20", {"21", "22", "23"}},
			{"21", {"22", "23"}},
			{"22", {"23"}},
		    	{"23", {}}
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Parent") {
		std::unordered_map<Statement, std::unordered_set<Statement>> actualAbstractions = getter->getParent();
    std::unordered_map<Statement, std::unordered_set<Statement>> expectedAbstractions = {
			// Parent 5
			{"1", {}},
			{"0", {}},
			{"2", {}},
			{"3", {}},
			{"4", {}},
			{"5", {"6", "7", "8", "9"}},
			{"6", {}},
			{"7", {}},
			{"8", {}},
			{"9", {}},
			// Parent 10
			{"10", {"11", "12", "13"}},
			{"11", {}},
			{"12", {}},
			{"13", {}},
			{"14", {}},
			{"15", {}},
			{"16", {}},
			{"17", {}},
			{"18", {}},
			{"19", {}},
			{"20", {}},
			{"21", {}},
			{"22", {}},
			{"23", {}},
		    };
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("ParentT") {
    std::unordered_map<Statement, std::unordered_set<Statement>> actualAbstractions = getter->getParentT();
    std::unordered_map<Statement, std::unordered_set<Statement>> expectedAbstractions = {
			// Parent 5
			{"0", {}},
			{"1", {}},
			{"2", {}},
			{"3", {}},
			{"4", {}},
			{"5", {"6", "7", "8", "9"}},
			{"6", {}},
			{"7", {}},
			{"8", {}},
			{"9", {}},
			// Parent 10
			{"10", {"11", "12", "13"}},
			{"11", {}},
			{"12", {}},
			{"13", {}},
			{"14", {}},
			{"15", {}},
			{"16", {}},
			{"17", {}},
			{"18", {}},
			{"19", {}},
			{"20", {}},
			{"21", {}},
			{"22", {}},
			{"23", {}},
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("UsesS") {
		std::unordered_map<Statement, std::unordered_set<Variable>> actualAbstractions = getter->getUsesS();
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedAbstractions = {
			// computeCentroid
			{"1", {}},
			{"2", {}},
			{"3", {}},
			{"4", {}},
			{"5", {"x", "y", "count", "cenX", "cenY"}},
			{"6", {"count"}},
			{"7", {"cenX", "x"}},
			{"8", {"cenY", "y"}},
			{"9", {}},
			{"10", {"count", "cenX", "cenY"}},
			{"11", {}},
			{"12", {"cenX", "count"}},
			{"13", {"cenY", "count"}},
			{"14", {"cenX", "cenY"}},
			// main
			{"15", {}},
			{"16", {"x", "y", "count", "cenX", "cenY"}},
			{"17", {"flag", "cenX", "cenY", "normSq"}},
			// readPoint
			{"18", {}},
			{"19", {}},
			// printResults
			{"20", {"flag"}},
			{"21", {"cenX"}},
			{"22", {"cenY"}},
			{"23", {"normSq"}}
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("UsesP") {
		std::unordered_map<Statement, std::unordered_set<Variable>> actualAbstractions = getter->getUsesP();
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedAbstractions = {
			{"computeCentroid", {"x", "y", "count", "cenX", "cenY"}},
			{"main", {"x", "y", "count", "cenX", "cenY", "flag", "normSq"}},
			{"readPoint", {}},
			{"printResults", {"flag", "cenX", "cenY", "normSq"}}
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("ModifiesS") {
		std::unordered_map<Statement, std::unordered_set<Variable>> actualAbstractions = getter->getModifiesS();
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedAbstractions = {
			// computeCentroid
			{"1", {"count"}},
			{"2", {"cenX"}},
			{"3", {"cenY"}},
			{"4", {"x", "y"}},
			{"5", {"count", "cenX", "cenY", "x", "y"}},
			{"6", {"count"}},
			{"7", {"cenX"}},
			{"8", {"cenY"}},
			{"9", {"x", "y"}},
			{"10", {"flag", "cenX", "cenY"}},
			{"11", {"flag"}},
			{"12", {"cenX"}},
			{"13", {"cenY"}},
			{"14", {"normSq"}},
			// main
			{"15", {"flag"}},
			{"16", {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}},
			// readPoint
			{"18", {"x"}},
			{"19", {"y"}},
			// printResults (none)
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("ModifiesP") {
		std::unordered_map<Statement, std::unordered_set<Variable>> actualAbstractions = getter->getModifiesP();
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedAbstractions = {
			{"computeCentroid", {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}},
			{"main", {"count", "cenX", "cenY", "x", "y", "flag", "normSq"}},
			{"readPoint", {"x", "y"}},
			{"printResults", {}}
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	/* =========================================== NEXT =========================================== */

	SECTION("isNext(numeric_ref, numeric_ref)") {
		// computeCentroid
		REQUIRE(getter->isNext("1", "2"));
		REQUIRE(getter->isNext("2", "3"));
		REQUIRE(getter->isNext("3", "4"));
		REQUIRE(getter->isNext("4", "5"));
		REQUIRE(getter->isNext("5", "6"));
		REQUIRE(getter->isNext("6", "7"));
		REQUIRE(getter->isNext("7", "8"));
		REQUIRE(getter->isNext("8", "9"));
		REQUIRE(getter->isNext("9", "5"));
		REQUIRE(getter->isNext("5", "10"));
		REQUIRE(getter->isNext("10", "11"));
		REQUIRE(getter->isNext("11", "14"));
		REQUIRE(getter->isNext("10", "12"));
		REQUIRE(getter->isNext("12", "13"));
		REQUIRE(getter->isNext("13", "14"));

		// main
		REQUIRE(getter->isNext("15", "16"));
		REQUIRE(getter->isNext("16", "17"));

		// readPoint
		REQUIRE(getter->isNext("18", "19"));

		// printResults
		REQUIRE(getter->isNext("20", "21"));
		REQUIRE(getter->isNext("21", "22"));
		REQUIRE(getter->isNext("22", "23"));
	}

	SECTION("Negative: isNext(numeric_ref, numeric_ref)") {
		// Test if !isNext(<consecutive statements on the same nesting level>)
		REQUIRE_FALSE(getter->isNext("2", "1"));
		REQUIRE_FALSE(getter->isNext("7", "6"));
		REQUIRE_FALSE(getter->isNext("8", "7"));
		REQUIRE_FALSE(getter->isNext("9", "8"));
		REQUIRE_FALSE(getter->isNext("13", "12"));

		// Test for !isNext(if statement, next statement in the same nesting level)
		REQUIRE_FALSE(getter->isNext("10", "14"));

		// Test for !isNext(then statement, else statement) and vice versa
		REQUIRE_FALSE(getter->isNext("11", "12"));
		REQUIRE_FALSE(getter->isNext("12", "11"));

		// Test for !isNext(statement inside while statement, while statement)
		REQUIRE_FALSE(getter->isNext("6", "5"));
		REQUIRE_FALSE(getter->isNext("7", "5"));
		REQUIRE_FALSE(getter->isNext("8", "5"));
	}

	SECTION("Positive: isNext(numeric_ref, wildcard)") {
		REQUIRE(getter->isNext("1", "_"));
		REQUIRE(getter->isNext("2", "_"));
		REQUIRE(getter->isNext("3", "_"));
		REQUIRE(getter->isNext("4", "_"));
		REQUIRE(getter->isNext("5", "_"));
		REQUIRE(getter->isNext("6", "_"));
		REQUIRE(getter->isNext("7", "_"));
		REQUIRE(getter->isNext("8", "_"));
		REQUIRE(getter->isNext("9", "_"));
		REQUIRE(getter->isNext("10", "_"));
		REQUIRE(getter->isNext("11", "_"));
		REQUIRE(getter->isNext("12", "_"));
		REQUIRE(getter->isNext("13", "_"));
		REQUIRE(getter->isNext("15", "_"));
		REQUIRE(getter->isNext("16", "_"));
		REQUIRE(getter->isNext("18", "_"));
		REQUIRE(getter->isNext("20", "_"));
		REQUIRE(getter->isNext("21", "_"));
		REQUIRE(getter->isNext("22", "_"));
	}

	SECTION("Negative: isNext(numeric_ref, wildcard)") {
		REQUIRE_FALSE(getter->isNext("14", "_"));
		REQUIRE_FALSE(getter->isNext("17", "_"));
		REQUIRE_FALSE(getter->isNext("19", "_"));
		REQUIRE_FALSE(getter->isNext("23", "_"));
	}

	SECTION("Positive: isNext(wildcard, numeric_ref)") {
		REQUIRE(getter->isNext("_", "2"));
		REQUIRE(getter->isNext("_", "3"));
		REQUIRE(getter->isNext("_", "4"));
		REQUIRE(getter->isNext("_", "5"));
		REQUIRE(getter->isNext("_", "6"));
		REQUIRE(getter->isNext("_", "7"));
		REQUIRE(getter->isNext("_", "8"));
		REQUIRE(getter->isNext("_", "9"));
		REQUIRE(getter->isNext("_", "10"));
		REQUIRE(getter->isNext("_", "11"));
		REQUIRE(getter->isNext("_", "12"));
		REQUIRE(getter->isNext("_", "13"));
		REQUIRE(getter->isNext("_", "14"));
		REQUIRE(getter->isNext("_", "16"));
		REQUIRE(getter->isNext("_", "17"));
		REQUIRE(getter->isNext("_", "19"));
		REQUIRE(getter->isNext("_", "21"));
		REQUIRE(getter->isNext("_", "22"));
		REQUIRE(getter->isNext("_", "23"));
	}

	SECTION("Negative: isNext(wildcard, numeric_ref)") {
		REQUIRE_FALSE(getter->isNext("_", "1"));
		REQUIRE_FALSE(getter->isNext("_", "15"));
		REQUIRE_FALSE(getter->isNext("_", "18"));
		REQUIRE_FALSE(getter->isNext("_", "20"));
	}

	SECTION("isNext(numeric_ref, wildcard)") {
		REQUIRE(getter->isNext("_", "_"));
	}

    /* ============================== Affects ============================== */

    SECTION("Positive: Affects(numeric_ref, numeric_ref)") {
		// 1 count
        REQUIRE(getter->isAffects("1","6"));
        REQUIRE(getter->isAffects("1","12"));
		REQUIRE(getter->isAffects("1","13"));

		// 2 cenX
        REQUIRE(getter->isAffects("2","7"));
		REQUIRE(getter->isAffects("2","12"));
        REQUIRE(getter->isAffects("2","14"));

		// 3 cenY
        REQUIRE(getter->isAffects("3","8"));
        REQUIRE(getter->isAffects("3","13"));
        REQUIRE(getter->isAffects("3","14"));

		// 6 count
        REQUIRE(getter->isAffects("6","12"));
		REQUIRE(getter->isAffects("6","13"));

		// 7 cenX
        REQUIRE(getter->isAffects("7","12"));
        REQUIRE(getter->isAffects("7","14"));

		// 8 cenY
        REQUIRE(getter->isAffects("8","13"));
        REQUIRE(getter->isAffects("8","14"));

		// 12 cenX
        REQUIRE(getter->isAffects("12","14"));

		// 13 cenY
        REQUIRE(getter->isAffects("13","14"));

		// path from 6->6
		REQUIRE(getter->isAffects("6", "6"));

		// path from 7->7
		REQUIRE(getter->isAffects("7", "7"));

		// path from 8->8
		REQUIRE(getter->isAffects("8", "8"));
    }

    SECTION("Negative: isAffects(numeric_ref, numeric_ref)") {
		// not assign stmt
		REQUIRE_FALSE(getter->isAffects("1", "5"));
		REQUIRE_FALSE(getter->isAffects("1", "10"));
		REQUIRE_FALSE(getter->isAffects("4", "7"));
		REQUIRE_FALSE(getter->isAffects("4", "8"));

		// different proc
		REQUIRE_FALSE(getter->isAffects("15", "11"));

		// no path
		REQUIRE_FALSE(getter->isAffects("6", "1"));

		// var not used
        REQUIRE_FALSE(getter->isAffects("1","7"));
        REQUIRE_FALSE(getter->isAffects("2","3"));
        REQUIRE_FALSE(getter->isAffects("6","14"));
    }

    SECTION("Positive: isAffects(numeric_ref, wildcard)") {
        REQUIRE(getter->isAffects("1", "_"));
        REQUIRE(getter->isAffects("2", "_"));
        REQUIRE(getter->isAffects("3", "_"));
        REQUIRE(getter->isAffects("6", "_"));
        REQUIRE(getter->isAffects("7", "_"));
        REQUIRE(getter->isAffects("8", "_"));
        REQUIRE(getter->isAffects("12", "_"));
        REQUIRE(getter->isAffects("13", "_"));
    }

    SECTION("Negative: isAffects(numeric_ref, wildcard)") {
		// not assign stmt
        REQUIRE_FALSE(getter->isAffects("4", "_"));
        REQUIRE_FALSE(getter->isAffects("9", "_"));
        REQUIRE_FALSE(getter->isAffects("10", "_"));

		// var not used in any RHS assignment stmt
        REQUIRE_FALSE(getter->isAffects("11", "_"));
		REQUIRE_FALSE(getter->isAffects("14", "_"));
		REQUIRE_FALSE(getter->isAffects("15", "_"));
    }

    SECTION("Positive: isAffects(wildcard, numeric_ref)") {
        REQUIRE(getter->isAffects("_", "6"));
        REQUIRE(getter->isAffects("_", "7"));
        REQUIRE(getter->isAffects("_", "8"));
        REQUIRE(getter->isAffects("_", "12"));
        REQUIRE(getter->isAffects("_", "13"));
        REQUIRE(getter->isAffects("_", "14"));
    }

    SECTION("Negative: isAffects(wildcard, numeric_ref)") {
		// not assign stmt
		REQUIRE_FALSE(getter->isAffects("_", "5"));
		REQUIRE_FALSE(getter->isAffects("_", "9"));
		REQUIRE_FALSE(getter->isAffects("_", "10"));
		
		// RHS stmt is first assignment of var
        REQUIRE_FALSE(getter->isAffects("_", "1"));
		REQUIRE_FALSE(getter->isAffects("_", "2"));
		REQUIRE_FALSE(getter->isAffects("_", "3"));
		REQUIRE_FALSE(getter->isAffects("_", "11"));
		REQUIRE_FALSE(getter->isAffects("_", "15"));
    }

    SECTION("isAffects(wildcard, wildcard)") {
        REQUIRE(getter->isAffects("_", "_"));
    }

        /* ============================== AffectsT ============================== */

    SECTION("Positive: AffectsT(numeric_ref, numeric_ref)") {
		REQUIRE(getter->isAffectsT("1","6"));
        REQUIRE(getter->isAffectsT("1","12"));
        REQUIRE(getter->isAffectsT("2","7"));
        REQUIRE(getter->isAffectsT("2","14"));
        REQUIRE(getter->isAffectsT("3","8"));
        REQUIRE(getter->isAffectsT("3","13"));
        REQUIRE(getter->isAffectsT("3","14"));
        REQUIRE(getter->isAffectsT("6","12"));
        REQUIRE(getter->isAffectsT("7","12"));
        REQUIRE(getter->isAffectsT("7","14"));
        REQUIRE(getter->isAffectsT("8","13"));
        REQUIRE(getter->isAffectsT("8","14"));
        REQUIRE(getter->isAffectsT("12","14"));
        REQUIRE(getter->isAffectsT("13","14"));

        // AffectsT
        REQUIRE(getter->isAffectsT("1","14"));
        REQUIRE(getter->isAffectsT("2","12"));
        REQUIRE(getter->isAffectsT("2","14"));
        REQUIRE(getter->isAffectsT("3","13"));
        REQUIRE(getter->isAffectsT("3","14"));
        REQUIRE(getter->isAffectsT("6","14"));
    }

    SECTION("Negative: isAffectsT(numeric_ref, numeric_ref)") {
        // var not used
        REQUIRE_FALSE(getter->isAffectsT("1","7"));
        REQUIRE_FALSE(getter->isAffectsT("2","3"));

        // call stmt
        REQUIRE_FALSE(getter->isAffectsT("4","7"));
        REQUIRE_FALSE(getter->isAffectsT("4","8"));

        // if stmt
        REQUIRE_FALSE(getter->isAffectsT("10","12"));
    }

    SECTION("Positive: isAffectsT(numeric_ref, wildcard)") {
        REQUIRE(getter->isAffectsT("1", "_"));
        REQUIRE(getter->isAffectsT("2", "_"));
        REQUIRE(getter->isAffectsT("3", "_"));
        REQUIRE(getter->isAffectsT("6", "_"));
        REQUIRE(getter->isAffectsT("7", "_"));
        REQUIRE(getter->isAffectsT("8", "_"));
        REQUIRE(getter->isAffectsT("12", "_"));
        REQUIRE(getter->isAffectsT("13", "_"));
    }

    SECTION("Negative: isAffectsT(numeric_ref, wildcard)") {
        // call stmt
        REQUIRE_FALSE(getter->isAffectsT("4", "_"));
        REQUIRE_FALSE(getter->isAffectsT("9", "_"));

        // if stmt
        REQUIRE_FALSE(getter->isAffectsT("10", "_"));

        // var not used in any assignments
        REQUIRE_FALSE(getter->isAffectsT("11", "_"));
    }

    SECTION("Positive: isAffectsT(wildcard, numeric_ref)") {
        REQUIRE(getter->isAffectsT("_", "6"));
        REQUIRE(getter->isAffectsT("_", "7"));
        REQUIRE(getter->isAffectsT("_", "8"));
        REQUIRE(getter->isAffectsT("_", "12"));
        REQUIRE(getter->isAffectsT("_", "13"));
        REQUIRE(getter->isAffectsT("_", "14"));
    }

    SECTION("Negative: isAffectsT(wildcard, numeric_ref)") {
        // first stmt
        REQUIRE_FALSE(getter->isAffectsT("_", "1"));

        // no vars used in the RHS statement
        REQUIRE_FALSE(getter->isAffectsT("_", "2"));
        REQUIRE_FALSE(getter->isAffectsT("_", "3"));

        // while stmt
        REQUIRE_FALSE(getter->isAffectsT("_", "5"));

        // call stmt
        REQUIRE_FALSE(getter->isAffectsT("_", "9"));

        // if stmt
        REQUIRE_FALSE(getter->isAffectsT("_", "10"));
    }

    SECTION("isAffectsT(wildcard, wildcard)") {
        REQUIRE(getter->isAffectsT("_", "_"));
    }

}