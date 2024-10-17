#include "SP/DesignExtractor/DesignExtractor.h"


// Constructor.
DesignExtractor::DesignExtractor(std::shared_ptr<PKBSetter> pkbSetter) {

    // TODO: improve this design

    _pkbSetter = pkbSetter;
    _identityExtractor = IdentityExtractor(pkbSetter);
    _followsExtractor = FollowsExtractor(pkbSetter);
    _parentsExtractor = ParentsExtractor(pkbSetter);
    _usesExtractor = UsesExtractor(pkbSetter);
    _modifiesExtractor = ModifiesExtractor(pkbSetter);
    _nextExtractor = NextExtractor(pkbSetter);
    _callsExtractor = CallsExtractor(pkbSetter);

}

Null DesignExtractor::process(std::shared_ptr<SourceProgram> p) {

    for (std::shared_ptr<SPProcedure> proc: p->getProceduresVector()) {
        proc->accept(shared_from_this());
    }

    return Null();
}

void DesignExtractor::extractFromProcedure(std::shared_ptr<SPProcedure> proc) {
    // This class -> Follows (Y), Parents (N), Uses (Y), Modifies (Y), Calls (Y)

    // ========= The procedure itself =========
    _identityExtractor.extractProcedureIdentity(proc);

    // ========= Design Abstractions =========
    _modifiesExtractor.extract(proc);
    _usesExtractor.extract(proc);
    _followsExtractor.extract(proc);
    _callsExtractor.extract(proc);
    _nextExtractor.extract(proc);

}

void DesignExtractor::extractFromAssignmentStatement(std::shared_ptr<AssignmentStatement> assignStmt) {
    // Follows (N), Parents (N), Uses (Y), Modifies (Y), Calls (N)

    // ========= The assignment statement itself =========
    _identityExtractor.extractAssignmentIdentity(assignStmt);

    // ========= Design Abstractions =========
    _modifiesExtractor.extract(assignStmt);
    _usesExtractor.extract(assignStmt);

}

void DesignExtractor::extractFromReadStatement(std::shared_ptr<ReadStatement> readStmt) {
    // Follows (N), Parents (N), Uses (N), Modifies (Y), Calls (N)

    // ========= The read statement itself =========
    _identityExtractor.extractReadIdentity(readStmt);

    // ========= Design Abstractions =========
    _modifiesExtractor.extract(readStmt);
    _usesExtractor.extract(readStmt);

}

void DesignExtractor::extractFromPrintStatement(std::shared_ptr<PrintStatement> printStmt) {
    // Follows (N), Parents (N), Uses (Y), Modifies (N), Calls (N)

    // ========= The print statement itself =========

    // Print statement found on this line number
    _identityExtractor.extractPrintIdentity(printStmt);

    // ========= Design Abstractions =========
    _modifiesExtractor.extract(printStmt);
    _usesExtractor.extract(printStmt);

}

void DesignExtractor::extractFromIfStatement(std::shared_ptr<IfStatement> ifStmt) {
    // Follows (Y), Parents (Y), Uses (Y), Modifies (Y), Calls (N)

    // ========= The if statement itself =========
    _identityExtractor.extractIfIdentity(ifStmt);

    // ========= Design Abstractions =========
    _modifiesExtractor.extract(ifStmt);
    _usesExtractor.extract(ifStmt);
    _followsExtractor.extract(ifStmt);
    _parentsExtractor.extract(ifStmt);

}

void DesignExtractor::extractFromWhileStatement(std::shared_ptr<WhileStatement> whileStmt) {
    // Follows (Y), Parents (Y), Uses (Y), Modifies (Y), Calls (N)

    // ========= The while statement itself =========
    _identityExtractor.extractWhileIdentity(whileStmt);

    // ========= Design Abstractions =========
    _modifiesExtractor.extract(whileStmt);
    _usesExtractor.extract(whileStmt);
    _followsExtractor.extract(whileStmt);
    _parentsExtractor.extract(whileStmt);

}


void DesignExtractor::extractFromCallStatement(std::shared_ptr<CallStatement> callStmt) {
    // Follows (N), Parents (N), Uses (Y), Modifies (Y), Calls (N)

    // ========= The call statement itself =========
    _identityExtractor.extractCallIdentity(callStmt);

    // ========= Design Abstractions =========
    _modifiesExtractor.extract(callStmt);
    _usesExtractor.extract(callStmt);

}
