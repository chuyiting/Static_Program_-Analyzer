#include "SP/SourceProcessor.h"

// Constructor.
SourceProcessor::SourceProcessor(std::shared_ptr<PKBSetter> pkbSetter) {
    _pkbSetter = pkbSetter;
    _init_components();
}

void SourceProcessor::_init_components() {
    fileReader = std::make_shared<FileReader>();
    tokenizer = std::make_shared<Tokenizer>();
    parser = std::make_shared<Parser>();
    designExtractor = std::make_shared<DesignExtractor>(_pkbSetter);
}

// Process source file.
void SourceProcessor::processFile(std::string filePath) {

    try {
        Pipeline(filePath)
                .process(std::move(fileReader))
                .process(std::move(tokenizer))
                .process(std::move(parser))
                .process(std::move(designExtractor));
    }

    catch (TokenizerException &e) {
        std::cout << "Tokenizer Exception: " << e.what() << std::endl;
        throw;
    }

    catch (ParserException &e) {
        std::cout << "Parser Exception: " << e.what() << std::endl;
        throw;
    }
}

// Process source file.
void SourceProcessor::processCode(std::string sourceCode) {

    try {
        Pipeline(sourceCode)
                .process(std::move(tokenizer))
                .process(std::move(parser))
                .process(std::move(designExtractor));
    }

    catch (TokenizerException &e) {
        std::cout << "Tokenizer Exception: " << e.what() << std::endl;
        throw;
    }

    catch (ParserException &e) {
        std::cout << "Parser Exception: " << e.what() << std::endl;
        throw;
    }
}

