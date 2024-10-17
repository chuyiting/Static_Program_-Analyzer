#include "utils/ExpressionHandler/Validator.h"


// Notes for removing left recursion:
// If A is defined as Aa/B, then we can rewrite it as:
// A = BA'
// A' = aA' / null

// ========================================================================

// For expr: we have
// expr: expr '+' term | expr '-' term | term
// term: term '*' factor | term '/' factor | term '%' factor | factor
// factor: var_name | const_value | '(' expr ')'

// We decompose this into:
// expr: term exprPart
// exprPart: '+' term exprPart | '-' term exprPart | null
// term: factor termPart
// termPart: '*' factor termPart | '/' factor termPart | '%' factor termPart | null
// factor: var_name | const_value | '(' expr ')'

// ========================================================================

// For cond_expr: we have
// cond_expr: rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' '&&' '(' cond_expr ')' | '(' cond_expr ')' '||' '(' cond_expr ')'
// rel_expr: rel_factor OP rel_factor, where OP is one of the following: <, <=, >, >=, ==, !=
// rel_factor: var_name | const_value | expr


// Constructor.
Validator::Validator(std::shared_ptr<TokenStream> tokenStream, ValidatedExpressionType expressionType) {
    _tokenStream = tokenStream;
    _expressionType = expressionType;
}

void Validator::validate() {
    if (_expressionType == ValidatedExpressionType::EXPRESSION) {

        validateExpr();
        if (_tokenStream->hasNextToken()) {
            throw ParserException(ParserExceptionType::INVALID_EXPR);
        }

    } 
    
    else if (_expressionType == ValidatedExpressionType::CONDITIONAL_EXPRESSION) {

        validateCondExpr();
        if (_tokenStream->hasNextToken()) {
            throw ParserException(ParserExceptionType::INVALID_COND_EXPR);
        }
    }
}

// expr: term exprPart
void Validator::validateExpr() {
    validateTerm();
    validateExprPart();
}

// exprPart: '+' term exprPart | '-' term exprPart | null
void Validator::validateExprPart() {

    if (!_tokenStream->hasNextToken()) {
        return;
    }

    std::shared_ptr<ParseableToken> token = _tokenStream->peekNextToken();
    if (token->isPlusOrMinusToken()) {
        consumeNextToken();
        validateTerm();
        validateExprPart();
    }
}

// term: factor termPart
void Validator::validateTerm() {
    validateFactor();
    validateTermPart();
}

// termPart: '*' factor termPart | '/' factor termPart | '%' factor termPart | null
void Validator::validateTermPart() {

    if (!_tokenStream->hasNextToken()) {
        return;
    }

    std::shared_ptr<ParseableToken> token = _tokenStream->peekNextToken();
    if (token->isMultiplyDivideOrModuloToken()) {
        consumeNextToken();
        validateFactor();
        validateTermPart();
    }
}

// factor: var_name | const_value | '(' expr ')'
void Validator::validateFactor() {

    if (!_tokenStream->hasNextToken()) {
        throw ParserException(ParserExceptionType::INVALID_FACTOR);
    }
    
    std::shared_ptr<ParseableToken> token = _tokenStream->peekNextToken();
    if (token->isName() || token->isInteger()) {
        consumeNextToken();
    } else if (token->isLeftRoundBracketToken()) {
        consumeLeftRound();
        validateExpr();
        consumeRightRound();
    } else {
        throw ParserException(ParserExceptionType::INVALID_FACTOR);
    }
}

// cond_expr: rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' '&&' '(' cond_expr ')' | '(' cond_expr ')' '||' '(' cond_expr ')'
void Validator::validateCondExpr() {

    // Remember current position.
    _tokenStream->saveIndex();

    try {
        validateRelExpr();  // try to validate rel_expr
    } catch (ParserException &e) {  // if rel_expr is not valid, try to validate the other cases

        // Restore the token stream
        _tokenStream->restoreIndex();

        if (!_tokenStream->hasNextToken()) {
            throw ParserException(ParserExceptionType::INVALID_COND_EXPR);
        }

        std::shared_ptr<ParseableToken> token = _tokenStream->peekNextToken();
        if (token->isNotOperatorToken()) {
            consumeNextToken();
            consumeLeftRound();
            validateCondExpr();
            consumeRightRound();
        } else if (token->isLeftRoundBracketToken()) {
            consumeLeftRound();
            validateCondExpr();
            consumeRightRound();

            if (!_tokenStream->hasNextToken()) {
                throw ParserException(ParserExceptionType::INVALID_COND_EXPR);
            }

            token = _tokenStream->peekNextToken();
            if (token->isAndOperatorToken() || token->isOrOperatorToken()) {
                consumeNextToken();
            } else {
                throw ParserException(ParserExceptionType::INVALID_COND_EXPR);
            }
            
            consumeLeftRound();
            validateCondExpr();
            consumeRightRound();
        } else {
            throw ParserException(ParserExceptionType::INVALID_COND_EXPR);
        }
    }

}

// rel_expr: rel_factor OP rel_factor, where OP is one of the following: <, <=, >, >=, ==, !=
void Validator::validateRelExpr() {
    validateRelFactor();
    consumeComparisonToken();
    validateRelFactor();
}

// rel_factor: var_name | const_value | expr
void Validator::validateRelFactor() {

    // Remember current position.
    _tokenStream->saveIndex();

    try {
        validateExpr();  // try to validate expr
    } catch (ParserException &e) {  // if expr is not valid, try to validate the other cases

        // Restore the token stream
        _tokenStream->restoreIndex();

        if (!_tokenStream->hasNextToken()) {
            throw ParserException(ParserExceptionType::INVALID_REL_FACTOR);
        }

        std::shared_ptr<ParseableToken> token = _tokenStream->peekNextToken();
        if (token->isName() || token->isInteger()) {
            consumeNextToken();
        } else {
            throw ParserException(ParserExceptionType::INVALID_REL_FACTOR);
        }
    }
}

void Validator::consumeNextToken() {
    _tokenStream->getNextToken();
}

void Validator::consumeComparisonToken() {
    std::shared_ptr<ParseableToken> t = _tokenStream->getNextToken();
    checkFailCondition(t == nullptr || !t->isComparisonToken(), ParserException(ParserExceptionType::MISSING_COMPARISON_TOKEN));
}

void Validator::consumeLeftRound() {
    std::shared_ptr<ParseableToken> t = _tokenStream->getNextToken();
    checkFailCondition(t == nullptr || !t->isLeftRoundBracketToken(), ParserException(ParserExceptionType::MISSING_LEFT_ROUND_BRACKET));
}

void Validator::consumeRightRound() {
    std::shared_ptr<ParseableToken> t = _tokenStream->getNextToken();
    checkFailCondition(t == nullptr || !t->isRightRoundBracketToken(),
                       ParserException(ParserExceptionType::MISSING_RIGHT_ROUND_BRACKET));
}

void Validator::checkFailCondition(bool failCondition, ParserException exception) {
    if (failCondition) {
        throw exception;
    }
}
