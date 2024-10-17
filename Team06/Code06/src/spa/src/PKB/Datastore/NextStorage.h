#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_NEXTSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_NEXTSTORAGE_H_
#include "PKB/Datastructure/CFG.h"
#include "PKB/type/pkb_types.h"
#include "utils/map_utils.h"
#include <unordered_map>
#include <set>

using namespace pkb_types;
class NextStorage {
private:
  CFG cfg;

public:
    // Constructor.
    NextStorage();

    // Add next relation.
    void addNext(const Statement &src, const Statement &dst);
    
    // Getter.
    std::unordered_map<Statement, std::unordered_set<Statement>> getNext();
    
    // Methods for querying.
    bool isNextDirect(const Entity &lhs, const Entity &rhs);
    bool isNextTDirect(const Entity &lhs, const Entity &rhs);
    bool isNextRight(const Entity &lhs);
    bool isNextLeft(const Entity &rhs);
    bool isNextExists();

    // Methods for set returns
    std::unordered_set<Statement> getNextStatements(const Statement &src);
    std::unordered_set<Statement> getNextTStatements(const Statement &src);
    std::unordered_set<Statement> getPrevStatements(const Statement &dst);
    std::unordered_set<Statement> getPrevTStatements(const Statement &dst);

    // Method to get all paths from src to dst.
    std::vector<std::vector<Statement>> getAllPathsSrcToDst(const Statement &src, const Statement &dst);
    std::vector<std::vector<Statement>> getAllPathsSrcToEnd(const Statement &src);

    // Method to clear cache.
    void clearCache();
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_NEXTSTORAGE_H_
