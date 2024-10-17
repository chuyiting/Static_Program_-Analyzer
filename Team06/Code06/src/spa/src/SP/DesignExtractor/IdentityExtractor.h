#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>

#include "PKB/PKBSetter.h"
#include "SP/AST/Constructs.h"


// Extracts the basic information of each class
class IdentityExtractor {

private:

    // Reference to PKB
    std::shared_ptr<PKBSetter> _pkbSetter;

public:

    IdentityExtractor();
    IdentityExtractor(std::shared_ptr<PKBSetter> pkbSetter);

    void extractProcedureIdentity(std::shared_ptr<SPProcedure> proc);
    void extractAssignmentIdentity(std::shared_ptr<AssignmentStatement> assignStmt);
    void extractReadIdentity(std::shared_ptr<ReadStatement> readStmt);
    void extractPrintIdentity(std::shared_ptr<PrintStatement> printStmt);
    void extractIfIdentity(std::shared_ptr<IfStatement> ifStmt);
    void extractWhileIdentity(std::shared_ptr<WhileStatement> whileStmt);
    void extractCallIdentity(std::shared_ptr<CallStatement> callStmt);
    void extractAssignmentExpression(std::shared_ptr<AssignmentStatement> assignStmt);

};
