#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "SP/SourceProcessor.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

TEST_CASE("SIMPLE IF") {
	std::string sourceCode =
		"procedure foo {"
		"	if (x == 10) then {" // 1
		"		read a;" // 2
		"		print b;" // 3
		"	} else {"
		"		c = 30;" // 4
		"	}"
		"	read a;" // 5
		"}";

	PKB pkb = PKB();
	std::shared_ptr<PKBSetter> setter = std::make_shared<PKBSetter>(PKBSetter(pkb));
	std::shared_ptr<PKBGetter> getter = std::make_shared<PKBGetter>(PKBGetter(pkb));
	SourceProcessor SP = SourceProcessor(setter);
	SP.processCode(sourceCode);
	setter->done();

	SECTION("Positive: isNext(numeric_ref, numeric_ref)") {
		REQUIRE(getter->isNext("1", "2"));
		REQUIRE(getter->isNext("1", "4"));
		REQUIRE(getter->isNext("2", "3"));
		REQUIRE(getter->isNext("3", "5"));
		REQUIRE(getter->isNext("4", "5"));
	}
}

TEST_CASE("Nested If Statements") {
	std::string sourceCode =
		"procedure foo {"
		"  if (x == 10) then {" // 1
		"    if (x == 20) then {" // 2
		"      read a;" // 3
		"      print b;" // 4
		"    } else {"
		"      c = 30;" // 5
		"      if (x == 40) then {" // 6
		"        call bar;" // 7
		"        read a;" // 8
		"      } else {"
		"        print b;" // 9
		"        c = 50;" // 10
		"      }"
		"      call bar;" // 11
		"    }"
		"    read a;" // 12
		"    print b;" // 13
		"  } else {"
		"    c = 60;" // 14
		"    call bar;" // 15
		"    if ((x >= 70) && (x <= 80)) then {" // 16
		"      read a;" // 17
		"      if (x == 90) then {" // 18
		"        print b;" // 19
		"        c = 100;" // 20
		"      } else {"
		"        call bar;" // 21
		"        read a;" // 22
		"      }"
		"      print b;" // 23
		"    } else {"
		"      c = 110;" // 24
		"      call bar;" // 25
		"    }"
		"  }"
		"}"
		"procedure bar {"
		"  z = 120;" // 26
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
			"foo", "bar"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Constants") {
		std::unordered_set<Constant> actualAbstractions = getter->getConstants();
		std::unordered_set<Constant> expectedAbstractions = {
			"10", "20", "30", "40", "50", "60", "70", "80", "90", "100", "110", "120"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Variables") {
		std::unordered_set<Variable> actualAbstractions = getter->getVariables();
		std::unordered_set<Variable> expectedAbstractions = {
			"x", "a", "b", "c", "z"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Read Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getReadStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"3", "8", "12", "17", "22"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Print Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getPrintStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"4", "9", "13", "19", "23"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Assign Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getAssignStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"5", "10", "14", "20", "24", "26"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("If Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getIfStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"1", "2", "6", "16", "18"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("While Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getWhileStmts();
		std::unordered_set<Statement> expectedAbstractions = {};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Call Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getCallStmts();
		std::unordered_set<Statement> expectedAbstractions = {
			"7", "11", "15", "21", "25"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("All Statements") {
		std::unordered_set<Statement> actualAbstractions = getter->getStatements();
		std::unordered_set<Statement> expectedAbstractions = {
			"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14",
			"15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26"
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Follows") {
		std::unordered_map<Statement, Statement> actualAbstractions = getter->getFollows();
		std::unordered_map<Statement, Statement> expectedAbstractions = {
			// Follows 2-12-13
			{"2", "12"}, {"12", "13"},
			// Follows 3-4
			{"3", "4"},
			// Follows 5-6-11
			{"5", "6"}, {"6", "11"},
			// Follows 7-8
			{"7", "8"},
			// Follows 9-10
			{"9", "10"},
			// Follows 14-15-16
			{"14", "15"}, {"15", "16"},
			// Follows 17-18-23
			{"17", "18"}, {"18", "23"},
			// Follows 19-20
			{"19", "20"},
			// Follows 21-22
			{"21", "22"},
			// Follows 24-25
			{"24", "25"},
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("FollowsT") {
		std::unordered_map<Statement, std::unordered_set<Statement>> actualAbstractions = getter->getFollowsT();
		std::unordered_map<Statement, std::unordered_set<Statement>> expectedAbstractions = {
		    	{"0", {}},
		    	{"1", {}},
			// Follows 2-12-13
			{"2", {"12", "13"}},
			// Follows 3-4
			{"3", {"4"}},
			{"4", {}},
			// Follows 5-6-11
			{"5", {"6", "11"}},
			{"6", {"11"}},
			// Follows 7-8
			{"7", {"8"}},
			{"8", {}},
		    // Follows 9-10
			{"9", {"10"}},
			{"10", {}},
			{"11", {}},
			{"12", {"13"}},
			{"13", {}},
		    	// Follows 14-15-16
			{"14", {"15", "16"}},
			{"15", {"16"}},
			{"16", {}},
			// Follows 17-18-23
			{"17", {"18", "23"}},
			{"18", {"23"}},
			// Follows 19-20
			{"19", {"20"}},
			{"20", {}},
		    	// Follows 21-22
			{"21", {"22"}},
			{"22", {}},
		    	// Follows 24-25
			{"23", {}},
			{"24", {"25"}},
			{"25", {}},
			{"26", {}},
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("Parent") {
   std::unordered_map<Statement, std::unordered_set<Statement>> actualAbstractions = getter->getParent();
   std::unordered_map<Statement, std::unordered_set<Statement>> expectedAbstractions = {
			// Parent 1

			{"1", {"2", "12", "13", "14", "15", "16"}},
			// Parent 2
			{"2", {"3", "4", "5", "6", "11"}},
			{"0", {}},
			{"3", {}},
			{"4", {}},
			{"5", {}},
			// Parent 6
			{"6", {"7", "8", "9", "10"}},
			{"7", {}},
			{"8", {}},
			{"9", {}},
			{"10", {}},
			{"11", {}},
			{"12", {}},
			{"13", {}},
			{"14", {}},
			{"15", {}},
			// Parent 16
			{"16", {"17", "18", "23", "24", "25"}},
			{"17", {}},
			// Parent 18
			{"18", {"19", "20", "21", "22"}},
			{"19", {}},
			{"20", {}},
			{"21", {}},
			{"22", {}},
			{"23", {}},
			{"24", {}},
			{"25", {}},
			{"26", {}},
   };
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("ParentT") {
   std::unordered_map<Statement, std::unordered_set<Statement>> actualAbstractions = getter->getParentT();
   std::unordered_map<Statement, std::unordered_set<Statement>> expectedAbstractions = {
			// Parent 1
			{"1", {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13",
			       "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24",
			       "25"}},
			// Parent 2
			{"2", {"3", "4", "5", "6", "7", "8", "9", "10", "11"}},
			// Parent 6
			{"0", {}},
			{"3", {}},
			{"4", {}},
			{"5", {}},
			{"6", {"7", "8", "9", "10"}},
			{"7", {}},
			{"8", {}},
			{"9", {}},
			{"10", {}},
			{"11", {}},
			{"12", {}},
			{"13", {}},
			{"14", {}},
			{"15", {}},
			// Parent 16
			{"16", {"17", "18", "19", "20", "21", "22", "23", "24", "25"}},
			// Parent 18
			{"17", {}},
			{"18", {"19", "20", "21", "22"}},
			{"19", {}},
			{"20", {}},
			{"21", {}},
			{"22", {}},
			{"23", {}},
			{"24", {}},
			{"25", {}},
			{"26", {}},
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("UsesS") {
		std::unordered_map<Statement, std::unordered_set<Variable>> actualAbstractions = getter->getUsesS();
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedAbstractions = {
			// foo
			{"1", {"x", "b"}},
			{"2", {"x", "b"}},
			{"3", {}},
			{"4", {"b"}},
			{"5", {}},
			{"6", {"x", "b"}},
			{"7", {}},
			{"8", {}},
			{"9", {"b"}},
			{"10", {}},
			{"11", {}},
			{"12", {}},
			{"13", {"b"}},
			{"14", {}},
			{"15", {}},
			{"16", {"x", "b"}},
			{"17", {}},
			{"18", {"x", "b"}},
			{"19", {"b"}},
			{"20", {}},
			{"21", {}},
			{"22", {}},
			{"23", {"b"}},
			{"24", {}},
			{"25", {}},
			// bar
			{"26", {}}
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("UsesP") {
		std::unordered_map<Procedure, std::unordered_set<Variable>> actualAbstractions = getter->getUsesP();
		std::unordered_map<Procedure, std::unordered_set<Variable>> expectedAbstractions = {
			{"foo", {"x", "b"}},
			{"bar", {}},
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("ModifiesS") {
		std::unordered_map<Statement, std::unordered_set<Variable>> actualAbstractions = getter->getModifiesS();
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedAbstractions = {
			// foo
			{"1", {"a", "c", "z"}},
			{"2", {"a", "c", "z"}},
			{"3", {"a"}},
			{"5", {"c"}},
			{"6", {"a", "c", "z"}},
			{"7", {"z"}},
			{"8", {"a"}},
			{"10", {"c"}},
			{"11", {"z"}},
			{"12", {"a"}},
			{"14", {"c"}},
			{"15", {"z"}},
			{"16", {"a", "c", "z"}},
			{"17", {"a"}},
			{"18", {"a", "c", "z"}},
			{"20", {"c"}},
			{"21", {"z"}},
			{"22", {"a"}},
			{"24", {"c"}},
			{"25", {"z"}},
			// bar
			{"26", {"z"}}
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	SECTION("ModifiesP") {
		std::unordered_map<Statement, std::unordered_set<Variable>> actualAbstractions = getter->getModifiesP();
		std::unordered_map<Statement, std::unordered_set<Variable>> expectedAbstractions = {
			{"foo", {"a", "c", "z"}},
			{"bar", {"z"}},
		};
		REQUIRE(actualAbstractions == expectedAbstractions);
	}

	/* =========================================== Next =========================================== */

	SECTION("Positive: isNext(numeric_ref, numeric_ref)") {
		// foo
		// if1
		REQUIRE(getter->isNext("1", "2"));
		// if2
		REQUIRE(getter->isNext("2", "3"));
		REQUIRE(getter->isNext("3", "4"));
		REQUIRE(getter->isNext("4", "12"));
		REQUIRE(getter->isNext("2", "5"));
		REQUIRE(getter->isNext("5", "6"));
		// start of if3
		REQUIRE(getter->isNext("6", "7"));
		REQUIRE(getter->isNext("7", "8"));
		REQUIRE(getter->isNext("8", "11"));
		REQUIRE(getter->isNext("6", "9"));
		REQUIRE(getter->isNext("9", "10"));
		REQUIRE(getter->isNext("10", "11"));
		// end of if3
		REQUIRE_FALSE(getter->isNext("6", "11"));
		REQUIRE(getter->isNext("11", "12"));
		// end of if2
		REQUIRE(getter->isNext("12", "13"));
		REQUIRE(getter->isNext("1", "14"));
		REQUIRE(getter->isNext("14", "15"));
		REQUIRE(getter->isNext("15", "16"));
		// start of if4
		REQUIRE(getter->isNext("16", "17"));
		REQUIRE(getter->isNext("17", "18"));
		// start of if5
		REQUIRE(getter->isNext("18", "19"));
		REQUIRE(getter->isNext("19", "20"));
		REQUIRE(getter->isNext("20", "23"));
		REQUIRE(getter->isNext("18", "21"));
		REQUIRE(getter->isNext("21", "22"));
		REQUIRE(getter->isNext("22", "23"));
		// end of if5
		REQUIRE(getter->isNext("16", "24"));
		REQUIRE(getter->isNext("24", "25"));
		// end of if4
		// end of if1

		// bar (none)
	}

	SECTION("Negative: isNext(numeric_ref, numeric_ref") {
		// Test if !isNext(<consecutive statements on the same nesting level>)
		REQUIRE_FALSE(getter->isNext("4", "3"));
		REQUIRE_FALSE(getter->isNext("8", "7"));
		REQUIRE_FALSE(getter->isNext("10", "9"));
		REQUIRE_FALSE(getter->isNext("12", "2"));
		REQUIRE_FALSE(getter->isNext("13", "12"));
		REQUIRE_FALSE(getter->isNext("15", "14"));
		REQUIRE_FALSE(getter->isNext("16", "15"));
		REQUIRE_FALSE(getter->isNext("20", "19"));
		REQUIRE_FALSE(getter->isNext("22", "21"));
		REQUIRE_FALSE(getter->isNext("25", "24"));

		// Test for !isNext(if statement, next statement in the same nesting level)
		REQUIRE_FALSE(getter->isNext("2", "12"));
		REQUIRE_FALSE(getter->isNext("6", "11"));
		REQUIRE_FALSE(getter->isNext("18", "23"));

		// Test for !isNext(then statement, else statement) and vice versa
		REQUIRE_FALSE(getter->isNext("2", "14"));
		REQUIRE_FALSE(getter->isNext("14", "2"));

		REQUIRE_FALSE(getter->isNext("3", "5"));
		REQUIRE_FALSE(getter->isNext("5", "3"));

		REQUIRE_FALSE(getter->isNext("7", "9"));
		REQUIRE_FALSE(getter->isNext("9", "7"));

		REQUIRE_FALSE(getter->isNext("17", "24"));
		REQUIRE_FALSE(getter->isNext("24", "17"));

		REQUIRE_FALSE(getter->isNext("19", "21"));
		REQUIRE_FALSE(getter->isNext("21", "19"));
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
		REQUIRE(getter->isNext("14", "_"));
		REQUIRE(getter->isNext("15", "_"));
		REQUIRE(getter->isNext("16", "_"));
		REQUIRE(getter->isNext("17", "_"));
		REQUIRE(getter->isNext("18", "_"));
		REQUIRE(getter->isNext("19", "_"));
		REQUIRE(getter->isNext("20", "_"));
		REQUIRE(getter->isNext("21", "_"));
		REQUIRE(getter->isNext("22", "_"));
		REQUIRE(getter->isNext("24", "_"));
	}

	SECTION("Negative: isNext(numeric_ref, wildcard)") {
		REQUIRE_FALSE(getter->isNext("13", "_"));
		REQUIRE_FALSE(getter->isNext("23", "_"));
		REQUIRE_FALSE(getter->isNext("25", "_"));
		REQUIRE_FALSE(getter->isNext("26", "_"));
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
		REQUIRE(getter->isNext("_", "15"));
		REQUIRE(getter->isNext("_", "16"));
		REQUIRE(getter->isNext("_", "17"));
		REQUIRE(getter->isNext("_", "18"));
		REQUIRE(getter->isNext("_", "19"));
		REQUIRE(getter->isNext("_", "20"));
		REQUIRE(getter->isNext("_", "21"));
		REQUIRE(getter->isNext("_", "22"));
		REQUIRE(getter->isNext("_", "23"));
		REQUIRE(getter->isNext("_", "24"));
		REQUIRE(getter->isNext("_", "25"));
	}

	SECTION("Negative: isNext(wildcard, numeric_ref)") {
		REQUIRE_FALSE(getter->isNext("_", "1"));
		REQUIRE_FALSE(getter->isNext("_", "26"));
	}

	SECTION("isNext(numeric_ref, wildcard)") {
		REQUIRE(getter->isNext("_", "_"));
	}

	/* =========================================== NextT =========================================== */

	SECTION("Positive: isNextT(numeric_ref, numeric_ref)") {
		REQUIRE(getter->isNextT("1", "2"));
		REQUIRE(getter->isNextT("1", "3"));
		REQUIRE(getter->isNextT("1", "4"));
		REQUIRE(getter->isNextT("1", "5"));
		REQUIRE(getter->isNextT("1", "6"));
		REQUIRE(getter->isNextT("1", "7"));
		REQUIRE(getter->isNextT("1", "8"));
		REQUIRE(getter->isNextT("1", "9"));
		REQUIRE(getter->isNextT("1", "10"));
		REQUIRE(getter->isNextT("1", "11"));
		REQUIRE(getter->isNextT("1", "12"));
		REQUIRE(getter->isNextT("1", "13"));
		REQUIRE(getter->isNextT("1", "14"));
		REQUIRE(getter->isNextT("1", "15"));
		REQUIRE(getter->isNextT("1", "16"));
		REQUIRE(getter->isNextT("1", "17"));
		REQUIRE(getter->isNextT("1", "18"));
		REQUIRE(getter->isNextT("1", "19"));
		REQUIRE(getter->isNextT("1", "20"));
		REQUIRE(getter->isNextT("1", "21"));
		REQUIRE(getter->isNextT("1", "22"));
		REQUIRE(getter->isNextT("1", "23"));
		REQUIRE(getter->isNextT("1", "24"));
		REQUIRE(getter->isNextT("1", "25"));

		REQUIRE(getter->isNextT("2", "3"));
		REQUIRE(getter->isNextT("2", "4"));
		REQUIRE(getter->isNextT("2", "5"));
		REQUIRE(getter->isNextT("2", "6"));
		REQUIRE(getter->isNextT("2", "7"));
		REQUIRE(getter->isNextT("2", "8"));
		REQUIRE(getter->isNextT("2", "9"));
		REQUIRE(getter->isNextT("2", "10"));
		REQUIRE(getter->isNextT("2", "11"));
		REQUIRE(getter->isNextT("2", "12"));
		REQUIRE(getter->isNextT("2", "13"));

		REQUIRE(getter->isNextT("3", "4"));
		REQUIRE(getter->isNextT("3", "12"));
		REQUIRE(getter->isNextT("3", "13"));

		REQUIRE(getter->isNextT("4", "12"));
		REQUIRE(getter->isNextT("4", "13"));

		REQUIRE(getter->isNextT("5", "6"));
		REQUIRE(getter->isNextT("5", "7"));
		REQUIRE(getter->isNextT("5", "8"));
		REQUIRE(getter->isNextT("5", "9"));
		REQUIRE(getter->isNextT("5", "10"));
		REQUIRE(getter->isNextT("5", "11"));
		REQUIRE(getter->isNextT("5", "12"));
		REQUIRE(getter->isNextT("5", "13"));

		REQUIRE(getter->isNextT("6", "7"));
		REQUIRE(getter->isNextT("6", "8"));
		REQUIRE(getter->isNextT("6", "9"));
		REQUIRE(getter->isNextT("6", "10"));
		REQUIRE(getter->isNextT("6", "11"));
		REQUIRE(getter->isNextT("6", "12"));
		REQUIRE(getter->isNextT("6", "13"));

		REQUIRE(getter->isNextT("7", "8"));
		REQUIRE(getter->isNextT("7", "11"));
		REQUIRE(getter->isNextT("7", "12"));
		REQUIRE(getter->isNextT("7", "13"));

		REQUIRE(getter->isNextT("8", "11"));
		REQUIRE(getter->isNextT("8", "12"));
		REQUIRE(getter->isNextT("8", "13"));

		REQUIRE(getter->isNextT("9", "10"));
		REQUIRE(getter->isNextT("9", "11"));
		REQUIRE(getter->isNextT("9", "12"));
		REQUIRE(getter->isNextT("9", "13"));

		REQUIRE(getter->isNextT("10", "11"));
		REQUIRE(getter->isNextT("10", "12"));
		REQUIRE(getter->isNextT("10", "13"));

		REQUIRE(getter->isNextT("11", "12"));
		REQUIRE(getter->isNextT("11", "13"));

		REQUIRE(getter->isNextT("12", "13"));

		REQUIRE(getter->isNextT("14", "15"));
		REQUIRE(getter->isNextT("14", "16"));
		REQUIRE(getter->isNextT("14", "17"));
		REQUIRE(getter->isNextT("14", "18"));
		REQUIRE(getter->isNextT("14", "19"));
		REQUIRE(getter->isNextT("14", "20"));
		REQUIRE(getter->isNextT("14", "21"));
		REQUIRE(getter->isNextT("14", "22"));
		REQUIRE(getter->isNextT("14", "23"));
		REQUIRE(getter->isNextT("14", "24"));
		REQUIRE(getter->isNextT("14", "25"));

		REQUIRE(getter->isNextT("15", "16"));
		REQUIRE(getter->isNextT("15", "17"));
		REQUIRE(getter->isNextT("15", "18"));
		REQUIRE(getter->isNextT("15", "19"));
		REQUIRE(getter->isNextT("15", "20"));
		REQUIRE(getter->isNextT("15", "21"));
		REQUIRE(getter->isNextT("15", "22"));
		REQUIRE(getter->isNextT("15", "23"));
		REQUIRE(getter->isNextT("15", "24"));
		REQUIRE(getter->isNextT("15", "25"));

		REQUIRE(getter->isNextT("16", "17"));
		REQUIRE(getter->isNextT("16", "18"));
		REQUIRE(getter->isNextT("16", "19"));
		REQUIRE(getter->isNextT("16", "20"));
		REQUIRE(getter->isNextT("16", "21"));
		REQUIRE(getter->isNextT("16", "22"));
		REQUIRE(getter->isNextT("16", "23"));
		REQUIRE(getter->isNextT("16", "24"));
		REQUIRE(getter->isNextT("16", "25"));

		REQUIRE(getter->isNextT("17", "18"));
		REQUIRE(getter->isNextT("17", "19"));
		REQUIRE(getter->isNextT("17", "20"));
		REQUIRE(getter->isNextT("17", "21"));
		REQUIRE(getter->isNextT("17", "22"));
		REQUIRE(getter->isNextT("17", "23"));

		REQUIRE(getter->isNextT("18", "19"));
		REQUIRE(getter->isNextT("18", "20"));
		REQUIRE(getter->isNextT("18", "21"));
		REQUIRE(getter->isNextT("18", "22"));
		REQUIRE(getter->isNextT("18", "23"));

		REQUIRE(getter->isNextT("19", "20"));
		REQUIRE(getter->isNextT("19", "23"));

		REQUIRE(getter->isNextT("20", "23"));

		REQUIRE(getter->isNextT("21", "22"));
		REQUIRE(getter->isNextT("21", "23"));

		REQUIRE(getter->isNextT("22", "23"));

		REQUIRE(getter->isNextT("24", "25"));
	}

	SECTION("Negative: isNextT(numeric_ref, numeric_ref") {
		REQUIRE_FALSE(getter->isNextT("13", "14"));
		REQUIRE_FALSE(getter->isNextT("23", "24"));
	}

	SECTION("Positive: isNextT(numeric_ref, wildcard)") {
		REQUIRE(getter->isNextT("1", "_"));
		REQUIRE(getter->isNextT("2", "_"));
		REQUIRE(getter->isNextT("3", "_"));
		REQUIRE(getter->isNextT("4", "_"));
		REQUIRE(getter->isNextT("5", "_"));
		REQUIRE(getter->isNextT("6", "_"));
		REQUIRE(getter->isNextT("7", "_"));
		REQUIRE(getter->isNextT("8", "_"));
		REQUIRE(getter->isNextT("9", "_"));
		REQUIRE(getter->isNextT("10", "_"));
		REQUIRE(getter->isNextT("11", "_"));
		REQUIRE(getter->isNextT("12", "_"));
		REQUIRE(getter->isNextT("14", "_"));
		REQUIRE(getter->isNextT("15", "_"));
		REQUIRE(getter->isNextT("16", "_"));
		REQUIRE(getter->isNextT("17", "_"));
		REQUIRE(getter->isNextT("18", "_"));
		REQUIRE(getter->isNextT("19", "_"));
		REQUIRE(getter->isNextT("20", "_"));
		REQUIRE(getter->isNextT("21", "_"));
		REQUIRE(getter->isNextT("22", "_"));
		REQUIRE(getter->isNextT("24", "_"));
	}

	SECTION("Negative: isNextT(numeric_ref, wildcard)") {
		REQUIRE_FALSE(getter->isNextT("13", "_"));
		REQUIRE_FALSE(getter->isNextT("23", "_"));
		REQUIRE_FALSE(getter->isNextT("25", "_"));
		REQUIRE_FALSE(getter->isNextT("26", "_"));
	}

	SECTION("Positive: isNextT(wildcard, numeric_ref)") {
		REQUIRE(getter->isNextT("_", "2"));
		REQUIRE(getter->isNextT("_", "3"));
		REQUIRE(getter->isNextT("_", "4"));
		REQUIRE(getter->isNextT("_", "5"));
		REQUIRE(getter->isNextT("_", "6"));
		REQUIRE(getter->isNextT("_", "7"));
		REQUIRE(getter->isNextT("_", "8"));
		REQUIRE(getter->isNextT("_", "9"));
		REQUIRE(getter->isNextT("_", "10"));
		REQUIRE(getter->isNextT("_", "11"));
		REQUIRE(getter->isNextT("_", "12"));
		REQUIRE(getter->isNextT("_", "13"));
		REQUIRE(getter->isNextT("_", "14"));
		REQUIRE(getter->isNextT("_", "15"));
		REQUIRE(getter->isNextT("_", "16"));
		REQUIRE(getter->isNextT("_", "17"));
		REQUIRE(getter->isNextT("_", "18"));
		REQUIRE(getter->isNextT("_", "19"));
		REQUIRE(getter->isNextT("_", "20"));
		REQUIRE(getter->isNextT("_", "21"));
		REQUIRE(getter->isNextT("_", "22"));
		REQUIRE(getter->isNextT("_", "23"));
		REQUIRE(getter->isNextT("_", "24"));
		REQUIRE(getter->isNextT("_", "25"));
	}

	SECTION("Negative: isNextT(wildcard, numeric_ref)") {
		REQUIRE_FALSE(getter->isNextT("_", "1"));
		REQUIRE_FALSE(getter->isNextT("_", "26"));
	}

	SECTION("isNextT(numeric_ref, wildcard)") {
		REQUIRE(getter->isNextT("_", "_"));
	}
}

TEST_CASE("Nested If Statements - With Assign Statements") {
	std::string sourceCode =
		"procedure foo {"
		"  a = 0;" // 1
		"  b = 5;" // 2
		"  if (x == 10) then {" // 3
		"    if (x == 20) then {" // 4
		"      a = a + b;" // 5
		"      b = b - 1;" // 6
		"    } else {"
		"      c = 30;" // 7
		"      if (x == 40) then {" // 8
		"        call bar;" // 9
		"        a = a * c;" // 10
		"      } else {"
		"        b = a - b * c;" // 11
		"        c = 50;" // 12
		"      }"
		"      call bar;" // 13
		"    }"
		"    a = 60;" // 14
		"    b = a;" // 15
		"  } else {"
		"    c = 60;" // 16
		"    call bar;" // 17
		"    if ((x >= 70) && (x <= 80)) then {" // 18
		"      a = a * b + c;" // 19
		"      if (x == 90) then {" // 20
		"        b = b + c;" // 21
		"        c = 100;" // 22
		"      } else {"
		"        call bar;" // 23
		"        a = a - 1;" // 24
		"      }"
		"      b = b * 2;" // 25
		"    } else {"
		"      c = 110;" // 26
		"      call bar;" // 27
		"    }"
		"  }"
		"}"
		"procedure bar {"
		"  a = a * 2;" // 28
		"}";

	PKB pkb = PKB();
	std::shared_ptr<PKBSetter> setter = std::make_shared<PKBSetter>(PKBSetter(pkb));
	std::shared_ptr<PKBGetter> getter = std::make_shared<PKBGetter>(PKBGetter(pkb));
	SourceProcessor SP = SourceProcessor(setter);
	SP.processCode(sourceCode);
	setter->done();

	/* ============================== Affects ============================== */

	SECTION("Positive: Affects(numeric_ref, numeric_ref)") {
		// 1 a
		REQUIRE(getter->isAffects("1", "5"));
		REQUIRE(getter->isAffects("1", "11"));

		// 2 b
		REQUIRE(getter->isAffects("2", "6"));
		REQUIRE(getter->isAffects("2", "11"));
		REQUIRE(getter->isAffects("2", "19"));
		REQUIRE(getter->isAffects("2", "21"));
		REQUIRE(getter->isAffects("2", "25"));

		// 7 c
		REQUIRE(getter->isAffects("7", "10"));
		REQUIRE(getter->isAffects("7", "11"));

		// 16 c
		REQUIRE(getter->isAffects("16", "19"));
		REQUIRE(getter->isAffects("16", "21"));

		// 21 b
		REQUIRE(getter->isAffects("21", "25"));
	}

	SECTION("Negative: isAffects(numeric_ref, numeric_ref)") {
		// not assign stmt
		REQUIRE_FALSE(getter->isAffects("1", "3"));
		REQUIRE_FALSE(getter->isAffects("1", "4"));
		REQUIRE_FALSE(getter->isAffects("19", "20"));

		REQUIRE_FALSE(getter->isAffects("1", "9"));
		REQUIRE_FALSE(getter->isAffects("1", "13"));
		REQUIRE_FALSE(getter->isAffects("10", "13"));
		REQUIRE_FALSE(getter->isAffects("1", "17"));
		REQUIRE_FALSE(getter->isAffects("19", "23"));
		REQUIRE_FALSE(getter->isAffects("1", "27"));

		// different proc
		REQUIRE_FALSE(getter->isAffects("1", "28"));
		REQUIRE_FALSE(getter->isAffects("10", "28"));
		REQUIRE_FALSE(getter->isAffects("19", "28"));

		// no path
		REQUIRE_FALSE(getter->isAffects("24", "19"));
		REQUIRE_FALSE(getter->isAffects("24", "15"));
		REQUIRE_FALSE(getter->isAffects("24", "11"));
		REQUIRE_FALSE(getter->isAffects("24", "10"));
		REQUIRE_FALSE(getter->isAffects("24", "5"));
		REQUIRE_FALSE(getter->isAffects("5", "10"));
		REQUIRE_FALSE(getter->isAffects("5", "19"));

		// var modified in execution path
		REQUIRE_FALSE(getter->isAffects("1", "10"));
		REQUIRE_FALSE(getter->isAffects("1", "14"));
		REQUIRE_FALSE(getter->isAffects("1", "15"));
		REQUIRE_FALSE(getter->isAffects("1", "19"));
		REQUIRE_FALSE(getter->isAffects("1", "24"));
		REQUIRE_FALSE(getter->isAffects("2", "15"));
		REQUIRE_FALSE(getter->isAffects("5", "14"));
		REQUIRE_FALSE(getter->isAffects("10", "14"));
		REQUIRE_FALSE(getter->isAffects("19", "24"));

		// var not used
		REQUIRE_FALSE(getter->isAffects("5", "6"));
	}

	SECTION("Positive: isAffects(numeric_ref, wildcard)") {
		REQUIRE(getter->isAffects("1", "_"));
		REQUIRE(getter->isAffects("2", "_"));
	}

	SECTION("Negative: isAffects(numeric_ref, wildcard)") {
		// not assign stmt
		REQUIRE_FALSE(getter->isAffects("3", "_"));
		REQUIRE_FALSE(getter->isAffects("4", "_"));
		REQUIRE_FALSE(getter->isAffects("8", "_"));
		REQUIRE_FALSE(getter->isAffects("18", "_"));
		REQUIRE_FALSE(getter->isAffects("20", "_"));
		REQUIRE_FALSE(getter->isAffects("9", "_"));
		REQUIRE_FALSE(getter->isAffects("13", "_"));
		REQUIRE_FALSE(getter->isAffects("17", "_"));
		REQUIRE_FALSE(getter->isAffects("23", "_"));
		REQUIRE_FALSE(getter->isAffects("27", "_"));

		// var not used in any RHS assignment stmt
		REQUIRE_FALSE(getter->isAffects("12", "_"));
		REQUIRE_FALSE(getter->isAffects("15", "_"));
		REQUIRE_FALSE(getter->isAffects("22", "_"));
		//REQUIRE_FALSE(getter->isAffects("24", "_"));
		REQUIRE_FALSE(getter->isAffects("25", "_"));
		REQUIRE_FALSE(getter->isAffects("26", "_"));
		REQUIRE_FALSE(getter->isAffects("28", "_"));
	}

	SECTION("Positive: isAffects(wildcard, numeric_ref)") {
		REQUIRE(getter->isAffects("_", "5"));
		REQUIRE(getter->isAffects("_", "6"));
		REQUIRE(getter->isAffects("_", "10"));
		REQUIRE(getter->isAffects("_", "11"));
		REQUIRE(getter->isAffects("_", "19"));
		REQUIRE(getter->isAffects("_", "21"));
		REQUIRE(getter->isAffects("_", "25"));
	}

	SECTION("Negative: isAffects(wildcard, numeric_ref)") {
		// not assign stmt
		REQUIRE_FALSE(getter->isAffects("_", "3"));
		REQUIRE_FALSE(getter->isAffects("_", "4"));
		REQUIRE_FALSE(getter->isAffects("_", "8"));
		REQUIRE_FALSE(getter->isAffects("_", "18"));
		REQUIRE_FALSE(getter->isAffects("_", "20"));
		REQUIRE_FALSE(getter->isAffects("_", "9"));
		REQUIRE_FALSE(getter->isAffects("_", "13"));
		REQUIRE_FALSE(getter->isAffects("_", "17"));
		REQUIRE_FALSE(getter->isAffects("_", "23"));
		REQUIRE_FALSE(getter->isAffects("_", "27"));

		// RHS stmt is first assignment of var
		REQUIRE_FALSE(getter->isAffects("_", "1"));
		REQUIRE_FALSE(getter->isAffects("_", "2"));
		REQUIRE_FALSE(getter->isAffects("_", "7"));
		REQUIRE_FALSE(getter->isAffects("_", "16"));
	}

	SECTION("isAffects(wildcard, wildcard)") {
		REQUIRE(getter->isAffects("_", "_"));
	}
}