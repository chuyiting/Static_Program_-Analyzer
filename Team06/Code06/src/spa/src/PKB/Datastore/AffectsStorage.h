#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_AFFECTSSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_AFFECTSSTORAGE_H_
#include "PKB/type/pkb_types.h"
#include "utils/map_utils.h"
#include "PKB/Datastore/NextStorage.h"
#include "PKB/Datastore/ModifiesStorage.h"
#include "PKB/Datastore/UsesStorage.h"
#include "PKB/Datastore/SynonymStorage.h"
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>

using namespace pkb_types;
class AffectsStorage {
private:
    // Required data.
    NextStorage &_next_storage;
    ModifiesStorage &_modifies_storage;
    UsesStorage &_uses_storage;
    SynonymStorage &_synonym_storage;

    // Caching.
    std::unordered_map<Statement, std::unordered_set<Statement>> affects;
    std::unordered_map<Statement, std::unordered_set<Statement>> affectsT;
public:
    // Constructor.
    AffectsStorage(
        NextStorage &_next_storage,
        ModifiesStorage &_modifies_storage,
        UsesStorage &_uses_storage,
        SynonymStorage &_synonym_storage
    );

    // Querying.
    bool isAffectsDirect(const Entity &lhs, const Entity &rhs);
    bool isAffectsRight(const Entity &lhs);
    bool isAffectsLeft(const Entity &rhs);
    bool isAffectsTDirect(const Entity &lhs, const Entity &rhs);
    bool isAffectsExists();

    // Set returns.
    std::unordered_set<Statement> getAffects(const Entity &lhs);
    std::unordered_set<Statement> getAffectsT(const Entity &lhs);
    std::unordered_set<Statement> getAffectedBy(const Entity &rhs);
    std::unordered_set<Statement> getAffectedByT(const Entity &rhs);

    // Graph traversal.
    bool isBfsAffectsDirect(const Statement& src, const Statement& dst, const Variable &modifiedVar);
    bool isBfsAffectsRight(const Statement& src, const Variable& modifiedVar);
    bool isBfsAffectsLeft(const Statement& src, const std::unordered_set<Statement>& usedVars);
    bool isBfsAffectsTDirect(const Statement& src, const Statement& dst);
    std::unordered_set<std::string> bfsAffectsRight(const Statement& src, const Variable& modifiedVar);
    std::unordered_set<std::string> bfsAffectsTRight(const Statement& src);

    // Caching.
    void clearCache();
    void cacheAffects(const Entity& lhs, const Entity& rhs);
    void cacheAffectsT(const Entity& lhs, const Entity& rhs);

    // Helper methods.
    Variable getModifiedVariable(const Entity& lhs);

    Procedure getProcedure(const Statement& stmt);
    std::set<Statement> getAssignStmtsInProcedure(const Statement& stmt);

    bool isInSameProcedure(const Statement& s1, const Statement& s2);
    bool doDirectChecksPass(const Entity& lhs, const Entity& rhs, const Variable& v);
    bool doDirectTChecksPass(const Entity& lhs, const Entity& rhs);
    bool doRightChecksPass(const Entity& lhs, const Variable& v);
    bool doLeftChecksPass(const Entity& rhs, const std::unordered_set<Variable>& usedVars);

    bool isAssignStmt(const Statement& stmt);
    bool isReadStmt(const Statement& stmt);
    bool isCallStmt(const Statement& stmt);

    bool isVarModifiedDuringExecutionPath(const Statement& neighbour, const Variable& modifiedVar);
    bool isUses(const Statement& s, const Variable& v);

};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_AFFECTSSTORAGE_H_
