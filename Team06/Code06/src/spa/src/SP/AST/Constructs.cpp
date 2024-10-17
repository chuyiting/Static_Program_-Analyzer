#include "SP/AST/Constructs.h"

/* ========== Expression ========== */

// Default constructor.
SPExpression::SPExpression() {
    _infixExpression = {};
    _constants = std::unordered_set<std::string>();
    _variables = std::unordered_set<std::string>();
}

// Constructor.
SPExpression::SPExpression(std::vector<std::string> infixExprString,
                           std::unordered_set<std::string> constants,
                           std::unordered_set<std::string> variables) {
    _infixExpression = infixExprString;
    _constants = constants;
    _variables = variables;
}

// Get infix vector.
std::vector<std::string> SPExpression::getInfixExpr() {
    return _infixExpression;
}

// Get constants.
std::unordered_set<std::string> SPExpression::getConstants() {
    return _constants;
}

// Get variables.
std::unordered_set<std::string> SPExpression::getVariables() {
    return _variables;
}

// Print expression.
void SPExpression::print(std::string indent) {
    std::cout << indent << "Infix Expression: ";
    for (std::string term: _infixExpression) {
        std::cout << term;
    }
    std::cout << std::endl;

    std::cout << indent << "Constants used: " << std::endl;
    for (std::string term: _constants) {
        std::cout << indent << "\t" << term << std::endl;
    }
    std::cout << indent << "Variables used: " << std::endl;
    for (std::string term: _variables) {
        std::cout << indent << "\t" << term << std::endl;
    }
    std::cout << std::endl;
}

/* ========== ConditionalExpression ========== */

// Default constructor.
ConditionalExpression::ConditionalExpression() {
    _constants = std::unordered_set<std::string>();
    _variables = std::unordered_set<std::string>();
}

// Constructor.
ConditionalExpression::ConditionalExpression(std::unordered_set<std::string> constants,
                                             std::unordered_set<std::string> variables) {
    _constants = constants;
    _variables = variables;
}


// Must implement
// Get constants.
std::unordered_set<std::string> ConditionalExpression::getConstants() {
    return _constants;
}

// Get variables.
std::unordered_set<std::string> ConditionalExpression::getVariables() {
    return _variables;
}

// Print expression.
void ConditionalExpression::print(std::string indent) {
    std::cout << indent << "Constants used: " << std::endl;
    for (std::string term: _constants) {
        std::cout << indent << "\t" << term << std::endl;
    }
    std::cout << indent << "Variables used: " << std::endl;
    for (std::string term: _variables) {
        std::cout << indent << "\t" << term << std::endl;
    }
    std::cout << std::endl;
}


/* ========== StatementList ========== */

void StatementList::addStatement(std::shared_ptr<SPStatement> stmt) {
    _statements.push_back(stmt);
}

std::unordered_map<int, std::unordered_set<int>> StatementList::getEdgesHelper(int followingLineNum) {

    // To populate
    std::unordered_map<int, std::unordered_set<int>> edges;

    // Information
    int n = _statements.size();

    // Storing variables
    std::shared_ptr<SPStatement> curStatement;
    int curStatementNum, nextStatementNum;
    std::unordered_map<int, std::unordered_set<int>> curStatementInternalEdges;  // If curStatement is if or while, this will contain edges in its statement lists

    // Iterate through all pairs of statements
    for (int i = 0; i < n; i++) {

        // Get information
        curStatement = _statements[i];
        curStatementNum = curStatement->getBaseLineNo();

        if (i < n - 1) {
            nextStatementNum = _statements[i + 1]->getBaseLineNo();
        } else {
            nextStatementNum = followingLineNum;  // Might or might not be 0
        }

        if (curStatement->isIfBlock()) {
            
            std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(curStatement);

            // Get all edges WITHIN the statement, add them to edges
            curStatementInternalEdges = ifStatement->getEdgesHelper(nextStatementNum);  // pass in next statement number
            edges.insert(curStatementInternalEdges.begin(), curStatementInternalEdges.end());
        }

        else if (curStatement->isWhileBlock()) {

            std::shared_ptr<WhileStatement> whileStatement = std::static_pointer_cast<WhileStatement>(curStatement);

            // Get all edges WITHIN the statement, add them to edges
            curStatementInternalEdges = whileStatement->getEdgesHelper(nextStatementNum);
            edges.insert(curStatementInternalEdges.begin(), curStatementInternalEdges.end());
        }

        // If the first statement is "simple" (not if or while), then we can add an edge of first->second
        // Technically, a while statement has this link also, but we will add it manually in the while check
        else {
            edges[curStatementNum].insert(nextStatementNum);
        }
    }

    return edges;
}

std::vector<std::shared_ptr<SPStatement>> StatementList::getStatements() {
    return _statements;
}

std::vector<std::shared_ptr<CallStatement>> StatementList::getCallStatements() {

    std::vector<std::shared_ptr<CallStatement>> allCallStatements;

    for (std::shared_ptr<SPStatement> stmt: _statements) {

        // If it is a call statement
        if (stmt->getStatementType() ==  StatementType::CALL) {
            allCallStatements.push_back(std::static_pointer_cast<CallStatement>(stmt));
        }

        // If it is an if statement
        if (stmt->getStatementType() ==  StatementType::IF) {
            std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(stmt);
            std::vector<std::shared_ptr<CallStatement>> ifCallStmts = ifStatement->getCallStatements();
            allCallStatements.insert(allCallStatements.end(), ifCallStmts.begin(), ifCallStmts.end());
        }

        // If it is a while statement
        if (stmt->getStatementType() ==  StatementType::WHILE) {
            std::shared_ptr<WhileStatement> whileStatement = std::static_pointer_cast<WhileStatement>(stmt);
            std::vector<std::shared_ptr<CallStatement>> whileCallStmts = whileStatement->getCallStatements();
            allCallStatements.insert(allCallStatements.end(), whileCallStmts.begin(), whileCallStmts.end());
        }
        
    }
    return allCallStatements;
}

int StatementList::getFirstStmtNo() {
    return _statements.front()->getBaseLineNo();
}

std::unordered_set<int> StatementList::getLastStmtNos() {
    // return _statements.back()->getBaseLineNo();

    std::shared_ptr<SPStatement> lastStatement = _statements.back();
    
    // If it is a simple statement, then just return its line number
    if (lastStatement->getStatementType() != StatementType::IF) {
        return {lastStatement->getBaseLineNo()};
    } else {
        std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(lastStatement);

        std::shared_ptr<StatementList> thenStmtList = ifStatement->getConsequent();
        std::shared_ptr<StatementList> elseStmtList = ifStatement->getAlternative();

        std::unordered_set<int> thenLastStmtNos = thenStmtList->getLastStmtNos();
        std::unordered_set<int> elseLastStmtNos = elseStmtList->getLastStmtNos();

        std::unordered_set<int> lastStmtNos;
        // merge
        lastStmtNos.insert(thenLastStmtNos.begin(), thenLastStmtNos.end());
        lastStmtNos.insert(elseLastStmtNos.begin(), elseLastStmtNos.end());
        return lastStmtNos;
    }
}

int StatementList::getSize() {
    return _statements.size();
}

void StatementList::print(std::string indent) {
    for (std::shared_ptr<SPStatement>  stmt: _statements) {
        stmt->print(indent);
    }
}

void StatementList::accept(std::shared_ptr<Visitor> visitor) {
    for (std::shared_ptr<SPStatement> stmt: _statements) {
        stmt->accept(visitor);
    }
}

/* ========== Statement ========== */

// Constructor.
SPStatement::SPStatement(StatementType stmtType, int baseLineNo) {
    _statementType = stmtType;
    _baseLineNo = baseLineNo;
}

// Get statement type.
StatementType SPStatement::getStatementType() {
    return _statementType;
}

int SPStatement::getBaseLineNo() {
    return _baseLineNo;
}

bool SPStatement::isAssignmentStatement() {
    return _statementType == StatementType::ASSIGN;
}

bool SPStatement::isReadStatement() {
    return _statementType == StatementType::READ;
}

bool SPStatement::isPrintStatement() {
    return _statementType == StatementType::PRINT;
}

bool SPStatement::isCallStatement() {
    return _statementType == StatementType::CALL;
}

bool SPStatement::isWhileBlock() {
    return _statementType == StatementType::WHILE;
}

bool SPStatement::isIfBlock() {
    return _statementType == StatementType::IF;
}

ExtractableType SPStatement::getExtractableType() {
    return ExtractableType::STATEMENT;
}

/* ========== AssignmentStatement ========== */

// Constructor.
AssignmentStatement::AssignmentStatement(std::string varName, std::shared_ptr<SPExpression> expr, int lineNo) : SPStatement(
        StatementType::ASSIGN, lineNo
) {
    _variableName = varName;
    _expression = expr;
}

// Get variable name.
std::string AssignmentStatement::getVariableName() {
    return _variableName;
}

std::shared_ptr<SPExpression> AssignmentStatement::getExpression() {
    return _expression;
}


// Virtual methods
std::unordered_set<int> AssignmentStatement::getLineNumbers() {
    // return a set initialised with just the base line number
    return std::unordered_set<int>{_baseLineNo};
}


std::unordered_set<std::string> AssignmentStatement::getConstantsUsed() {
    return _expression->getConstants();
}

std::unordered_set<std::string> AssignmentStatement::getVariablesUsed() {
    return _expression->getVariables();
}

std::unordered_set<std::string> AssignmentStatement::getVariablesModified() {
    return std::unordered_set<std::string>{_variableName};
}

void AssignmentStatement::accept(std::shared_ptr<Visitor> visitor) {
    visitor->extractFromAssignmentStatement(shared_from_this());
}

// Print the assignment statement.
void AssignmentStatement::print(std::string indent) {
    std::cout << indent << "Assignment statement (line no. " << _baseLineNo << ")" << std::endl;
    std::cout << indent << "\t" << "Variable name: " << _variableName << std::endl;
    _expression->print(indent + "\t");
}

/* ========== ReadStatement ========== */

// Constructor.
ReadStatement::ReadStatement(std::string varName, int lineNo) : SPStatement(StatementType::READ, lineNo) {
    _variableName = varName;
}

// Get variable name.
std::string ReadStatement::getVariableName() {
    return _variableName;
}


// Virtual methods
std::unordered_set<int> ReadStatement::getLineNumbers() {
    // return a set initialised with just the base line number
    return std::unordered_set<int>{_baseLineNo};
}

std::unordered_set<std::string> ReadStatement::getConstantsUsed() {
    return std::unordered_set<std::string>();
}

std::unordered_set<std::string> ReadStatement::getVariablesUsed() {
    return std::unordered_set<std::string>();
}

std::unordered_set<std::string> ReadStatement::getVariablesModified() {
    return std::unordered_set<std::string>{_variableName};
}

void ReadStatement::accept(std::shared_ptr<Visitor> visitor) {
    visitor->extractFromReadStatement(shared_from_this());
}

// Print the read statement.
void ReadStatement::print(std::string indent) {
    std::cout << indent << "Read statement (line no. " << _baseLineNo << ")" << std::endl;
    std::cout << indent << "\t" << "Variable to read: " << _variableName << std::endl;
}

/* ========== PrintStatement ========== */

// Constructor.
PrintStatement::PrintStatement(std::string varName, int lineNo) : SPStatement(StatementType::PRINT, lineNo) {
    _variableName = varName;
}

// Get variable name.
std::string PrintStatement::getVariableName() {
    return _variableName;
}


// Virtual methods
std::unordered_set<int> PrintStatement::getLineNumbers() {
    // return a set initialised with just the base line number
    return std::unordered_set<int>{_baseLineNo};
}

std::unordered_set<std::string> PrintStatement::getConstantsUsed() {
    return std::unordered_set<std::string>();
}

std::unordered_set<std::string> PrintStatement::getVariablesUsed() {
    return std::unordered_set<std::string>{_variableName};
}

std::unordered_set<std::string> PrintStatement::getVariablesModified() {
    return std::unordered_set<std::string>();
}

void PrintStatement::accept(std::shared_ptr<Visitor> visitor) {
    visitor->extractFromPrintStatement(shared_from_this());
}

// Print the print statement.
void PrintStatement::print(std::string indent) {
    std::cout << indent << "Print statement (line no. " << _baseLineNo << ")" << std::endl;
    std::cout << indent << "\t" << "Variable to print: " << _variableName << std::endl;
}

/* ========== CallStatement ========== */

// Constructor.
CallStatement::CallStatement(std::string parentProcName, std::string procName, int lineNo) : SPStatement(StatementType::CALL, lineNo) {
    _parentProcedureName = parentProcName;
    _procedureName = procName;
}

std::string CallStatement::getParentProcedureName() {
    return _parentProcedureName;
}

// Get procedure name.
std::string CallStatement::getCalledProcedureName() {
    return _procedureName;
}

// Get procedure.
std::shared_ptr<SPProcedure> CallStatement::getCalledProcedure() {
    return _sourceProgramPointer->getProcedure(_procedureName);
}

// Set source program pointer.
void CallStatement::setSourceProgramPointer(std::shared_ptr<SourceProgram> sourceProgramPointer) {
    _sourceProgramPointer = sourceProgramPointer;
}

void CallStatement::accept(std::shared_ptr<Visitor> visitor) {
    visitor->extractFromCallStatement(shared_from_this());
}

// Virtual methods
std::unordered_set<int> CallStatement::getLineNumbers() {
    // return a set initialised with just the base line number
    return std::unordered_set<int>{_baseLineNo};
}

std::unordered_set<std::string> CallStatement::getConstantsUsed() {
    // Guaranteed to be an existing procedure.
    std::shared_ptr<SPProcedure> procedure = _sourceProgramPointer->getProcedure(_procedureName);
    return procedure->getConstantsUsed();
}

std::unordered_set<std::string> CallStatement::getVariablesUsed() {
    // Guaranteed to be an existing procedure.
    std::shared_ptr<SPProcedure> procedure = _sourceProgramPointer->getProcedure(_procedureName);
    return procedure->getVariablesUsed();
}

std::unordered_set<std::string> CallStatement::getVariablesModified() {
    // Guaranteed to be an existing procedure.
    std::shared_ptr<SPProcedure> procedure = _sourceProgramPointer->getProcedure(_procedureName);
    return procedure->getVariablesModified();
}

// Print the call statement.
void CallStatement::print(std::string indent) {
    std::cout << indent << "Call statement (line no. " << _baseLineNo << ")" << std::endl;
    std::cout << indent << "\t" << "Procedure to call: " << _procedureName << std::endl;
}

/* ========== IfStatement ========== */

IfStatement::IfStatement(
        std::shared_ptr<ConditionalExpression> condition,
        std::shared_ptr<StatementList> consequentStatements,
        std::shared_ptr<StatementList> alternativeStatements,
        int lineNo
) : SPStatement(StatementType::IF, lineNo) {
    predicate = condition;
    consequent = consequentStatements;
    alternative = alternativeStatements;
}

// Getters
std::shared_ptr<ConditionalExpression> IfStatement::getPredicate() {
    return predicate;
}

std::shared_ptr<StatementList> IfStatement::getConsequent() {
    return consequent;
}

std::shared_ptr<StatementList> IfStatement::getAlternative() {
    return alternative;
}


std::vector<std::shared_ptr<CallStatement>> IfStatement::getCallStatements() {

    std::vector<std::shared_ptr<CallStatement>> callStatements = std::vector<std::shared_ptr<CallStatement>>();

    std::vector<std::shared_ptr<CallStatement>> consequentCallStatements = consequent->getCallStatements();
    callStatements.insert(callStatements.end(), consequentCallStatements.begin(), consequentCallStatements.end());

    std::vector<std::shared_ptr<CallStatement>> alternativeCallStatements = alternative->getCallStatements();
    callStatements.insert(callStatements.end(), alternativeCallStatements.begin(), alternativeCallStatements.end());

    return callStatements;

}


// Virtual methods

std::unordered_set<int> IfStatement::getLineNumbers() {

    std::unordered_set<int> lineNumbers;
    lineNumbers.insert(_baseLineNo);
    for (std::shared_ptr<SPStatement>  statement: consequent->getStatements()) {
        std::unordered_set<int> statementLineNumbers = statement->getLineNumbers();
        lineNumbers.insert(statementLineNumbers.begin(), statementLineNumbers.end());
    }
    for (std::shared_ptr<SPStatement>  statement: alternative->getStatements()) {
        std::unordered_set<int> statementLineNumbers = statement->getLineNumbers();
        lineNumbers.insert(statementLineNumbers.begin(), statementLineNumbers.end());
    }
    return lineNumbers;

}

std::unordered_set<std::string> IfStatement::getConstantsUsed() {
    std::unordered_set<std::string> constantsUsed;

    // Add constants used in predicate.
    std::unordered_set<std::string> predicateConstantsUsed = predicate->getConstants();
    constantsUsed.insert(predicateConstantsUsed.begin(), predicateConstantsUsed.end());

    // For each statement in consequent, add constants used in statement
    for (std::shared_ptr<SPStatement>  statement: consequent->getStatements()) {
        std::unordered_set<std::string> statementConstantsUsed = statement->getConstantsUsed();
        constantsUsed.insert(statementConstantsUsed.begin(), statementConstantsUsed.end());
    }

    // For each statement in alternative, add constants used in statement
    for (std::shared_ptr<SPStatement>  statement: alternative->getStatements()) {
        std::unordered_set<std::string> statementConstantsUsed = statement->getConstantsUsed();
        constantsUsed.insert(statementConstantsUsed.begin(), statementConstantsUsed.end());
    }

    return constantsUsed;
}

std::unordered_set<std::string> IfStatement::getVariablesUsed() {
    std::unordered_set<std::string> varsUsed;

    // Add variables used in predicate.
    std::unordered_set<std::string> predicateVarsUsed = predicate->getVariables();
    varsUsed.insert(predicateVarsUsed.begin(), predicateVarsUsed.end());

    // For each statement in consequent, add variables used in statement
    for (std::shared_ptr<SPStatement>  stmt: consequent->getStatements()) {
        std::unordered_set<std::string> stmtVarsUsed = stmt->getVariablesUsed();
        varsUsed.insert(stmtVarsUsed.begin(), stmtVarsUsed.end());
    }

    // For each statement in alternative, add variables used in statement
    for (std::shared_ptr<SPStatement>  stmt: alternative->getStatements()) {
        std::unordered_set<std::string> stmtVarsUsed = stmt->getVariablesUsed();
        varsUsed.insert(stmtVarsUsed.begin(), stmtVarsUsed.end());
    }

    return varsUsed;
}

std::unordered_set<std::string> IfStatement::getVariablesModified() {

    std::unordered_set<std::string> varsModified;

    // For each statement in consequent, add variables modified in statement
    for (std::shared_ptr<SPStatement>  stmt: consequent->getStatements()) {
        std::unordered_set<std::string> stmtVarsModified = stmt->getVariablesModified();
        varsModified.insert(stmtVarsModified.begin(), stmtVarsModified.end());
    }

    // For each statement in alternative, add variables modified in statement
    for (std::shared_ptr<SPStatement>  stmt: alternative->getStatements()) {
        std::unordered_set<std::string> stmtVarsModified = stmt->getVariablesModified();
        varsModified.insert(stmtVarsModified.begin(), stmtVarsModified.end());
    }

    return varsModified;
}

std::vector<std::vector<int>> IfStatement::getFollowsStatements() {

    std::vector<int> consequentFollowsStatements;
    // For each statement in consequent, add follows statements in consequentFollowsStatements
    for (std::shared_ptr<SPStatement>  stmt: consequent->getStatements()) {
        consequentFollowsStatements.push_back(stmt->getBaseLineNo());
    }

    std::vector<int> alternateFollowsStatements;
    // For each statement in alternative, add follows statements in alternateFollowsStatements
    for (std::shared_ptr<SPStatement>  stmt: alternative->getStatements()) {
        alternateFollowsStatements.push_back(stmt->getBaseLineNo());
    }

    std::vector<std::vector<int>> setsOfStatements = {consequentFollowsStatements, alternateFollowsStatements};

    return setsOfStatements;
}


std::vector<std::pair<int, std::vector<int> > > IfStatement::getParentsRelationships() {

    std::vector<std::pair<int, std::vector<int> > > parentsRelationships;

    int baseIfLineNo = _baseLineNo;

    // Add parents relationship for consequent statements
    std::vector<int> consequentStatementLineNos;
    for (std::shared_ptr<SPStatement>  statement: consequent->getStatements()) {
        consequentStatementLineNos.push_back(statement->getBaseLineNo());
    }

    parentsRelationships.push_back(std::make_pair(baseIfLineNo, consequentStatementLineNos));

    // Add parents relationship for alternative statements
    std::vector<int> alternativeStatementLineNos;
    for (std::shared_ptr<SPStatement>  statement: alternative->getStatements()) {
        alternativeStatementLineNos.push_back(statement->getBaseLineNo());
    }

    parentsRelationships.push_back(std::make_pair(baseIfLineNo, alternativeStatementLineNos));

    return parentsRelationships;
}

std::unordered_map<int, std::unordered_set<int>> IfStatement::getEdgesHelper(int followingLineNum) {

    std::unordered_map<int, std::unordered_set<int>> edges;
    std::unordered_map<int, std::unordered_set<int>> thenEdges = consequent->getEdgesHelper(followingLineNum);
    std::unordered_map<int, std::unordered_set<int>> elseEdges = alternative->getEdgesHelper(followingLineNum);

    // Add edge from if statement to first statement in then block
    edges[_baseLineNo].insert(consequent->getFirstStmtNo());

    // Add edge from if statement to first statement in else block
    edges[_baseLineNo].insert(alternative->getFirstStmtNo());

    // Add edges from consequent
    edges.insert(thenEdges.begin(), thenEdges.end());
    
    // Add edges from alternative
    edges.insert(elseEdges.begin(), elseEdges.end());

    // Add edge from last statement in then block to following line
    std::unordered_set<int> lastThenStmts = consequent->getLastStmtNos();
    for (int lastThenStmt: lastThenStmts) {
        edges[lastThenStmt].insert(followingLineNum);
    }

    // Add edge from last statement in else block to following line
    std::unordered_set<int> lastElseStmts = alternative->getLastStmtNos();
    for (int lastElseStmt: lastElseStmts) {
        edges[lastElseStmt].insert(followingLineNum);
    }

    return edges;
}

void IfStatement::accept(std::shared_ptr<Visitor> visitor) {
    visitor->extractFromIfStatement(shared_from_this());
    consequent->accept(visitor);
    alternative->accept(visitor);
}


void IfStatement::print(std::string indent) {
    std::cout << indent << "If statement (line no. " << _baseLineNo << ")" << std::endl;

    std::cout << indent << "Predicate:" << std::endl;
    predicate->print(indent + "\t");

    std::cout << indent << "Consequent statements:" << std::endl;
    consequent->print(indent + "\t");

    std::cout << indent << "Alternative statements:" << std::endl;
    alternative->print(indent + "\t");
}

/* ========== WhileStatement ========== */

WhileStatement::WhileStatement(
        std::shared_ptr<ConditionalExpression> condition,
        std::shared_ptr<StatementList> statements,
        int lineNo
) : SPStatement(StatementType::WHILE, lineNo) {
    predicate = condition;
    body = statements;
}

// Getters

std::shared_ptr<ConditionalExpression> WhileStatement::getPredicate() {
    return predicate;
}

std::shared_ptr<StatementList> WhileStatement::getBody() {
    return body;
}

std::vector<std::shared_ptr<CallStatement>> WhileStatement::getCallStatements() {
    return body->getCallStatements();
}

std::unordered_set<int> WhileStatement::getLineNumbers() {

    std::unordered_set<int> lineNumbers;
    lineNumbers.insert(_baseLineNo);
    for (std::shared_ptr<SPStatement>  statement: body->getStatements()) {
        std::unordered_set<int> statementLineNumbers = statement->getLineNumbers();
        lineNumbers.insert(statementLineNumbers.begin(), statementLineNumbers.end());
    }
    return lineNumbers;

}

std::unordered_set<std::string> WhileStatement::getConstantsUsed() {
    std::unordered_set<std::string> constantsUsed;

    // Add constants used in predicate.
    std::unordered_set<std::string> predicateConstantsUsed = predicate->getConstants();
    constantsUsed.insert(predicateConstantsUsed.begin(), predicateConstantsUsed.end());

    // for each statement in body, add constants used in statement
    for (std::shared_ptr<SPStatement>  stmt: body->getStatements()) {
        std::unordered_set<std::string> stmtConstantsUsed = stmt->getConstantsUsed();
        constantsUsed.insert(stmtConstantsUsed.begin(), stmtConstantsUsed.end());
    }

    return constantsUsed;
}

std::unordered_set<std::string> WhileStatement::getVariablesUsed() {
    std::unordered_set<std::string> varsUsed;

    // Add variables used in predicate.
    std::unordered_set<std::string> predicateVarsUsed = predicate->getVariables();
    varsUsed.insert(predicateVarsUsed.begin(), predicateVarsUsed.end());

    // for each statement in body, add vars used in statement
    for (std::shared_ptr<SPStatement>  stmt: body->getStatements()) {
        std::unordered_set<std::string> stmtVarsUsed = stmt->getVariablesUsed();
        varsUsed.insert(stmtVarsUsed.begin(), stmtVarsUsed.end());
    }

    return varsUsed;
}

std::unordered_set<std::string> WhileStatement::getVariablesModified() {

    std::unordered_set<std::string> varsModified;

    // for each statement in body, add vars modified in statement
    for (std::shared_ptr<SPStatement>  stmt: body->getStatements()) {
        std::unordered_set<std::string> stmtVarsModified = stmt->getVariablesModified();
        varsModified.insert(stmtVarsModified.begin(), stmtVarsModified.end());
    }

    return varsModified;
}

std::vector<std::vector<int>> WhileStatement::getFollowsStatements() {

    std::vector<int> bodyFollowsStatements;
    // For each statement in consequent, add follows statements in consequentFollowsStatements
    for (std::shared_ptr<SPStatement>  stmt: body->getStatements()) {
        bodyFollowsStatements.push_back(stmt->getBaseLineNo());
    }

    std::vector<std::vector<int>> setsOfStatements = {bodyFollowsStatements};

    return setsOfStatements;
}

std::vector<std::pair<int, std::vector<int> > > WhileStatement::getParentsRelationships() {

    std::vector<std::pair<int, std::vector<int> > > parentsRelationships;

    int baseIfLineNo = _baseLineNo;

    // Add parents relationship for body statements
    std::vector<int> consequentStatementLineNos;
    for (std::shared_ptr<SPStatement>  statement: body->getStatements()) {
        consequentStatementLineNos.push_back(statement->getBaseLineNo());
    }

    parentsRelationships.push_back(std::make_pair(baseIfLineNo, consequentStatementLineNos));

    return parentsRelationships;
}

std::unordered_map<int, std::unordered_set<int>> WhileStatement::getEdgesHelper(int followingLineNum) {

    std::unordered_map<int, std::unordered_set<int>> edges;

    // In a while loop, the last line in the body does not go to the following line after the while statement
    // So we pass in 0
    std::unordered_map<int, std::unordered_set<int>> bodyEdges = body->getEdgesHelper(_baseLineNo);  

    // Add edge from while statement to first statement in body
    edges[_baseLineNo].insert(body->getFirstStmtNo());

    // Add edges from body
    edges.insert(bodyEdges.begin(), bodyEdges.end());

    // Add edge from last statement in body to while statement
    // edges[body->getLastStmtNo()].insert(_baseLineNo);

    // Add edge from while statement to following line
    edges[_baseLineNo].insert(followingLineNum);

    return edges;
}

void WhileStatement::accept(std::shared_ptr<Visitor> visitor) {
    visitor->extractFromWhileStatement(shared_from_this());
    body->accept(visitor);
}

void WhileStatement::print(std::string indent) {
    std::cout << indent << "While statement (line no. " << _baseLineNo << ")" << std::endl;

    std::cout << indent << "Predicate:" << std::endl;
    predicate->print(indent + "\t");

    std::cout << indent << "Body statements:" << std::endl;
    body->print(indent + "\t");
}

/* ========== Procedure ========== */

void SPProcedure::setName(std::string procedureName) {
    procName = procedureName;
}

void SPProcedure::setStatements(std::shared_ptr<StatementList> stmtList) {
    statementList = stmtList;
}

// Virtual methods
ExtractableType SPProcedure::getExtractableType() {
    return ExtractableType::PROCEDURE;
}


std::unordered_set<std::string> SPProcedure::getConstantsUsed() {
    std::unordered_set<std::string> constantsUsed;

    // for each statement in procedure, add constants used in statement
    for (std::shared_ptr<SPStatement>  stmt: statementList->getStatements()) {
        std::unordered_set<std::string> stmtConstantsUsed = stmt->getConstantsUsed();
        constantsUsed.insert(stmtConstantsUsed.begin(), stmtConstantsUsed.end());
    }

    return constantsUsed;
}

std::unordered_set<std::string> SPProcedure::getVariablesUsed() {
    std::unordered_set<std::string> varsUsed;

    // for each statement in statements, add vars used in statement
    for (std::shared_ptr<SPStatement>  stmt: statementList->getStatements()) {
        std::unordered_set<std::string> stmtVarsUsed = stmt->getVariablesUsed();
        varsUsed.insert(stmtVarsUsed.begin(), stmtVarsUsed.end());
    }

    return varsUsed;
}

std::unordered_set<std::string> SPProcedure::getVariablesModified() {

    std::unordered_set<std::string> varsModified;

    // for each statement in statements, add vars modified in statement
    for (std::shared_ptr<SPStatement>  stmt: statementList->getStatements()) {
        std::unordered_set<std::string> stmtVarsModified = stmt->getVariablesModified();
        varsModified.insert(stmtVarsModified.begin(), stmtVarsModified.end());
    }

    return varsModified;
}

std::vector<std::vector<int>> SPProcedure::getFollowsStatements() {

    std::vector<int> followsStatements;

    // for each statement in procedure body, add follows statements
    for (std::shared_ptr<SPStatement>  stmt: statementList->getStatements()) {
        followsStatements.push_back(stmt->getBaseLineNo());
    }

    std::vector<std::vector<int>> setsOfStatements = {followsStatements};

    return setsOfStatements;
}

std::unordered_map<int, std::unordered_set<int>> SPProcedure::getEdges() {
    return statementList->getEdgesHelper(0);
}

std::string SPProcedure::getCallerName() {
    return procName;
}

std::vector<std::string> SPProcedure::getProceduresCalled() {

    std::vector<std::string> proceduresCalled;

    // for each call statement in procedure body, add procedure called
    for (std::shared_ptr<CallStatement> callStmt: callStatements) {
        proceduresCalled.push_back(callStmt->getCalledProcedureName());
    }

    return proceduresCalled;
}

void SPProcedure::accept(std::shared_ptr<Visitor> visitor) {
    visitor->extractFromProcedure(shared_from_this());
    statementList->accept(visitor);   // pass the visitor on to the nested statement list
}

void SPProcedure::print(std::string indent) {
    std::cout << indent << "Procedure name: " << procName << std::endl;
    statementList->print(indent + "\t");
}

std::unordered_set<int> SPProcedure::getLineNumbers() {
    std::unordered_set<int> lineNumbers;
    for (std::shared_ptr<SPStatement>  stmt: statementList->getStatements()) {
        std::unordered_set<int> stmtLineNumbers = stmt->getLineNumbers();
        lineNumbers.insert(stmtLineNumbers.begin(), stmtLineNumbers.end());
    }
    return lineNumbers;
}

std::shared_ptr<StatementList> SPProcedure::getStatementList() {
    return statementList;
}

std::string SPProcedure::getName() {
    return procName;
}

void SPProcedure::addCallStatement(std::shared_ptr<CallStatement> callStmt) {
    callStatements.push_back(callStmt);
}

std::vector<std::shared_ptr<CallStatement>> SPProcedure::getCallStatements() {
    return callStatements;
}

/* ========== SourceProgram ========== */

std::map<std::string, std::shared_ptr<SPProcedure>> SourceProgram::getProceduresMap() {
    return _proceduresMap;
}

std::vector<std::shared_ptr<SPProcedure>> SourceProgram::getProceduresVector() {
    return _proceduresVector;
}

std::shared_ptr<SPProcedure> SourceProgram::getProcedure(std::string procName) {
    if (_proceduresMap.find(procName) == _proceduresMap.end()) {
        throw ParserException(ParserExceptionType::INVALID_PROCEDURE_NAME);
    }
    return _proceduresMap[procName];
}

void SourceProgram::addProcedure(std::shared_ptr<SPProcedure> proc) {
    _proceduresMap[proc->getName()] = proc;
    _proceduresVector.push_back(proc);
}

bool SourceProgram::hasProcedure(std::string procName) {
    return _proceduresMap.find(procName) != _proceduresMap.end();
}

void SourceProgram::print(std::string indent) {
    for (std::shared_ptr<SPProcedure> proc: _proceduresVector) {
        proc->print(indent);
        std::cout << std::endl;
    }
}
