#pragma once

#include <stdio.h>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

#include "Exceptions/ParserException.h"
#include "SP/AST/Constructs.h"
#include "SP/Pipeline/PipelineComponent.h"
#include "utils/CycleDetector/CycleDetector.h"
#include "utils/ExpressionHandler/ExpressionHandler.h"
#include "utils/ExpressionHandler/Validator.h"
#include "utils/TokenHandler/SourceTokens.h"
#include "utils/TokenHandler/TokenStream.h"


class Parser : public PipelineComponent<std::shared_ptr<TokenStream>, std::shared_ptr<SourceProgram>> {
private:
    // Field variables.
    std::shared_ptr<TokenStream> tokenStream;
    int curLineNo = 0;
    std::string curProcedureName;  // name of the procedure we are looking at currently

    // Additional work: store call statements, add a reference to SourceProgram
    std::shared_ptr<SourceProgram> _sourceProgram;
    std::vector<std::shared_ptr<CallStatement>> _callStatements;

    // Functions.
    
    void checkSemanticErrors();

    std::shared_ptr<SourceProgram> expectProgram();

    std::shared_ptr<SPProcedure> expectProcedure();

    std::shared_ptr<StatementList> expectStmtLst();

    std::shared_ptr<SPStatement> expectStmt();

    std::shared_ptr<AssignmentStatement> expectAssign(int curLineNo);

    std::shared_ptr<ReadStatement> expectRead(int curLineNo);

    std::shared_ptr<PrintStatement> expectPrint(int curLineNo);

    std::shared_ptr<CallStatement> expectCall(int curLineNo);

    std::shared_ptr<IfStatement> expectIf(int curLineNo);

    std::shared_ptr<WhileStatement> expectWhile(int curLineNo);

    std::shared_ptr<SPExpression> expectExpression();

    std::shared_ptr<ConditionalExpression> expectConditionalExpression(int peekAheadForLeftCurly);

    std::shared_ptr<SourceProgram> readSourceProgram();

    std::shared_ptr<StatementList> readStmtLst();

    void consumeProcedureKeyword();

    void consumeReadKeyword();

    void consumePrintKeyword();

    void consumeCallKeyword();

    void consumeIfKeyword();

    void consumeThenKeyword();

    void consumeElseKeyword();

    void consumeWhileKeyword();

    void consumeSemicolonToken();

    void consumeLeftCurlyToken();

    void consumeRightCurlyToken();

    void consumeLeftRoundToken();

    void consumeRightRoundToken();

    void consumeAssignmentToken();

    void consumeOperatorToken();

    void consumeComparisonToken();

    // Helper methods
    std::string readProcName();
    std::string readVarName();
    void checkFailCondition(bool failCondition, ParserException exception);
    bool hasNextToken();
    std::shared_ptr<SourceToken> peekNextToken();
    std::shared_ptr<SourceToken> peekNextToken(int offset);
    std::shared_ptr<SourceToken> getNextToken();

public:
    // Function.
    std::shared_ptr<SourceProgram> process(std::shared_ptr<TokenStream> tokenStream);
};
