#include "SP/Parser/Parser.h"

// Parse tokens into AST.
std::shared_ptr<SourceProgram> Parser::process(std::shared_ptr<TokenStream> tokenStream) {

    this->tokenStream = tokenStream;
    _sourceProgram = expectProgram();

    // Uncomment to print out Program
    // program->print();

    // Additional processing work:
    checkSemanticErrors();
    
    return _sourceProgram;
}

void Parser::checkSemanticErrors() {

    // Initialise checker for cyclic procedure calls
    CycleDetector cycleDetector = CycleDetector(_callStatements.size());

    // Semantic Error Checks

    // Semantic Error Check 1: A program cannot have two procedures with the same name.
    if (_sourceProgram->getProceduresVector().size() != _sourceProgram->getProceduresMap().size()) {
        throw ParserException(ParserExceptionType::DUPLICATE_PROCEDURE_NAME);
    }

    for (std::shared_ptr<CallStatement> callStatement : _callStatements) {

        std::string parentProcName = callStatement->getParentProcedureName();
        std::string procToCall = callStatement->getCalledProcedureName();
        
        // Semantic Error Check 2: A procedure cannot call a non-existing procedure
        if (!_sourceProgram->hasProcedure(procToCall)) {
            throw ParserException(ParserExceptionType::NO_SUCH_PROCEDURE_TO_CALL);
        }
        
        // Semantic Error Check 3: A procedure cannot call itself (recursive call)
        // Technically, this is checked by CycleDetector as well, but we can throw a more specific error message
        if (parentProcName == procToCall) {
            throw ParserException(ParserExceptionType::RECURSIVE_CALL);
        }

        cycleDetector.addEdge(parentProcName, procToCall);
    }

    // Semantic Error Check 4: Cyclic calls not allowed
    if (cycleDetector.findCycle()) {
        throw ParserException(ParserExceptionType::CYCLIC_CALL);
    }

}

// Expect to parse a program.
std::shared_ptr<SourceProgram> Parser::expectProgram() {
    checkFailCondition(!hasNextToken(), ParserException(ParserExceptionType::INVALID_PROGRAM));

    std::shared_ptr<SourceProgram> program = readSourceProgram();

    // Post-parsing work
    
    // For each call statement, add a reference to the SourceProgram
    for (std::shared_ptr<CallStatement> callStatement : _callStatements) {
        callStatement->setSourceProgramPointer(program);
    }

    return program;
}

// Expect to parse a procedure.
std::shared_ptr<SPProcedure> Parser::expectProcedure() {
    consumeProcedureKeyword();
    std::string procName = readProcName();
    curProcedureName = procName;
    consumeLeftCurlyToken();
    std::shared_ptr<StatementList>stmtList = expectStmtLst();
    consumeRightCurlyToken();

    std::shared_ptr<SPProcedure>proc = std::make_shared<SPProcedure>();

    // Add data into procedure object
    proc->setName(procName);
    proc->setStatements(stmtList);

    // For call statement in statement list, add a reference to the procedure
    for (std::shared_ptr<CallStatement> callStatement : stmtList->getCallStatements()) {
        _callStatements.push_back(callStatement);
        proc->addCallStatement(callStatement);
    }
    return proc;
}

// Expect to parse a list of statements.
std::shared_ptr<StatementList> Parser::expectStmtLst() {

    // Statement list must have at least one statement
    checkFailCondition(peekNextToken()->isRightCurlyBracketToken(),
                       ParserException(ParserExceptionType::INVALID_STMT_LST));

    return readStmtLst();

}

// Expect to parse a statement.
std::shared_ptr<SPStatement> Parser::expectStmt() {
    curLineNo++;

    std::shared_ptr<SourceToken>firstToken = peekNextToken();
    std::shared_ptr<SourceToken>secondToken = peekNextToken(1);

    if (firstToken->isName() && secondToken->isAssignmentToken()) {
        return expectAssign(curLineNo);
    } else if (firstToken->isReadKeyword()) {
        return expectRead(curLineNo);
    } else if (firstToken->isPrintKeyword()) {
        return expectPrint(curLineNo);
    } else if (firstToken->isCallKeyword()) {
        return expectCall(curLineNo);
    } else if (firstToken->isIfKeyword()) {
        return expectIf(curLineNo);
    } else if (firstToken->isWhileKeyword()) {
        return expectWhile(curLineNo);
    } else {
        throw ParserException(ParserExceptionType::INVALID_STMT);
    }
}

// Expect to parse an assign statement.
std::shared_ptr<AssignmentStatement> Parser::expectAssign(int curLineNo) {
    std::string varName = readVarName();
    consumeAssignmentToken();
    std::shared_ptr<SPExpression>expr = expectExpression();
    consumeSemicolonToken();

    return std::make_shared<AssignmentStatement>(varName, expr, curLineNo);
}

// Expect to parse a read statement.
std::shared_ptr<ReadStatement> Parser::expectRead(int curLineNo) {
    consumeReadKeyword();
    std::string varName = readVarName();
    consumeSemicolonToken();

    return std::make_shared<ReadStatement>(varName, curLineNo);
}

// Expect to parse a print statement.
std::shared_ptr<PrintStatement> Parser::expectPrint(int curLineNo) {
    consumePrintKeyword();
    std::string varName = readVarName();
    consumeSemicolonToken();

    return std::make_shared<PrintStatement>(varName, curLineNo);
}

// Expect to parse a call statement.
std::shared_ptr<CallStatement> Parser::expectCall(int curLineNo) {
    consumeCallKeyword();
    std::string varName = readVarName();
    consumeSemicolonToken();

    std::shared_ptr<CallStatement> cs = std::make_shared<CallStatement>(curProcedureName, varName, curLineNo);  // Add reference to SourceProgram eventually
    _callStatements.push_back(cs);  // Track
    return cs;
}

// Expect to parse an if statement.
std::shared_ptr<IfStatement> Parser::expectIf(int curLineNo) {
    consumeIfKeyword();
    consumeLeftRoundToken();
    std::shared_ptr<ConditionalExpression> condExpr = expectConditionalExpression(2);
    consumeRightRoundToken();
    consumeThenKeyword();
    consumeLeftCurlyToken();
    std::shared_ptr<StatementList>consequentStatements = expectStmtLst();
    consumeRightCurlyToken();
    consumeElseKeyword();
    consumeLeftCurlyToken();
    std::shared_ptr<StatementList>alternativeStatements = expectStmtLst();
    consumeRightCurlyToken();

    return std::make_shared<IfStatement>(condExpr, consequentStatements, alternativeStatements, curLineNo);
}

// Expect to parse a while statement.
std::shared_ptr<WhileStatement> Parser::expectWhile(int curLineNo) {
    consumeWhileKeyword();
    consumeLeftRoundToken();
    std::shared_ptr<ConditionalExpression> condExpr = expectConditionalExpression(1);
    consumeRightRoundToken();
    consumeLeftCurlyToken();
    std::shared_ptr<StatementList>whileStatements = expectStmtLst();
    consumeRightCurlyToken();

    return std::make_shared<WhileStatement>(condExpr, whileStatements, curLineNo);
}

// Expect to parse an expression.
std::shared_ptr<SPExpression> Parser::expectExpression() {

    // Extract out the expression
    std::shared_ptr<TokenStream> exprTokenStream = std::make_shared<TokenStream>();

    while (!peekNextToken()->isSemicolonToken()) {
        std::shared_ptr<SourceToken> token = getNextToken();
        if (token->isEndOfStreamToken()) {
            throw ParserException(ParserExceptionType::INVALID_EXPR);
        }
        exprTokenStream->addToken(token);
    }

    // Parse expression
    ExpressionHandler exprHandler = ExpressionHandler(ExpressionType::EXPRESSION, exprTokenStream);
    exprHandler.parse();

    std::vector<std::string> infixExprString = exprHandler.getInfixNotation();
    std::unordered_set<std::string> constants = exprHandler.getConstantsUsed();
    std::unordered_set<std::string> variables = exprHandler.getVarsUsed();

    return std::make_shared<SPExpression>(infixExprString, constants, variables);
}

// Expect to parse a conditional expression.
// For if conditional expressions, it is followed by these tokens: ")" "then" "{"
// For while conditional expressions, it is followed by these tokens: ")" "{"
std::shared_ptr<ConditionalExpression> Parser::expectConditionalExpression(int peekAheadForLeftCurly) {

    // Explanation for peekAheadForLeftCurly:
    // We need the distance from the { token that we should peek ahead
    // At the point of stopping, to be sure that I have obtained all of the conditional expression,
    // my tokenStream pointer should be pointing at the right round bracket
    // So I will have to be two tokens ahead (for if) or one token ahead (for while)

    // Extract out the expression
    std::shared_ptr<TokenStream> condExprTokenStream = std::make_shared<TokenStream>();

    while (!peekNextToken(peekAheadForLeftCurly)->isLeftCurlyBracketToken()) {
        std::shared_ptr<SourceToken> token = getNextToken();
        if (token->isEndOfStreamToken()) {
            throw ParserException(ParserExceptionType::INVALID_COND_EXPR);
        }
        condExprTokenStream->addToken(token);
    }
    // Now I am either two or one tokens away from the right curly bracket }
    // i.e. I am at the right round bracket

    // Parse conditional expression
    ExpressionHandler exprHandler = ExpressionHandler(ExpressionType::CONDITIONAL_EXPRESSION, condExprTokenStream);
    exprHandler.parse();

    std::unordered_set<std::string> variables = exprHandler.getVarsUsed();
    std::unordered_set<std::string> constants = exprHandler.getConstantsUsed();

    return std::make_shared<ConditionalExpression>(constants, variables);
}

/* Helper functions for reading tokens. */
std::shared_ptr<SourceProgram> Parser::readSourceProgram() {
    std::shared_ptr<SourceProgram> program = std::make_shared<SourceProgram>();
    while (hasNextToken()) {
        std::shared_ptr<SPProcedure> p = expectProcedure();
        program->addProcedure(p);
    }
    return program;
}

std::shared_ptr<StatementList> Parser::readStmtLst() {
    std::shared_ptr<StatementList>stmtList = std::make_shared<StatementList>();
    while (!peekNextToken()->isRightCurlyBracketToken()) {
        std::shared_ptr<SPStatement>s = expectStmt();
        stmtList->addStatement(s);
    }
    return stmtList;
}

void Parser::consumeProcedureKeyword() {
    checkFailCondition(
            !getNextToken()->isProcedureKeyword(),
            ParserException(ParserExceptionType::MISSING_PROCEDURE_KEYWORD)
    );
}

void Parser::consumeReadKeyword() {
    checkFailCondition(
            !getNextToken()->isReadKeyword(),
            ParserException(ParserExceptionType::MISSING_READ_KEYWORD)
    );
}

void Parser::consumePrintKeyword() {
    checkFailCondition(
            !getNextToken()->isPrintKeyword(),
            ParserException(ParserExceptionType::MISSING_PRINT_KEYWORD)
    );
}

void Parser::consumeCallKeyword() {
    checkFailCondition(
            !getNextToken()->isCallKeyword(),
            ParserException(ParserExceptionType::MISSING_CALL_KEYWORD)
    );
}

void Parser::consumeIfKeyword() {
    checkFailCondition(
            !getNextToken()->isIfKeyword(),
            ParserException(ParserExceptionType::MISSING_IF_KEYWORD)
    );
}

void Parser::consumeThenKeyword() {
    checkFailCondition(
            !getNextToken()->isThenKeyword(),
            ParserException(ParserExceptionType::MISSING_THEN_KEYWORD)
    );
}

void Parser::consumeElseKeyword() {
    checkFailCondition(
            !getNextToken()->isElseKeyword(),
            ParserException(ParserExceptionType::MISSING_ELSE_KEYWORD)
    );
}

void Parser::consumeWhileKeyword() {
    checkFailCondition(
            !getNextToken()->isWhileKeyword(),
            ParserException(ParserExceptionType::MISSING_WHILE_KEYWORD)
    );
}

void Parser::consumeSemicolonToken() {
    checkFailCondition(
            !getNextToken()->isSemicolonToken(),
            ParserException(ParserExceptionType::MISSING_SEMICOLON)
    );
}

void Parser::consumeLeftCurlyToken() {
    checkFailCondition(
            !getNextToken()->isLeftCurlyBracketToken(),
            ParserException(ParserExceptionType::MISSING_LEFT_CURLY_BRACKET)
    );
}

void Parser::consumeRightCurlyToken() {
    checkFailCondition(
            !getNextToken()->isRightCurlyBracketToken(),
            ParserException(ParserExceptionType::MISSING_RIGHT_CURLY_BRACKET)
    );
}

void Parser::consumeLeftRoundToken() {
    checkFailCondition(
            !getNextToken()->isLeftRoundBracketToken(),
            ParserException(ParserExceptionType::MISSING_LEFT_ROUND_BRACKET)
    );
}

void Parser::consumeAssignmentToken() {
    checkFailCondition(
            !getNextToken()->isAssignmentToken(),
            ParserException(ParserExceptionType::MISSING_ASSIGNMENT)
    );
}

void Parser::consumeRightRoundToken() {
    checkFailCondition(
            !getNextToken()->isRightRoundBracketToken(),
            ParserException(ParserExceptionType::MISSING_RIGHT_ROUND_BRACKET)
    );
}

void Parser::consumeOperatorToken() {
    checkFailCondition(
            !getNextToken()->isOperatorToken(),
            ParserException(ParserExceptionType::MISSING_OPERATOR_TOKEN)
    );
}

void Parser::consumeComparisonToken() {
    checkFailCondition(
            !getNextToken()->isComparisonToken(),
            ParserException(ParserExceptionType::MISSING_COMPARISON_TOKEN)
    );
}

std::string Parser::readProcName() {
    std::shared_ptr<SourceToken>t = getNextToken();
    checkFailCondition(
            !t->isName(),
            ParserException(ParserExceptionType::INVALID_PROCEDURE_NAME)
    );
    return t->getValue();
}

std::string Parser::readVarName() {
    std::shared_ptr<SourceToken>t = getNextToken();
    checkFailCondition(
            !t->isName(),
            ParserException(ParserExceptionType::INVALID_VARIABLE_NAME)
    );
    return t->getValue();
}

void Parser::checkFailCondition(bool failCondition, ParserException exception) {
    if (failCondition) {
        throw exception;
    }
}

bool Parser::hasNextToken() {
    return tokenStream->hasNextToken();
}

std::shared_ptr<SourceToken> Parser::peekNextToken() {
    std::shared_ptr<ParseableToken> token = tokenStream->peekNextToken();
    if (token == nullptr) {
        return SourceToken::createEndToken();
    }
    return std::static_pointer_cast<SourceToken>(tokenStream->peekNextToken());
}

std::shared_ptr<SourceToken> Parser::peekNextToken(int offset) {
    std::shared_ptr<ParseableToken> token = tokenStream->peekNextToken(offset);
    if (token == nullptr) {
        return SourceToken::createEndToken();
    }
    return std::static_pointer_cast<SourceToken>(token);
}

std::shared_ptr<SourceToken> Parser::getNextToken() {
    std::shared_ptr<ParseableToken> token = tokenStream->getNextToken();
    if (token == nullptr) {
        return SourceToken::createEndToken();
    }
    return std::static_pointer_cast<SourceToken>(token);
}
