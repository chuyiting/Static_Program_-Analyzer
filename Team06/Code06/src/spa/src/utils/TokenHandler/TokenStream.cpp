#include "utils/TokenHandler/TokenStream.h"

// Default constructor.
TokenStream::TokenStream() {
    _tokens = std::vector<std::shared_ptr<ParseableToken>>();
}

// Constructor.
TokenStream::TokenStream(std::vector<std::shared_ptr<ParseableToken>> tokens) {
    _tokens = tokens;
}

// Constructor.
TokenStream::TokenStream(std::vector<std::shared_ptr<SourceToken>> tokens) {
    for (std::shared_ptr<SourceToken> token : tokens) {
        _tokens.push_back(token);
    }
}


// Overload the equals operator.
bool TokenStream::operator==(const TokenStream& other) const {
    if (_tokens.size() != other._tokens.size()) {
        return false;
    }
    for (int i = 0; i < _tokens.size(); i++) {
        if (!_tokens[i]->isEqual(other._tokens[i])) {
            return false;
        }
    }
    return true;
}

// Add a new token to the stream.
void TokenStream::addToken(std::shared_ptr<ParseableToken> token) {
    _tokens.push_back(token);
}

// Peek the next token in the stream without incrementing the index.
std::shared_ptr<ParseableToken> TokenStream::peekNextToken() {
    if (!hasNextToken()) {
        return nullptr;
    }
    return _tokens[_idx];
}

// Peek beyond the next token in the stream without incrementing the index.
 std::shared_ptr<ParseableToken> TokenStream::peekNextToken(int additionalIncrement) {
    if (!hasNextToken()) {
        return nullptr;
    }
    int updatedIndex = _idx + additionalIncrement;
    if (updatedIndex >= _tokens.size()) {
        return nullptr;
    }
    return _tokens[updatedIndex];
}

// Get the next token in the stream with index incrementation.
std::shared_ptr<ParseableToken> TokenStream::getNextToken() {

    if (!hasNextToken()) {
        return nullptr;
    }

    std::shared_ptr<ParseableToken> nextToken =  _tokens[_idx];

    // Increment the token pointer.
    ++_idx;

    return nextToken;
}

std::string TokenStream::getNextTokenValue() {
  std::string s = getNextToken()->getValue();
  return s;
}

// Check if the stream still has tokens.
bool TokenStream::hasNextToken() {
    return _idx < _tokens.size();
}

void TokenStream::saveIndex() {
    _savedIdx = _idx;
}

void TokenStream::restoreIndex() {
    _idx = _savedIdx;
}

// Print all tokens in the stream.
void TokenStream::printAllTokens() {
    for (auto t: _tokens) {
        t->print();
    }
}

// Convert the token stream to a vector of ParseableToken.
std::vector<std::shared_ptr<ParseableToken>> TokenStream::toParseableTokens() {
    return _tokens;
}

// Convert a vector of parseable tokens to a token stream.
TokenStream TokenStream::fromParseableTokens(std::vector<std::shared_ptr<ParseableToken>> tokens) {
    return TokenStream(tokens);
}
