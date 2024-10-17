#pragma once

#include <memory>

#include "Exceptions/ParserException.h"
#include "utils/TokenHandler/TokenStream.h"

enum class ValidatedExpressionType {
    EXPRESSION, CONDITIONAL_EXPRESSION
};

class Validator {
private:
    // Field variable.
    std::shared_ptr<TokenStream> _tokenStream;
    ValidatedExpressionType _expressionType;

    // Functions.
    void consumeNextToken();
    void consumeComparisonToken();
    void consumeLeftRound();
    void consumeRightRound();
    void checkFailCondition(bool failCondition, ParserException exception);

    // For expressions.
    void validateExpr();
    void validateExprPart();
    void validateTerm();
    void validateTermPart();
    void validateFactor();

    // For conditional expressions.
    void validateCondExpr();
    void validateRelExpr();
    void validateRelFactor();

public:
    // Constructor.
    Validator();
    Validator(std::shared_ptr<TokenStream> tokenStream, ValidatedExpressionType expressionType);

    // Function.
    void validate();
};