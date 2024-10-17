#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PKB/PKBSetter.h"
#include "PKB/type/pkb_types.h"
#include "SP/AST/Constructs.h"
#include "SP/DesignExtractor/CallsExtractor.h"
#include "SP/DesignExtractor/Extractables.h"
#include "SP/DesignExtractor/FollowsExtractor.h"
#include "SP/DesignExtractor/IdentityExtractor.h"
#include "SP/DesignExtractor/FollowsExtractor.h"
#include "SP/DesignExtractor/ParentsExtractor.h"
#include "SP/DesignExtractor/UsesExtractor.h"
#include "SP/DesignExtractor/ModifiesExtractor.h"
#include "SP/DesignExtractor/NextExtractor.h"
#include "SP/DesignExtractor/CallsExtractor.h"
#include "SP/DesignExtractor/NullSentinel.h"
#include "SP/Pipeline/PipelineComponent.h"

class DesignExtractor : public PipelineComponent<std::shared_ptr<SourceProgram>, Null>,
    public Visitor, public std::enable_shared_from_this<DesignExtractor> {

private:
    // Reference to PKB
    std::shared_ptr<PKBSetter> _pkbSetter;

    // Extract basic information
    IdentityExtractor _identityExtractor;

    // Extract design abstractions
    FollowsExtractor _followsExtractor;
    ParentsExtractor _parentsExtractor;
    UsesExtractor _usesExtractor;
    ModifiesExtractor _modifiesExtractor;
    NextExtractor _nextExtractor;
    CallsExtractor _callsExtractor;

public:
    // Constructor.
    DesignExtractor(std::shared_ptr<PKBSetter> pkbSetter);

    // Functions.
    Null process(std::shared_ptr<SourceProgram> p);

    // Visitor pattern
    void extractFromProcedure(std::shared_ptr<SPProcedure> proc);

    void extractFromAssignmentStatement(std::shared_ptr<AssignmentStatement> assignStmt);

    void extractFromReadStatement(std::shared_ptr<ReadStatement> readStmt);

    void extractFromPrintStatement(std::shared_ptr<PrintStatement> printStmt);

    void extractFromCallStatement(std::shared_ptr<CallStatement> callStmt);

    void extractFromIfStatement(std::shared_ptr<IfStatement> ifStmt);

    void extractFromWhileStatement(std::shared_ptr<WhileStatement> whileStmt);
};
