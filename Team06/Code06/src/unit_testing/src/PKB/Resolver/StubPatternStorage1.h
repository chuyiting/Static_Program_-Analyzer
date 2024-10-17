//
// Created by Chong Jun Wei on 8/10/22.
//

#ifndef SPA_STUBPATTERNSTORAGE1_H
#define SPA_STUBPATTERNSTORAGE1_H

#include "PKB/Datastore/PatternStorage.h"
#include "utils/ExpressionHandler/ExpressionHandler.h"

// assign pattern only
class StubPatternStorage1 : public PatternStorage {
public:
  StubPatternStorage1() : PatternStorage() {
    // Convert the below expressions to infix notation for storage
    // 3 c = s + 3 * 2
    std::string e = "s + 3 * 2";
    ExpressionHandler h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e1 = h.getInfixNotation();

    // 5 s = var + 3 * foo * (s + 3)
    e = "var + 3 * foo * (s + 3)";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e2 = h.getInfixNotation();

    // 7 a = (x+y)
    e = "(x+y)";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e3 = h.getInfixNotation();

    // 8 c = (x+y)*2
    e = "(x+y)*2";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e4 = h.getInfixNotation();

    // 9 x = v + x * y + z * t
    e = "v +       x *     y + z * t            ";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e5 = h.getInfixNotation();

    // 10 m = d
    e = "d";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e6 = h.getInfixNotation();

    // 13 v = 3
    e = "3";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e7 = h.getInfixNotation();

    // 20 f = a1 + 2
    e = "a1 + 2";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e8 = h.getInfixNotation();

    // 35 g = 2003
    e = "2003";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e9 = h.getInfixNotation();

    // 40 x = (x1 + ((((((y)))))))
    e = "(x1 + ((((((y)))))))";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e10 = h.getInfixNotation();

    // 41 x1 = x1 + x
    e = "x1 + n";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e11 = h.getInfixNotation();

    // 50 s = aa2 + a31b + s3k
    e = "aa2 + a31b + s3k";
    h = ExpressionHandler(ExpressionType::EXPRESSION, e);
    h.parse();
    std::vector<std::string> e12 = h.getInfixNotation();

    modifiesS = {{"3", "c"},  {"5", "s"},  {"7", "a"},   {"8", "c"},
                 {"9", "x"},  {"10", "m"}, {"13", "v"},  {"20", "f"},
                 {"35", "g"}, {"40", "x"}, {"41", "x1"}, {"50", "s"}};

    assignmentExpr = {{"3", e1},  {"5", e2},   {"7", e3},   {"8", e4},
                      {"9", e5},  {"10", e6},  {"13", e7},  {"20", e8},
                      {"35", e9}, {"40", e10}, {"41", e11}, {"50", e12}};
  }
};

#endif // SPA_STUBPATTERNSTORAGE1_H
