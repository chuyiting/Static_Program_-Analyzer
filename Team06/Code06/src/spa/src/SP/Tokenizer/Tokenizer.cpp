#include "SP/Tokenizer/Tokenizer.h"

// Constructor.
Tokenizer::Tokenizer() {
    _sourceCode = "";
    _programLength = 0;
    _cursorPosition = 0;
};

// Move pointer.
void Tokenizer::_advancePointer(int length) {
    _cursorPosition += length;
    _sourceCode.erase(0, length);
}

// Tokenize the source code.
std::shared_ptr<TokenStream> Tokenizer::process(std::string sourceCode) {

    _sourceCode = sourceCode;
    _programLength = sourceCode.length();
    _cursorPosition = 0;

    std::shared_ptr<TokenStream> t = std::make_shared<TokenStream>();

    while (hasMoreTokens()) {
        std::shared_ptr<SourceToken> nextToken = tokenizeNextItem();
        if (!nextToken->isWhitespace()) {
            t->addToken(nextToken);
        }
    }

    // Uncomment to print out all tokens
    // t->printAllTokens();

    return t;
}

// Check if the source code has been fully tokenized.
bool Tokenizer::hasMoreTokens() {
    return _cursorPosition < _programLength;
}

// Tokenize the next item of the source code.
std::shared_ptr<SourceToken> Tokenizer::tokenizeNextItem() {
    std::shared_ptr<SourceToken> nextToken = SourceToken::parse(_sourceCode);
    _advancePointer(nextToken->getValue().length());
    return nextToken;
}
