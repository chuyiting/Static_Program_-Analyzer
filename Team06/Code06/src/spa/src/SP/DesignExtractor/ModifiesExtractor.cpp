#include "SP/DesignExtractor/ModifiesExtractor.h"


ModifiesExtractor::ModifiesExtractor() = default;

ModifiesExtractor::ModifiesExtractor(std::shared_ptr<PKBSetter> pkbSetter) {
    // Reference to PKB
    _pkbSetter = pkbSetter;
}

void ModifiesExtractor::_addAllVariablesModified(std::unordered_set<std::string> variables) {
    for (std::string variable : variables) {
        _pkbSetter->addVariable(variable);
    }
}

void ModifiesExtractor::_addVariablesModified(int lineNum, std::unordered_set<std::string> variables) {
    std::string stringLineNum = std::to_string(lineNum);
    for (std::string variable: variables) {
        _pkbSetter->addModifiesS(stringLineNum, variable);
    }
}

void ModifiesExtractor::_addVariablesModified(std::string procName, std::unordered_set<std::string> variables) {
    _pkbSetter->addModifiesP(procName, variables);
}

void ModifiesExtractor::extract(std::shared_ptr<ModifiesExtractable> modifiesExtractable) {
    std::unordered_set<std::string> variablesModified = modifiesExtractable->getVariablesModified();

    // if modifiesExtractable is SPProcedure
    if (modifiesExtractable->getExtractableType() == ExtractableType::PROCEDURE) {
        std::shared_ptr<SPProcedure> proc = std::static_pointer_cast<SPProcedure>(modifiesExtractable);
        std::string procName = proc->getName();
        _addVariablesModified(procName, variablesModified);
        
        // Deliberate choice: we only add the variables modified in the procedure
        // While we can also add the variables modified for each statement, there will be duplication
        // since the variables modified in the procedure will be modified in the statements
    
        _addAllVariablesModified(variablesModified);
    }

    // if modifiesExtractable is SPStatement
    else if (modifiesExtractable->getExtractableType() == ExtractableType::STATEMENT) {
        std::shared_ptr<SPStatement> stmt = std::static_pointer_cast<SPStatement>(modifiesExtractable);
        int lineNum = stmt->getBaseLineNo();
        _addVariablesModified(lineNum, variablesModified);
    }
}
