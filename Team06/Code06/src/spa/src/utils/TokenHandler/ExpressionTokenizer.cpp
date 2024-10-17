#include "utils/TokenHandler/ExpressionTokenizer.h"


// Constructor.
ExpressionTokenizer::ExpressionTokenizer() {
    _exprLength = 0;
    _cursorPosition = 0;
};

// Move pointer.
void ExpressionTokenizer::_advancePointer(int length) {
    _cursorPosition += length;
    _expression.erase(0, length);
}

// Check if the source code has been fully tokenized.
bool ExpressionTokenizer::_hasMoreTokens() {
    return _cursorPosition < _exprLength;
}

// Tokenize the next item of the source code.
std::shared_ptr<SourceToken> ExpressionTokenizer::_tokenizeNextItem() {
    std::shared_ptr<SourceToken> nextToken = SourceToken::parse(_expression);
    _advancePointer(nextToken->getValue().length());
    return nextToken;
}

// Tokenize the source code.
std::shared_ptr<TokenStream> ExpressionTokenizer::process(std::string expression) {

    _expression = expression;
    _exprLength = expression.length();
    _cursorPosition = 0;

    std::shared_ptr<TokenStream> t = std::make_shared<TokenStream>();

    while (_hasMoreTokens()) {
        std::shared_ptr<SourceToken> nextToken = _tokenizeNextItem();
        if (!nextToken->isWhitespace()) {
            t->addToken(nextToken);
        }
    }

    return t;
}
