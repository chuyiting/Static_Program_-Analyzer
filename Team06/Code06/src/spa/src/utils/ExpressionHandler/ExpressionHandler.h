#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

#include "utils/ExpressionHandler/ShuntingYard.h"
#include "utils/ExpressionHandler/Validator.h"
#include "utils/TokenHandler/ExpressionTokenizer.h"
#include "utils/TokenHandler/ParseableToken.h"
#include "utils/TokenHandler/TokenStream.h"

enum class ExpressionType {
    EXPRESSION, CONDITIONAL_EXPRESSION
};


// A class that handles the parsing of an expression/conditional expression.
// For conditional expressions, it can retrieve its infix notation without spaces, and extract the variables/constants used in the expression.
// For expressions, it can do the same as above, and also convert the expression to postfix notation.
class ExpressionHandler {

private:

    ExpressionType _expressionType;
    std::vector<std::string> _postfixNotation;
    std::vector<std::string> _infixNotation;
    std::shared_ptr<TokenStream> _tokens;
    std::unordered_set<std::string> _varsUsed = std::unordered_set<std::string>();
    std::unordered_set<std::string> _constantsUsed = std::unordered_set<std::string>();

    void _parsePostfix();
    void _extractInfo();
    void _validate();

public:

    // Two methods of initialising the ExpressionHandler.
    // 1) Pass in a string expression as the second parameter.
    // 2) Pass in a TokenStream as the second parameter.
    // The first parameter is either ExpressionType::EXPRESSION or ExpressionType::CONDITIONAL_EXPRESSION.
    ExpressionHandler(ExpressionType expressionType, std::shared_ptr<TokenStream> parseableTokenExpression);
    ExpressionHandler(ExpressionType expressionType, std::string stringExpression);
    ExpressionHandler(ExpressionType expressionType, std::vector<std::string> stringExpression);

    void parse();

    // Getters.
    std::vector<std::string> getPostfixNotation();

    std::vector<std::string> getInfixNotation();

    std::unordered_set<std::string> getVarsUsed();

    std::unordered_set<std::string> getConstantsUsed();
};
