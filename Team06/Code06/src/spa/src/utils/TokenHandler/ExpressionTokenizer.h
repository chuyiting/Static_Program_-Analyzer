#pragma once

#include <memory>
#include <string>
#include <vector>

#include "utils/TokenHandler/SourceTokens.h"
#include "utils/TokenHandler/TokenStream.h"

class ExpressionTokenizer {
private:
    // Field variables.
    std::string _expression;
    int _exprLength;
    int _cursorPosition;

    // Function.
    void _advancePointer(int length);
    bool _hasMoreTokens();
    std::shared_ptr<SourceToken> _tokenizeNextItem();

public:
    // Constructor.
    ExpressionTokenizer();

    // Functions.
    std::shared_ptr<TokenStream> process(std::string expression);

};
