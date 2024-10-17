#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>

#include "PKB/PKBSetter.h"
#include "SP/AST/Constructs.h"
#include "SP/DesignExtractor/Extractables.h"


// Extracts from a class that inherits UsesExtractable
class UsesExtractor {

private:
    std::shared_ptr<PKBSetter> _pkbSetter;
    void _addAllVariablesUsed(std::unordered_set<std::string> variables);
    void _addAllConstantsUsed(std::unordered_set<std::string> constants);
    void _addVariablesUsed(int lineNum, std::unordered_set<std::string> variables);
    void _addVariablesUsed(std::string procName, std::unordered_set<std::string> variables);
public:
    UsesExtractor();
    UsesExtractor(std::shared_ptr<PKBSetter> pkbSetter);
    void extract(std::shared_ptr<UsesExtractable> usesExtractable);
};
