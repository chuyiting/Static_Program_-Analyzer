#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SP/AST/Constructs.h"
#include "SP/DesignExtractor/NullSentinel.h"
#include "SP/InputReader/FileReader.h"
#include "SP/Parser/Parser.h"
#include "SP/Pipeline/Pipeline.h"
#include "SP/Pipeline/PipelineComponent.h"
#include "SP/Tokenizer/Tokenizer.h"
#include "SP/DesignExtractor/DesignExtractor.h"
#include "PKB/PKBSetter.h"
#include "Exceptions/ParserException.h"
#include "Exceptions/TokenizerException.h"
#include "utils/TokenHandler/TokenStream.h"

class SourceProcessor {
private:

    // Field variable.
    std::shared_ptr<PKBSetter> _pkbSetter;

    // Pipeline components available
    std::shared_ptr<PipelineComponent<std::string, std::string>> fileReader;
    std::shared_ptr<PipelineComponent<std::string, std::shared_ptr<TokenStream>>> tokenizer;
    std::shared_ptr<PipelineComponent<std::shared_ptr<TokenStream>, std::shared_ptr<SourceProgram>>> parser;
    std::shared_ptr<PipelineComponent<std::shared_ptr<SourceProgram>, Null>> designExtractor;

    // Initialize pipeline components.
    void _init_components();


public:
    // Constructor.
    SourceProcessor(std::shared_ptr<PKBSetter> pkbSetter);

    // Functions.
    void processFile(std::string fileName);
    void processCode(std::string sourceCode);
};
