#pragma once

#include <stdio.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <exception>

#include "SP/Pipeline/PipelineComponent.h"
#include "utils/TokenHandler/SourceTokens.h"
#include "utils/TokenHandler/TokenStream.h"

class Tokenizer : public PipelineComponent<std::string, std::shared_ptr<TokenStream>> {
private:
    // Field variables.
    std::string _sourceCode;
    int _programLength;
    int _cursorPosition;

    // Function.
    void _advancePointer(int length);

public:
    // Constructor.
    Tokenizer();

    // Functions.
    std::shared_ptr<TokenStream> process(std::string sourceCode);

    bool hasMoreTokens();

    std::shared_ptr<SourceToken> tokenizeNextItem();
};
