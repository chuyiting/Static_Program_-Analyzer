#pragma once

#include <memory>
#include <string>

class ParseableToken {

public:

    virtual std::string getValue () const = 0;

    // Basics - define the type of the token.
    virtual bool isInteger() = 0;
    virtual bool isName() = 0;
    virtual bool isWhitespace() = 0;

    // For expressions.
    virtual bool isOperatorToken() = 0;
    virtual bool isPlusOrMinusToken() = 0;
    virtual bool isMultiplyDivideOrModuloToken() = 0;

    // Given two tokens that represent operators,
    // the stronger operator should have a higher value (e.g. * has higher value than +).
    // Feel free to set this as 0 for non-operators.
    virtual int getOperatorPrecedence() = 0;

    // For parsing.
    virtual bool isLeftRoundBracketToken() = 0;
    virtual bool isRightRoundBracketToken() = 0;
    virtual bool isLeftCurlyBracketToken() = 0;
    virtual bool isRightCurlyBracketToken() = 0;
    virtual bool isEndToken() = 0;

    // For conditional expressions.
    virtual bool isComparisonToken() = 0;
    virtual bool isAndOperatorToken() = 0;
    virtual bool isOrOperatorToken() = 0;
    virtual bool isNotOperatorToken() = 0;

    // For debugging.
    virtual void print() = 0;

    // For equality checking.
    virtual bool isEqual(std::shared_ptr<ParseableToken> other) = 0;
};
