#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Exceptions/ParserException.h"
#include "SP/DesignExtractor/Extractables.h"

class SPProcedure;

class AssignmentStatement;

class ReadStatement;

class PrintStatement;

class CallStatement;

class IfStatement;

class WhileStatement;

// Abstract classes that uses the Visitor design pattern.
class Visitor {
public:
    virtual void extractFromProcedure(std::shared_ptr<SPProcedure> proc) = 0;

    virtual void extractFromAssignmentStatement(std::shared_ptr<AssignmentStatement> assignStmt) = 0;

    virtual void extractFromReadStatement(std::shared_ptr<ReadStatement> readStmt) = 0;

    virtual void extractFromPrintStatement(std::shared_ptr<PrintStatement> printStmt) = 0;

    virtual void extractFromCallStatement(std::shared_ptr<CallStatement> callStmt) = 0;

    virtual void extractFromIfStatement(std::shared_ptr<IfStatement> ifStmt) = 0;

    virtual void extractFromWhileStatement(std::shared_ptr<WhileStatement> whileStmt) = 0;

};

class Visitable {
public:
    virtual void accept(std::shared_ptr<Visitor> visitor) = 0;
};


// Class that represents an expression, which is an infix expression in the SIMPLE program.
class SPExpression {
private:
    // Field variables.
    std::vector<std::string> _infixExpression;
    std::unordered_set<std::string> _constants = std::unordered_set<std::string>();
    std::unordered_set<std::string> _variables = std::unordered_set<std::string>();

public:
    // Constructors.
    SPExpression();

    SPExpression(std::vector<std::string> infixExprString,
                 std::unordered_set<std::string> constants,
                 std::unordered_set<std::string> variables);

    // Getters.
    std::vector<std::string> getInfixExpr();

    std::unordered_set<std::string> getConstants();

    std::unordered_set<std::string> getVariables();

    // Must be fulfilled.
    void print(std::string indent = "");  // From self.
};


// Class that represents a conditional expression.
class ConditionalExpression {
private:
    // Field variable.
    std::unordered_set<std::string> _constants = std::unordered_set<std::string>();
    std::unordered_set<std::string> _variables = std::unordered_set<std::string>();
public:
    // Constructors.
    ConditionalExpression();

    ConditionalExpression(std::unordered_set<std::string> constants,
                          std::unordered_set<std::string> variables);

    // Getters.
    std::unordered_set<std::string> getConstants();

    std::unordered_set<std::string> getVariables();

    // Must be fulfilled.
    void print(std::string indent = "");  // From self
};

enum class StatementType {
    ASSIGN, READ, PRINT, CALL, WHILE, IF
};

// Abstract class that represents a statement, can be an assignment, read, print, call, while, or if statement.
class SPStatement : public UsesExtractable, public ModifiesExtractable, public Visitable {
protected:
    // Field variables.
    StatementType _statementType;
    int _baseLineNo;

public:
    // Constructors.
    SPStatement(StatementType statementType, int baseLineNo);

    // Getters.
    StatementType getStatementType();

    int getBaseLineNo();

    // Functions.
    bool isAssignmentStatement();

    bool isReadStatement();

    bool isPrintStatement();

    bool isCallStatement();

    bool isWhileBlock();

    bool isIfBlock();
    
    // Fulfilling contract
    ExtractableType getExtractableType();  // From ModifiesExtractable & UsesExtractable

    // Contract for subclasses.
    virtual void print(std::string indent) = 0;  // Requirement from SPStatement (ownself)
    virtual std::unordered_set<int> getLineNumbers() = 0;  // Requirement from SPStatement (ownself)
    virtual std::unordered_set<std::string> getVariablesModified() = 0;  // Requirement from ModifiesExtractable
    virtual std::unordered_set<std::string> getConstantsUsed() = 0;  // Requirement from UsesExtractable
    virtual std::unordered_set<std::string> getVariablesUsed() = 0;  // Requirement from UsesExtractable
    virtual void accept(std::shared_ptr<Visitor> visitor) = 0;  // Requirement from Visitable

};

class StatementList : public Visitable {

private:
    std::vector<std::shared_ptr<SPStatement>> _statements;

public:
    // Getters.
    std::vector<std::shared_ptr<SPStatement>> getStatements();
    int getFirstStmtNo();
    std::unordered_set<int> getLastStmtNos();
    int getSize();

    // Functions.
    void addStatement(std::shared_ptr<SPStatement> statement);
    std::vector<std::shared_ptr<CallStatement>> getCallStatements();
    
    std::unordered_map<int, std::unordered_set<int>> getEdgesHelper(int followingLineNum); // For NextExtractable

    // Must be fulfilled.
    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable
    void print(std::string indent = "");  // From self
};


// Class that represents a procedure, which is of the form "procedure procedureName { statementList }".
class SPProcedure : public UsesExtractable, public ModifiesExtractable, public FollowsExtractable,
    public CallsExtractable, public NextExtractable, public Visitable, public std::enable_shared_from_this<SPProcedure> {
public:
    // Field variables.
    std::string procName = "";
    std::shared_ptr<StatementList>statementList;
    std::vector<std::shared_ptr<CallStatement>> callStatements;

    // Setters.
    void setName(std::string procedureName);

    void setStatements(std::shared_ptr<StatementList>stmtList);

    // Getters.
    std::unordered_set<int> getLineNumbers();

    std::shared_ptr<StatementList>getStatementList();

    std::string getName();

    void addCallStatement(std::shared_ptr<CallStatement> callStmt);
    std::vector<std::shared_ptr<CallStatement>> getCallStatements();

    // Must be fulfilled.
    ExtractableType getExtractableType();  // From ModifiesExtractable & UsesExtractable
    std::unordered_set<std::string> getVariablesModified();  // From ModifiesExtractable
    std::unordered_set<std::string> getConstantsUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesUsed();  // From UsesExtractable
    std::vector<std::vector<int>> getFollowsStatements();  // From FollowsExtractable
    std::unordered_map<int, std::unordered_set<int>> getEdges(); // From NextExtractable
    std::string getCallerName();  // From CallsExtractable
    std::vector<std::string> getProceduresCalled();  // From CallsExtractable

    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable

    // Functions.
    void print(std::string indent = "");  // TODO: remove, for debugging purposes only
};

// Class that represents a program, which consists of procedures.
class SourceProgram {
private:
    std::map<std::string, std::shared_ptr<SPProcedure>> _proceduresMap;
    std::vector<std::shared_ptr<SPProcedure>> _proceduresVector;

    // Additional work: store call statements, add a reference to SourceProgram
    std::vector<std::shared_ptr<CallStatement>> callStatements;
public:
    std::map<std::string, std::shared_ptr<SPProcedure>> getProceduresMap();
    std::vector<std::shared_ptr<SPProcedure>> getProceduresVector();

    std::shared_ptr<SPProcedure>getProcedure(std::string procName);

    void addProcedure(std::shared_ptr<SPProcedure>proc);
    bool hasProcedure(std::string procName);
    void print(std::string indent = "");
};


// Class that represents an assignment statement, which is of the form "variable = expression".
class AssignmentStatement : public SPStatement, public std::enable_shared_from_this<AssignmentStatement> {
private:
    // Field variables.
    std::string _variableName;
    std::shared_ptr<SPExpression> _expression;
public:
    // Constructor.
    AssignmentStatement(std::string varName, std::shared_ptr<SPExpression> expr, int lineNo);

    // Getters.
    std::string getVariableName();

    std::shared_ptr<SPExpression> getExpression();

    // Must be fulfilled.
    std::unordered_set<int> getLineNumbers();  // Requirement from SPStatement
    std::unordered_set<std::string> getConstantsUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesModified();  // From ModifiesExtractable
    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable

    // Functions.
    void print(std::string indent = "");
};

// Class that represents a read statement, which is of the form "read variable".
class ReadStatement : public SPStatement, public std::enable_shared_from_this<ReadStatement> {
private:
    // Field variables.
    std::string _variableName;
public:
    // Constructor.
    ReadStatement(std::string varName, int lineNo);

    // Getters.
    std::string getVariableName();

    // Must be fulfilled.
    std::unordered_set<int> getLineNumbers();  // Requirement from SPStatement
    std::unordered_set<std::string> getConstantsUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesModified();  // From ModifiesExtractable
    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable

    // Functions.
    void print(std::string indent = "");
};


// Class that represents a print statement, which is of the form "print variable".
class PrintStatement : public SPStatement, public std::enable_shared_from_this<PrintStatement> {
private:
    // Field variables.
    std::string _variableName;
public:
    // Constructor.
    PrintStatement(std::string varName, int lineNo);

    // Getters.
    std::string getVariableName();

    // Must be fulfilled.
    std::unordered_set<int> getLineNumbers();  // Requirement from SPStatement
    std::unordered_set<std::string> getConstantsUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesModified();  // From ModifiesExtractable
    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable

    // Functions.
    void print(std::string indent = "");
};

// Class that represents a call statement, which is of the form "call procedureName".
class CallStatement : public SPStatement, public std::enable_shared_from_this<CallStatement> {
private:
    // Field variables.
    std::string _parentProcedureName;  // the name of the procedure that this call statement belongs to
    std::string _procedureName;  // the name of the procedure that this call statement calls

    // We need a pointer to the source program we are in, because we could have parsed and instantiated
    // a CallStatement which calls a procedure that we have not parsed yet
    // After parsing that procedure next time, we can find it again by using this pointer
    std::shared_ptr<SourceProgram> _sourceProgramPointer;
public:
    // Constructor.
    CallStatement(std::string parentProcName, std::string procName, int lineNo);

    // Getters.
    std::string getParentProcedureName();
    std::string getCalledProcedureName();
    std::shared_ptr<SPProcedure>getCalledProcedure();  // Helper method to get the procedure that this call statement calls

    // Setters.
    void setSourceProgramPointer(std::shared_ptr<SourceProgram> sourceProgramPointer);

    // Must be fulfilled.
    std::unordered_set<int> getLineNumbers();  // Requirement from SPStatement
    std::unordered_set<std::string> getConstantsUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesModified();  // From ModifiesExtractable
    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable

    // Functions.
    void print(std::string indent = "");
};

// Class that represents an if statement, which is of the form "if (expression) then {statementList} else {statementList}".
class IfStatement : public SPStatement, public FollowsExtractable,
    public ParentsExtractable, public std::enable_shared_from_this<IfStatement> {
public:
    // Field variables.
    std::shared_ptr<ConditionalExpression> predicate;
    std::shared_ptr<StatementList>consequent;
    std::shared_ptr<StatementList>alternative;

    // Constructor.
    IfStatement(
            std::shared_ptr<ConditionalExpression> condition,
            std::shared_ptr<StatementList>consequentStatements,
            std::shared_ptr<StatementList>alternativeStatements,
            int lineNo
    );

    // Getters.
    std::shared_ptr<ConditionalExpression> getPredicate();

    std::shared_ptr<StatementList>getConsequent();

    std::shared_ptr<StatementList>getAlternative();

    std::vector<std::shared_ptr<CallStatement>> getCallStatements();

    // Must be fulfilled.
    std::unordered_set<int> getLineNumbers();  // Requirement from SPStatement
    std::unordered_set<std::string> getConstantsUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesModified();  // From ModifiesExtractable
    std::vector<std::vector<int>> getFollowsStatements();  // From FollowsExtractable
    std::vector<std::pair<int, std::vector<int>>> getParentsRelationships();  // From ParentsExtractable
    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable
    
    std::unordered_map<int, std::unordered_set<int>> getEdgesHelper(int followingLineNum); // For NextExtractable for SPProcedure

    // Functions.
    void print(std::string indent = "");
};

// Class that represents a while block, which is of the form "while (expression) { statementList }".
class WhileStatement : public SPStatement, public FollowsExtractable,
    public ParentsExtractable, public std::enable_shared_from_this<WhileStatement> {

public:
    // Field variables.
    std::shared_ptr<ConditionalExpression> predicate;
    std::shared_ptr<StatementList>body;

    // Constructor.
    WhileStatement(std::shared_ptr<ConditionalExpression> condition, std::shared_ptr<StatementList>statements, int lineNo);

    // Getters.
    std::shared_ptr<ConditionalExpression> getPredicate();

    std::shared_ptr<StatementList>getBody();
    
    std::vector<std::shared_ptr<CallStatement>> getCallStatements();

    // Must be fulfilled.
    std::unordered_set<int> getLineNumbers();
    std::unordered_set<std::string> getConstantsUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesUsed();  // From UsesExtractable
    std::unordered_set<std::string> getVariablesModified();  // From ModifiesExtractable
    std::vector<std::vector<int>> getFollowsStatements();  // From FollowsExtractable
    std::vector<std::pair<int, std::vector<int> > > getParentsRelationships();  // From ParentsExtractable
    void accept(std::shared_ptr<Visitor> visitor);  // From Visitable
    
    std::unordered_map<int, std::unordered_set<int>> getEdgesHelper(int followingLineNum); // For NextExtractable for SPProcedure

    // Functions.
    void print(std::string indent = "");
};
