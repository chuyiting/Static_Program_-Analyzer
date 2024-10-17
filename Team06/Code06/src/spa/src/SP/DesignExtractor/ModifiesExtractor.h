#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>

#include "PKB/PKBSetter.h"
#include "SP/AST/Constructs.h"
#include "SP/DesignExtractor/Extractables.h"


// Extracts from a class that inherits ModifiesExtractable
class ModifiesExtractor {

private:
    std::shared_ptr<PKBSetter> _pkbSetter;
    void _addAllVariablesModified(std::unordered_set<std::string> variables);
    void _addVariablesModified(int lineNum, std::unordered_set<std::string> variables);
    void _addVariablesModified(std::string procName, std::unordered_set<std::string> variables);
public:
    ModifiesExtractor();
    ModifiesExtractor(std::shared_ptr<PKBSetter> pkbSetter);
    void extract(std::shared_ptr<ModifiesExtractable> modifiesExtractable);
};
