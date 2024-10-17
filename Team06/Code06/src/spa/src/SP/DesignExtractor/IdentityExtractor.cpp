#include "SP/DesignExtractor/IdentityExtractor.h"


IdentityExtractor::IdentityExtractor() = default;

IdentityExtractor::IdentityExtractor(std::shared_ptr<PKBSetter> pkbSetter) {
    // Reference to PKB
    _pkbSetter = pkbSetter;
}

void IdentityExtractor::extractProcedureIdentity(std::shared_ptr<SPProcedure>  proc) {

    // Get information
    std::string procName = proc->getName();
    std::unordered_set<int> statementNumbers = proc->getLineNumbers();

    // Convert integer statement numbers to string
    std::unordered_set<std::string> stringStmtNumbers = std::unordered_set<std::string>();
    for (int statementNumber: statementNumbers) {
        stringStmtNumbers.insert(std::to_string(statementNumber));
    }

    // This procedure consists of these statement numbers
    _pkbSetter->addProcedure(procName, stringStmtNumbers);
}

void IdentityExtractor::extractAssignmentIdentity(std::shared_ptr<AssignmentStatement> assignStmt) {

    // Get information
    int assignStmtLineNo = assignStmt->getBaseLineNo();  // we probably want integer
    std::string stringLineNo = std::to_string(assignStmtLineNo);  // string for now
    std::string assignStmtVar = assignStmt->getVariableName();

    _pkbSetter->addAssignmentStmt(stringLineNo, assignStmtVar);

    // Additional work to do
    extractAssignmentExpression(assignStmt);
}

void IdentityExtractor::extractReadIdentity(std::shared_ptr<ReadStatement> readStmt) {

    // Get information
    int readStmtLineNo = readStmt->getBaseLineNo();
    std::string stringLineNo = std::to_string(readStmtLineNo);
    std::string readStmtVar = readStmt->getVariableName();

    // Read statement found on this line number
    _pkbSetter->addReadStmt(stringLineNo, readStmtVar);
}

void IdentityExtractor::extractPrintIdentity(std::shared_ptr<PrintStatement> printStmt) {

    // Get information
    int printStmtLineNo = printStmt->getBaseLineNo();
    std::string stringLineNo = std::to_string(printStmtLineNo);
    std::string printStmtVar = printStmt->getVariableName();
    
    // Print statement found on this line number
    _pkbSetter->addPrintStmt(stringLineNo, printStmtVar);
}

void IdentityExtractor::extractIfIdentity(std::shared_ptr<IfStatement> ifStmt) {

    // Get information
    int ifStmtLineNo = ifStmt->getBaseLineNo();
    std::string stringLineNo = std::to_string(ifStmtLineNo);
    std::shared_ptr<ConditionalExpression> predicate = ifStmt->getPredicate();

    // If statement found on this line number
    _pkbSetter->addIfStmt(stringLineNo);

    // Add the variables used in the predicate
    _pkbSetter->addUsesIf(stringLineNo, predicate->getVariables());
}

void IdentityExtractor::extractWhileIdentity(std::shared_ptr<WhileStatement> whileStmt) {

    // Get information
    int whileStmtLineNo = whileStmt->getBaseLineNo();
    std::string stringLineNo = std::to_string(whileStmtLineNo);
    std::shared_ptr<ConditionalExpression>predicate = whileStmt->getPredicate();

    // While statement found on this line number
    _pkbSetter->addWhileStmt(stringLineNo);
    
    // Add the variables used in the predicate
    _pkbSetter->addUsesWhile(stringLineNo, predicate->getVariables());
}

void IdentityExtractor::extractCallIdentity(std::shared_ptr<CallStatement> callStmt) {

    // Get information
    int callStmtLineNo = callStmt->getBaseLineNo();
    std::string stringLineNo = std::to_string(callStmtLineNo);
    std::string calledProcName = callStmt->getCalledProcedureName();

    // Call statement found on this line number, calling this procedure
    _pkbSetter->addCallStmt(stringLineNo, calledProcName);
}

void IdentityExtractor::extractAssignmentExpression(std::shared_ptr<AssignmentStatement> assignStmt) {

    int assignStmtLineNo = assignStmt->getBaseLineNo();  // we probably want integer
    std::string stringLineNo = std::to_string(assignStmtLineNo);  // string for now

    // Get the expression on the RHS of the assignment
    std::shared_ptr<SPExpression> assignStmtExpr = assignStmt->getExpression();
    std::vector<std::string> infixExpr = assignStmtExpr->getInfixExpr();

    // Expression found on the assignment on this line number
    _pkbSetter->addAssignmentExpr(stringLineNo, infixExpr);
}
