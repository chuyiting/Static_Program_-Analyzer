#include "SP/DesignExtractor/UsesExtractor.h"


UsesExtractor::UsesExtractor() = default;

UsesExtractor::UsesExtractor(std::shared_ptr<PKBSetter> pkbSetter) {
    // Reference to PKB
    _pkbSetter = pkbSetter;
}

void UsesExtractor::_addAllConstantsUsed(std::unordered_set<std::string> constants) {
  for (std::string constant: constants) {
    _pkbSetter->addConstant(constant);
  }
}

void UsesExtractor::_addAllVariablesUsed(std::unordered_set<std::string> variables) {
    for (std::string variable : variables) {
        _pkbSetter->addVariable(variable);
    }
}

void UsesExtractor::_addVariablesUsed(int lineNum, std::unordered_set<std::string> variables) {
    std::string stringLineNum = std::to_string(lineNum);
    _pkbSetter->addUsesS(stringLineNum, variables);
}

void UsesExtractor::_addVariablesUsed(std::string procName, std::unordered_set<std::string> variables) {
    _pkbSetter->addUsesP(procName, variables);
}

void UsesExtractor::extract(std::shared_ptr<UsesExtractable> usesExtractable) {

    std::unordered_set<std::string> variablesUsed = usesExtractable->getVariablesUsed();
    std::unordered_set<std::string> constantsUsed = usesExtractable->getConstantsUsed();

    // if usesExtractable is SPProcedure
    if (usesExtractable->getExtractableType() == ExtractableType::PROCEDURE) {
        std::shared_ptr<SPProcedure> proc = std::static_pointer_cast<SPProcedure>(usesExtractable);
        std::string procName = proc->getName();
        _addVariablesUsed(procName, variablesUsed);

        // Deliberate choice: we only add the variables and constants used in the procedure
        // While we can also add the variables and constants used for each statement, there will be duplication
        // since the variables and constants used in the procedure will be used in the statements

        _addAllVariablesUsed(variablesUsed);
        _addAllConstantsUsed(constantsUsed);
    }

    // if usesExtractable is SPStatement
    else if (usesExtractable->getExtractableType() == ExtractableType::STATEMENT) {
        std::shared_ptr<SPStatement> stmt = std::static_pointer_cast<SPStatement>(usesExtractable);
        int lineNum = stmt->getBaseLineNo();
        _addVariablesUsed(lineNum, variablesUsed);
    }
}
