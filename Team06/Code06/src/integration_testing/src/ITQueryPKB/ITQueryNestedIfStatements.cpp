#include "catch.hpp"

#include "PKB/PKB.h"
#include "PKB/PKBGetter.h"
#include "PKB/PKBSetter.h"
#include "QPS/QPS.h"

TEST_CASE("TestNestedIfStatement") {
    /*
    Populate pkb with the source file -
    procedure foo {
                    a = 1; // 1
                    if (b == 2) then { //2
                            c = 3+b;	//3
                            if (d == 4+c) then { //4
                                    e = 5; //5
                            } else {
                                    e = 1; //6
                            }
                    } else {
                        if (a == 5) then { //7
                            e = 5; //8
                        } else {
                            e = 6; //9
                        }
                    }

                    
    }
    */
    PKB pkb = PKB();
    PKBSetter* setter = new PKBSetter(pkb);
    auto getter = std::make_shared<PKBGetter>(pkb);
    QPS qps = QPS(getter);
    std::unordered_set<std::string> stmts({ "1", "2", "3", "4", "5", "6", "7", "8", "9"});
    std::vector<std::string> stmts_vec({ "1", "2", "3", "4", "5", "6", "7", "8", "9"});
    std::unordered_set<std::string> constants({ "1", "2", "3", "4", "5", "6"});
    std::unordered_set<std::string> vars({ "a", "b", "c", "d", "e" });

    setter->addProcedure("foo", stmts);

    setter->addVariable("a");
    setter->addVariable("b");
    setter->addVariable("c");
    setter->addVariable("d");
    setter->addVariable("e");
    setter->addConstant("1");
    setter->addConstant("2");
    setter->addConstant("3");
    setter->addConstant("4");
    setter->addConstant("5");
    setter->addConstant("6");


    setter->addAssignmentStmt("1", "a");
    setter->addIfStmt("2");
    setter->addAssignmentStmt("3", "c");
    setter->addIfStmt("4");
    setter->addAssignmentStmt("5", "e");
    setter->addAssignmentStmt("6", "e");
    setter->addIfStmt("7");
    setter->addAssignmentStmt("8", "e");
    setter->addAssignmentStmt("9", "e");

    setter->addModifiesP("foo", vars);
    setter->addModifiesS("1", "a");
    setter->addModifiesS("2", "c");
    setter->addModifiesS("2", "e");
    setter->addModifiesS("3", "c");
    setter->addModifiesS("4", "e");
    setter->addModifiesS("5", "e");
    setter->addModifiesS("6", "e");
    setter->addModifiesS("7", "e");
    setter->addModifiesS("8", "e");
    setter->addModifiesS("9", "e");


    setter->addUsesS("2", std::unordered_set<std::string>{"b", "c", "d"});
    setter->addUsesIf("2", std::unordered_set<std::string>{"b", "c", "d"});

    setter->addUsesS("3", std::unordered_set<std::string>{"b"});

    setter->addUsesS("4", std::unordered_set<std::string>{"d", "c"});
    setter->addUsesIf("4", std::unordered_set<std::string>{"d","c"});

    setter->addUsesS("7", std::unordered_set<std::string>{"a"});
    setter->addUsesIf("7", std::unordered_set<std::string>{"a"});

    setter->addAssignmentExpr("1", { "1" });
    setter->addAssignmentExpr("3", { "3+b" });
    setter->addAssignmentExpr("5", { "5" });
    setter->addAssignmentExpr("6", { "1" });
    setter->addAssignmentExpr("8", { "5" });
    setter->addAssignmentExpr("9", { "6" });


    setter->addFollowsT(std::vector<std::string>{"1", "2"});
    setter->addFollowsT(std::vector<std::string>{"3", "4"});
    setter->addFollowsT(std::vector<std::string>{"5"});
    setter->addFollowsT(std::vector<std::string>{"6"});
    setter->addFollowsT(std::vector<std::string>{"7"});
    setter->addFollowsT(std::vector<std::string>{"8"});
    setter->addFollowsT(std::vector<std::string>{"9"});


    setter->addParent("2", std::vector<std::string>{"3", "4"});
    setter->addParent("4", std::vector<std::string>{"5"});
    setter->addParent("4", std::vector<std::string>{"6"});
    setter->addParent("7", std::vector<std::string>{"8"});
    setter->addParent("7", std::vector<std::string>{"9"});


    SECTION("Test Modifies(stmt_syn, var_literal)") {
        std::string testQuery = "stmt s;\nSelect s such that Modifies(s,\"c\")";
        std::unordered_set<std::string> expectedResult{ "2", "3" };

        REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
    }

    SECTION("Test Uses(stmt_syn, var_literal)") {
        std::string testQuery = "stmt s;\nSelect s such that Uses(s,\"c\")";
        std::unordered_set<std::string> expectedResult{ "2", "4" };

        REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
    }

    SECTION("Test Parent(stmt_syn, stmt_literal)") {
        std::string testQuery = "stmt s;\nSelect s such that Parent(s,4)";
        std::unordered_set<std::string> expectedResult{ "2" };

        REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
    }
    SECTION("Test Parent(stmt_literal, if_syn)") {
        std::string testQuery = "if w;\nSelect w such that Parent(2,w)";
        std::unordered_set<std::string> expectedResult{ "4" };

        REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
    }

    SECTION("Test Parent*(stmt_literal, if_syn)") {
        std::string testQuery = "if f;\nSelect f such that Parent*(f,5)";
        std::unordered_set<std::string> expectedResult{ "2", "4" };
        std::string testQuery2 = "if f;\nSelect f such that Parent*(f,8)";
        std::unordered_set<std::string> expectedResult2{ "7" };

        REQUIRE(qps.run(testQuery)->getResult() == expectedResult);
        REQUIRE(qps.run(testQuery2)->getResult() == expectedResult2);
    }
}