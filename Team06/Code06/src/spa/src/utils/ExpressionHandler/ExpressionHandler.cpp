#include "utils/ExpressionHandler/ExpressionHandler.h"


ExpressionHandler::ExpressionHandler(ExpressionType expressionType, std::shared_ptr<TokenStream> parseableTokenExpression) {

    _expressionType = expressionType;
    _tokens = parseableTokenExpression;

}

// If we receive a string, use our ExpressionTokenizer to tokenize it to a vector of ParseableTokens.
ExpressionHandler::ExpressionHandler(ExpressionType expressionType, std::string stringExpression) {

    _expressionType = expressionType;
    _tokens = ExpressionTokenizer().process(stringExpression);
}

ExpressionHandler::ExpressionHandler(ExpressionType expressionType, std::vector<std::string> stringExpression) {

    _expressionType = expressionType;
    std::string s;
    for (const auto &piece : stringExpression) s += piece;
    _tokens = ExpressionTokenizer().process(s);
}

void ExpressionHandler::parse() {

    _validate();
    if (_expressionType == ExpressionType::EXPRESSION) {
        _parsePostfix();
    }
    _extractInfo();
}

void ExpressionHandler::_validate() {

    if (_expressionType == ExpressionType::EXPRESSION) {
        Validator(_tokens, ValidatedExpressionType::EXPRESSION).validate();
    }
    
    if (_expressionType == ExpressionType::CONDITIONAL_EXPRESSION) {
        Validator(_tokens, ValidatedExpressionType::CONDITIONAL_EXPRESSION).validate();
    }
    
}

// Converts the expression into postfix notation.
void ExpressionHandler::_parsePostfix() {

    // Convert infix expression to postfix expression.
    std::shared_ptr<TokenStream> _postfixTokens = ShuntingYard::convertToPostfix(_tokens);

    // Get postfix notation.
    while(_postfixTokens->hasNextToken()) {
        _postfixNotation.push_back(_postfixTokens->getNextTokenValue());
    }

}


// This method does many things:
// 1) It stores a copy of infix notation.
// 2) It obtains the variables used in the expression.
// 3) It obtains the constants used in the expression.
void ExpressionHandler::_extractInfo() {

    for (std::shared_ptr<ParseableToken> token: _tokens->toParseableTokens()) {
        _infixNotation.push_back(token->getValue());
        if (token->isName()) {
            _varsUsed.insert(token->getValue());
        }
        if (token->isInteger()) {
            _constantsUsed.insert(token->getValue());
        }
    }
}

// Getters.
std::vector<std::string> ExpressionHandler::getPostfixNotation() {
    return _postfixNotation;
}

std::vector<std::string> ExpressionHandler::getInfixNotation() {
    return _infixNotation;
}

std::unordered_set<std::string> ExpressionHandler::getVarsUsed() {
    return _varsUsed;
}

std::unordered_set<std::string> ExpressionHandler::getConstantsUsed() {
    return _constantsUsed;
}
