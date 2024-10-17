#include "catch.hpp"

#include "SP/SourceProcessor.h"
#include "PKB/PKB.h"
#include "PKB/PKBSetter.h"
#include "PKB/PKBGetter.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

TEST_CASE("Nested While stmts, followed by Nested If stmts") {
    std::string sourceCode =
        "procedure nested {"
        "	count = 0;" // 1
        "	x = 0;" // 2
        "	y = 0;" // 3
        "	while ((x != 0) && (y != 0)) {" // 4
        "		x = 1 + x;" // 5
        "		while (x == 0) {" // 6
        "			x = 2 + x;" // 7
        "			y = 3 + y;" // 8
        "			while (y == 0) {" // 9
        "				z = x + y + 1;" // 10
        "			}"
        " 		}"
        " 	}"
        "	i = 1;" // 11
        "	j = 2;" // 12
        "	if ((x != 0) && (y != 0)) then {" // 13
        "		i = 1 + i;" // 14
        "		if (x == 0) then {" // 15
        "			j = 2 + j;" // 16
        "			i = 3;" // 17
        "			if (y == 0) then {" // 18
        "				i = 3;" // 19
        "				j = 5;" // 20
        "				z = i + j + 1;" // 21
        "			} else {"
        "				print a;" // 22
        "			}"
        "		} else {"
        "			print a;" // 23
        "		}"
        "	} else {"
        "		print a;" // 24
        "	}"
        "	x = y + z + i + j;" // 25
        "}";

    PKB pkb = PKB();
    std::shared_ptr<PKBSetter> setter = std::make_shared<PKBSetter>(PKBSetter(pkb));
    std::shared_ptr<PKBGetter> getter = std::make_shared<PKBGetter>(PKBGetter(pkb));
    SourceProcessor SP = SourceProcessor(setter);
    SP.processCode(sourceCode);
    setter->done();

    /* ============================== Affects ============================== */

    SECTION("Positive: Affects(numeric_ref, numeric_ref)") {

        // while loops
        std::unordered_map<std::string, std::unordered_set<std::string>> whileAffects = {
            {"2", {"5"}},
            {"3", {"8", "25"}},
            {"5", {"5", "7"}},
            {"7", {"5", "7", "10"}},
            {"8", {"10", "25"}},
            {"10", {"25"}}
        };

        for (auto const& [key, val] : whileAffects) {
            for (auto const& v : val) {
                REQUIRE(getter->isAffects(key, v));
            }
        }
        
        // if loops        
        std::unordered_map<std::string, std::unordered_set<std::string>> ifAffects = {
            {"11", {"14", "25"}},
            {"12", {"16", "25"}},
            {"14", {"25"}},
            {"16", {"25"}},
            {"17", {"25"}},
            {"19", {"21"}},
            {"20", {"21", "25"}},
            {"21", {"25"}}
        };

        for (auto const& [key, val] : ifAffects) {
            for (auto const& v : val) {
                REQUIRE(getter->isAffects(key, v));
            }
        }
    }

    SECTION("Negative: isAffects(numeric_ref, numeric_ref)") {
        // while stmt
        REQUIRE_FALSE(getter->isAffects("2", "4"));
        REQUIRE_FALSE(getter->isAffects("4", "6"));
        REQUIRE_FALSE(getter->isAffects("6", "9"));

        // if stmt
        REQUIRE_FALSE(getter->isAffects("11", "13"));
        REQUIRE_FALSE(getter->isAffects("13", "15"));
        REQUIRE_FALSE(getter->isAffects("15", "18"));

        // no path
        REQUIRE_FALSE(getter->isAffects("7", "2"));
        REQUIRE_FALSE(getter->isAffects("8", "3"));
        REQUIRE_FALSE(getter->isAffects("17", "14"));
        REQUIRE_FALSE(getter->isAffects("19", "12"));

        // var modified in execution path
        REQUIRE_FALSE(getter->isAffects("14", "21"));
        REQUIRE_FALSE(getter->isAffects("16", "21"));
        REQUIRE_FALSE(getter->isAffects("17", "21"));

        // var not used
        REQUIRE_FALSE(getter->isAffects("2", "25"));
        REQUIRE_FALSE(getter->isAffects("5", "25"));
        REQUIRE_FALSE(getter->isAffects("7", "25"));
        REQUIRE_FALSE(getter->isAffects("14", "17"));
    }

    /* ============================== AffectsT ============================== */

    SECTION("Positive: isAffectsT(numeric_ref, numeric_ref)") {
        // while loops
        std::unordered_map<std::string, std::unordered_set<std::string>> whileAffectsT = {
            {"2", {"5"}},
            {"3", {"8", "25"}},
            {"5", {"5", "7"}},
            {"7", {"5", "7", "10"}},
            {"8", {"10", "25"}},
            {"10", {"25"}}
        };
        for (auto const& [key, val] : whileAffectsT) {
            for (auto const& v : val) {
                REQUIRE(getter->isAffectsT(key, v));
            }
        }

        // if loops
        std::unordered_map<std::string, std::unordered_set<std::string>> ifAffectsT = {
            {"11", {"14", "25"}},
            {"12", {"16", "25"}},
            {"14", {"25"}},
            {"16", {"25"}},
            {"17", {"25"}},
            {"19", {"21"}},
            {"20", {"21", "25"}},
            {"21", {"25"}}
        };
        for (auto const& [key, val] : ifAffectsT) {
            for (auto const& v : val) {
                REQUIRE(getter->isAffectsT(key, v));
            }
        }


        // AffectsT
        std::unordered_map<std::string, std::unordered_set<std::string>> affectsT = {
            {"2", {"7", "10", "25"}},
            {"3", {"10", "25"}},
            {"5", {"10", "25"}},
            {"7", {"25"}},
            {"19", {"25"}}
        };
        for (auto const& [key, val] : affectsT) {
            for (auto const& v : val) {
                REQUIRE(getter->isAffectsT(key, v));
            }
        }
    }

    SECTION("Negative: isAffectsT(numeric_ref, numeric_ref)") {
        std::unordered_map<std::string, std::unordered_set<std::string>> expectedAffectsT = {
            {"2", {"2", "3", "8"}},
            {"3", {"2", "3", "5", "7"}},
            {"5", {"2", "3", "8"}},
            {"7", {"2", "3", "8"}},
            {"8", {"2", "3", "5", "7"}},
            {"11", {"11", "12", "16", "17", "19", "20", "21"}},
            {"12", {"11", "12", "14", "17", "19", "20", "21"}},
            {"14", {"11", "12", "14", "16", "17", "19", "20", "21"}},
            {"16", {"11", "12", "14", "16", "17", "19", "20", "21"}},
            {"17", {"11", "12", "14", "16", "17", "19", "20", "21"}},
            {"19", {"11", "12", "14", "16", "17", "19", "20"}},
            {"20", {"11", "12", "14", "16", "17", "19", "20"}},
        };
        for (auto const& [key, val] : expectedAffectsT) {
            for (auto const& v : val) {
                REQUIRE_FALSE(getter->isAffectsT(key, v));
            }
        }
    }
}


TEST_CASE("Deeply nested container statements") {
    std::string sourceCode =
        "procedure nested {"
        "	count = 0;" // 1
        "	x = 0;" // 2
        "	y = 0;" // 3
        "	while ((x != 0) && (y != 0)) {" // 4
        "		x = 1 + x;" // 5
        "		while (x == 0) {" // 6
        "			x = 2 + x;" // 7
        "			y = 3 + y;" // 8
        "			while (y == 0) {" // 9
        "				z = x + y + 1;" // 10
        "			}"
        " 		}"
        "       if (x == 0) then {" // 11
        "           x = 1 + x;" // 12
        "           while (x == 0) {" // 13
        "               x = 2 + x;" // 14
        "           }"
        "       } else {"
        "           y = 1 + y;" // 15
        "           while (y == 0) {" // 16
        "               y = 2 + y;" // 17
        "               if (y == 0) then {" // 18
        "                   y = 3 + y;" // 19
        "               } else {"
        "                   x = 3 + x;" // 20
        "               }"
        "           }"
        "           x = 1 + x;" // 21
        "       }"
        "   }"
        "	z = x + y + 1;" // 22
        "}";

    PKB pkb = PKB();
    std::shared_ptr<PKBSetter> setter = std::make_shared<PKBSetter>(PKBSetter(pkb));
    std::shared_ptr<PKBGetter> getter = std::make_shared<PKBGetter>(PKBGetter(pkb));
    SourceProcessor SP = SourceProcessor(setter);
    SP.processCode(sourceCode);
    setter->done();

    SECTION("Positive: isNext(numeric_ref, numeric_ref)") {
        std::unordered_map<std::string, std::unordered_set<std::string>> expectedNext = {
            {"1", {"2"}},
            {"2", {"3"}},
            {"3", {"4"}},
            {"4", {"5", "22"}},
            {"5", {"6"}},
            {"6", {"7", "11"}},
            {"7", {"8"}},
            {"8", {"9"}},
            {"9", {"6", "10"}},
            {"10", {"9"}},
            {"11", {"12"}},
            {"12", {"13"}},
            {"13", {"4", "14"}},
            {"14", {"13"}},
            {"15", {"16"}},
            {"16", {"17", "21"}},
            {"17", {"18"}},
            {"18", {"19", "20"}},
            {"19", {"16"}},
            {"20", {"16"}},
            {"21", {"4"}},
        };
        for (auto const& [key, val] : expectedNext) {
            for (auto const& v : val) {
                REQUIRE(getter->isNext(key, v));
            }
        }
    }

    SECTION("Positive: isNextT(numeric_ref, numeric_ref)") {
        std::unordered_map<std::string, std::unordered_set<std::string>> expectedNextT = {
            {"1", {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"2", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"3", {"4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"4", {"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"5", {"4", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"6", {"4", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"7", {"4", "6", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"8", {"4", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"9", {"4", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"10", {"4", "9", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"11", {"4", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"12", {"4", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"13", {"4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"14", {"4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
            {"15", {"16", "17", "18", "19", "20", "21", "22"}},
            {"16", {"17", "18", "19", "20", "21", "22"}},
            {"17", {"16", "17", "18", "19", "20", "21", "22"}},
            {"18", {"16", "17", "18", "19", "20", "21", "22"}},
            {"19", {"16", "17", "18", "19", "20", "21", "22"}},
            {"20", {"16", "17", "18", "19", "20", "21", "22"}},
            {"21", {"4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22"}},
        };

        for (auto const& [key, val] : expectedNextT) {
            for (auto const& v : val) {
                REQUIRE(getter->isNextT(key, v));
            }
        }
    }
}
