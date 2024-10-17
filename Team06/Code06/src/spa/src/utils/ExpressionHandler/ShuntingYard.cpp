#include "utils/ExpressionHandler/ShuntingYard.h"


// Convert infix expression to postfix expression.
std::shared_ptr<TokenStream> ShuntingYard::convertToPostfix(std::shared_ptr<TokenStream> tokens) {

    std::shared_ptr<TokenStream> _outputQueue = std::make_shared<TokenStream>();
    std::stack<std::shared_ptr<ParseableToken>> _operatorsStack = std::stack<std::shared_ptr<ParseableToken>>();

    for (std::shared_ptr<ParseableToken> token: tokens->toParseableTokens()) {
        
        // (1) if it's a number, add it to output
        if (token->isInteger()) { _outputQueue->addToken(token); }

        // (1.5) if it's a variable, add it to output
        if (token->isName()) { _outputQueue->addToken(token); }

        // (2) if it's an operator,
        if (token->isOperatorToken()) {

            // while there is an operator at the top of the operator stack with greater precedence
            while (!_operatorsStack.empty()
                   && _operatorsStack.top()->getOperatorPrecedence() >= token->getOperatorPrecedence()) {

                // pop operators from the operator stack, onto the output queue
                _outputQueue->addToken(_operatorsStack.top());
                _operatorsStack.pop();
            }

            // push current operator onto the operator stack
            _operatorsStack.push(token);
        }

        // (3) if it's a left bracket, push it onto the stack
        if (token->isLeftRoundBracketToken()) { _operatorsStack.push(token); }

        // (4) if it's a right bracket,
        if (token->isRightRoundBracketToken()) {

            // while the operator at the top of the operator stack is not a left bracket
            while (!_operatorsStack.empty() && !_operatorsStack.top()->isLeftRoundBracketToken()) {

                // pop the operator from the operator stack onto the output queue
                _outputQueue->addToken(_operatorsStack.top());
                _operatorsStack.pop();
            }

            // pop the left bracket from the stack, discard it
            _operatorsStack.pop();
        }
    }

    // while there are still operators on the stack, pop them to the queue
    while (!_operatorsStack.empty()) {
        _outputQueue->addToken(_operatorsStack.top());
        _operatorsStack.pop();
    }
    return _outputQueue;
};
