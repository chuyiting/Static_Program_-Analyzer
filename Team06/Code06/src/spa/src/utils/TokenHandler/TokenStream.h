#pragma once

#include <memory>
#include <string>
#include <vector>

#include "utils/TokenHandler/ParseableToken.h"
#include "utils/TokenHandler/SourceTokens.h"

class TokenStream {

private:
    int _savedIdx = 0;  // one time saving, for purposes of restoring the stream
    int _idx = 0;
    std::vector<std::shared_ptr<ParseableToken>> _tokens;

public:
    // Constructors.
    TokenStream();
    TokenStream(std::vector<std::shared_ptr<ParseableToken>> tokens);
    TokenStream(std::vector<std::shared_ptr<SourceToken>> tokens);

    // Operator overloading.
    bool operator==(const TokenStream& other) const;

    // Functions.
    void saveIndex();
    void restoreIndex();
    void printAllTokens();

    // Setters.
    void addToken(std::shared_ptr<ParseableToken> token);

    // Getters.
    std::shared_ptr<ParseableToken> peekNextToken();
    std::shared_ptr<ParseableToken> peekNextToken(int additionalIncrement);
    std::shared_ptr<ParseableToken> getNextToken();
    std::string getNextTokenValue();
    bool hasNextToken();

    // Helpers.
    std::vector<std::shared_ptr<ParseableToken>> toParseableTokens();
    static TokenStream fromParseableTokens(std::vector<std::shared_ptr<ParseableToken>> tokens);
};
