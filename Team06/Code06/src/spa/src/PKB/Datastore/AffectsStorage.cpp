#include "AffectsStorage.h"

using pair = std::pair<std::string, std::string>;

/* ============================= Constructor ============================= */

AffectsStorage::AffectsStorage(NextStorage &_next_storage,
                               ModifiesStorage &_modifies_storage,
                               UsesStorage &_uses_storage,
                               SynonymStorage &_synonym_storage)
    : _next_storage(_next_storage), _modifies_storage(_modifies_storage),
      _uses_storage(_uses_storage), _synonym_storage(_synonym_storage) {}

/* ============================= Querying ============================= */

bool AffectsStorage::isAffectsDirect(const Entity &lhs, const Entity &rhs) {
  // Try to get cached result from Affects
  bool isLhsInAffects = map_utils::isIterableContains(affects, lhs);
  bool isRhsInAffectsLhs = map_utils::isIterableContains(affects[lhs], rhs);
  if (isLhsInAffects && isRhsInAffectsLhs) {
    return true;
  }

  // Initialize variables
  Variable modifiedVar = getModifiedVariable(lhs);

  // Perform basic checks
  if (!doDirectChecksPass(lhs, rhs, modifiedVar)) {
    return false;
  }

  // Calculate isAffectsDirect
  return isBfsAffectsDirect(lhs, rhs, modifiedVar);
}

bool AffectsStorage::isAffectsRight(const Entity &lhs) {
    // Try to get cached result from Affects
    bool isLhsInAffects = map_utils::isIterableContains(affects, lhs);
    if (isLhsInAffects) {
        return true;
    }

  // Initialize variable
  Variable modifiedVar = getModifiedVariable(lhs);

  // Perform basic checks
  if (!doRightChecksPass(lhs, modifiedVar)) {
    return false;
  }

  // Calculate isAffectsRight
  return isBfsAffectsRight(lhs, modifiedVar);
}

bool AffectsStorage::isAffectsLeft(const Entity &rhs) {
    // Try to get cached result from Affects
    for (auto const& [key, val] : affects) {
        if (map_utils::isIterableContains(val, rhs)) {
            return true;
        }
    }

    // Initialize variables
    std::unordered_set<Variable> usedVars = _uses_storage.getStatementUses(rhs);

    // Perform basic checks
    if (!doLeftChecksPass(rhs, usedVars)) {
        return false;
    }

    // Calculate isAffectsLeft
    return isBfsAffectsLeft(rhs, usedVars);
}

bool AffectsStorage::isAffectsTDirect(const Entity &lhs, const Entity &rhs) {
    // Try to get cached result from Affects
    bool isLhsInAffects = map_utils::isIterableContains(affects, lhs);
    bool isRhsInAffectsLhs = map_utils::isIterableContains(affects[lhs], rhs);
    if (isLhsInAffects && isRhsInAffectsLhs) {
        return true;
    }

  // Try to get cached result from AffectsT
  bool isLhsInAffectsT = map_utils::isIterableContains(affectsT, lhs);
  bool isRhsInAffectsTLhs = map_utils::isIterableContains(affectsT[lhs], rhs);
  if (isLhsInAffectsT && isRhsInAffectsTLhs) {
    return true;
  }

  // Perform basic checks
  if (!doDirectTChecksPass(lhs, rhs)) {
    return false;
  }

  // Calculate isAffectsT
  return isBfsAffectsTDirect(lhs, rhs);
}

bool AffectsStorage::isAffectsExists() {
  std::unordered_set<Statement> assignStmts = _synonym_storage.getAssignStmts();
  for (Statement const &s : assignStmts) {
    if (isAffectsRight(s)) {
      return true;
    }
  }

  return false;
}

/* ============================= Set returns ============================= */

// return every stmts affected by lhs
std::unordered_set<Statement> AffectsStorage::getAffects(const Entity &lhs) {
  // Initialize variable
  Variable modifiedVar = getModifiedVariable(lhs);

  // Perform basic checks
  if (!doRightChecksPass(lhs, modifiedVar)) {
    return std::unordered_set<Statement>();
  }

  // Get AffectsRight
  return bfsAffectsRight(lhs, modifiedVar);
}

std::unordered_set<Statement> AffectsStorage::getAffectedBy(const Entity &rhs) {
    // Initialize variables
    std::unordered_set<Statement> stmtsAffected;
    std::unordered_set<Variable> usedVars = _uses_storage.getStatementUses(rhs);

    // Perform basic checks
    if (!doLeftChecksPass(rhs, usedVars)) {
        return stmtsAffected;
    }

  // Get AffectsLeft
  std::set<Statement> assignStmtsInProcedure = getAssignStmtsInProcedure(rhs);
  for (Statement const &lhs : assignStmtsInProcedure) {
    if (isAffectsDirect(lhs, rhs)) {
      stmtsAffected.insert(lhs);
    }
  }
  return stmtsAffected;
}

std::unordered_set<Statement> AffectsStorage::getAffectsT(const Entity &lhs) {
    // Initialize variable
    Variable modifiedVar = getModifiedVariable(lhs);

    // Perform basic checks
    if (!doRightChecksPass(lhs, modifiedVar)) {
        return std::unordered_set<Statement>();
    }

    // Get AffectsTRight
    return bfsAffectsTRight(lhs);
}

std::unordered_set<Statement>
AffectsStorage::getAffectedByT(const Entity &rhs) {
    // Initialize variables
    std::unordered_set<Statement> stmtsAffected;
    std::unordered_set<Variable> usedVars = _uses_storage.getStatementUses(rhs);

    // Perform basic checks
    if (!doLeftChecksPass(rhs, usedVars)) {
        return stmtsAffected;
    }

    // Get AffectsTLeft
    std::set<Statement> assignStmtsInProcedure = getAssignStmtsInProcedure(rhs);
    for (Statement const& lhs : assignStmtsInProcedure) {
        if (isAffectsTDirect(lhs, rhs)) {
            stmtsAffected.insert(lhs);
        }
    }
    return stmtsAffected;
}

/* ============================= Graph traversal =============================
 */

bool AffectsStorage::isBfsAffectsDirect(const Statement& src,
    const Statement& dst,
    const Variable& modifiedVar) {
    std::unordered_set<std::string> visited;
    std::queue<std::string> queue;

    visited.insert(src);
    queue.push(src);

    while (!queue.empty()) {
        std::string vertex = queue.front();
        queue.pop();

        std::unordered_set<std::string> neighbours =
            _next_storage.getNextStatements(vertex);

        if (neighbours.empty()) {
            continue;
        }

        for (std::string neighbour : neighbours) {
            if (neighbour == dst) {
                cacheAffects(src, dst);
                return true;
            }

            if (map_utils::isIterableContains(visited, neighbour)) {
                continue;
            }

            if (isVarModifiedDuringExecutionPath(neighbour, modifiedVar)) {
                continue;
            }

            visited.insert(neighbour);
            queue.push(neighbour);
        }
    }

    return false;
}

bool AffectsStorage::isBfsAffectsRight(const Statement& src,
    const Variable& modifiedVar) {
    std::unordered_set<std::string> visited;
    std::queue<std::string> queue;

    visited.insert(src);
    queue.push(src);

    while (!queue.empty()) {
        std::string vertex = queue.front();
        queue.pop();

        std::unordered_set<std::string> neighbours =
            _next_storage.getNextStatements(vertex);

        if (neighbours.empty()) {
            continue;
        }

        for (std::string neighbour : neighbours) {
            if (isAssignStmt(neighbour) && isUses(neighbour, modifiedVar)) {
                cacheAffects(src, neighbour);
                return true;
            }

            if (map_utils::isIterableContains(visited, neighbour)) {
                continue;
            }

            if (isVarModifiedDuringExecutionPath(neighbour, modifiedVar)) {
                continue;
            }

            visited.insert(neighbour);
            queue.push(neighbour);
        }
    }

    return false;
}

bool AffectsStorage::isBfsAffectsLeft(const Statement& src, const std::unordered_set<Statement>& usedVars) {
    std::unordered_set<std::string> visited;
    std::queue<std::string> queue;

    visited.insert(src);
    queue.push(src);

    while (!queue.empty()) {
        std::string vertex = queue.front();
        queue.pop();

        std::unordered_set<std::string> neighbours =
            _next_storage.getPrevStatements(vertex);

        if (neighbours.empty()) {
            continue;
        }

        for (std::string neighbour : neighbours) {
            if (isAssignStmt(neighbour) &&
                map_utils::isIterableContains(usedVars, getModifiedVariable(neighbour))) {
                cacheAffects(neighbour, src);
                return true;
            }

            if (map_utils::isIterableContains(visited, neighbour)) {
                continue;
            }

            visited.insert(neighbour);
            queue.push(neighbour);
        }
    }

    return false;
}

bool AffectsStorage::isBfsAffectsTDirect(const Statement &src,
                                         const Statement &dst) {
  std::unordered_map<std::string, std::unordered_set<std::string>> visited;

  std::queue<std::pair<std::string, std::string>> queue;
  queue.push(pair({ src, getModifiedVariable(src) }));

  while (!queue.empty()) {
    std::pair<std::string, std::string> currPair = queue.front();
    std::string vertex = currPair.first;
    std::string modifiedVar = currPair.second;
    queue.pop();

    if (!_next_storage.isNextRight(vertex)) {
      continue;
    }

    bool isModifiedVarVisited =
        map_utils::isIterableContains(visited, modifiedVar);
    bool isModifiedVarVisitedInCurrStmt =
        map_utils::isIterableContains(visited[modifiedVar], vertex);
    if (isModifiedVarVisited && isModifiedVarVisitedInCurrStmt) {
      continue;
    }

    visited[modifiedVar].insert(vertex);

    std::unordered_set<std::string> neighbours =
        _next_storage.getNextStatements(vertex);

    for (Statement const &neighbour : neighbours) {
      if (neighbour == dst && isUses(dst, modifiedVar)) {
        cacheAffectsT(src, dst);
        return true;
      }

      if (isAssignStmt(neighbour) && isUses(neighbour, modifiedVar)) {
        queue.push(pair({ neighbour, getModifiedVariable(neighbour) }));
      }

      if (isVarModifiedDuringExecutionPath(neighbour, modifiedVar)) {
        continue;
      }

      queue.push(pair({ neighbour, modifiedVar }));
    }
  }
  return false;
}

std::unordered_set<std::string>
AffectsStorage::bfsAffectsRight(const Statement& src,
    const Variable& modifiedVar) {
    std::unordered_set<std::string> affectedStmts;
    std::unordered_set<std::string> visited;
    std::queue<std::string> queue;

    visited.insert(src);
    queue.push(src);

    while (!queue.empty()) {
        std::string vertex = queue.front();
        queue.pop();

        std::unordered_set<std::string> neighbours =
            _next_storage.getNextStatements(vertex);

        if (neighbours.empty()) {
            continue;
        }

        for (std::string neighbour : neighbours) {
            if (isAssignStmt(neighbour) && isUses(neighbour, modifiedVar)) {
                cacheAffects(src, neighbour);
                affectedStmts.insert(neighbour);
            }

            if (map_utils::isIterableContains(visited, neighbour)) {
                continue;
            }

            if (isVarModifiedDuringExecutionPath(neighbour, modifiedVar)) {
                continue;
            }

            visited.insert(neighbour);
            queue.push(neighbour);
        }
    }

    return affectedStmts;
}

std::unordered_set<std::string>
AffectsStorage::bfsAffectsTRight(const Statement &src) {
  std::unordered_set<std::string> affectedStmtsT;
  std::unordered_map<std::string, std::unordered_set<std::string>> visited;

  std::queue<std::pair<std::string, std::string>> queue;
  queue.push(pair({ src, getModifiedVariable(src) }));

  while (!queue.empty()) {
    std::pair<std::string, std::string> currPair = queue.front();
    std::string vertex = currPair.first;
    std::string modifiedVar = currPair.second;
    queue.pop();

    if (!_next_storage.isNextRight(vertex)) {
      continue;
    }

    bool isModifiedVarVisited =
        map_utils::isIterableContains(visited, modifiedVar);
    bool isModifiedVarVisitedInCurrStmt =
        map_utils::isIterableContains(visited[modifiedVar], vertex);
    if (isModifiedVarVisited && isModifiedVarVisitedInCurrStmt) {
      continue;
    }

    visited[modifiedVar].insert(vertex);

    std::unordered_set<std::string> neighbours =
        _next_storage.getNextStatements(vertex);

    for (Statement const &neighbour : neighbours) {
      if (isAssignStmt(neighbour) && isUses(neighbour, modifiedVar)) {
        queue.push(pair({ neighbour, getModifiedVariable(neighbour) }));
        cacheAffects(src, neighbour);
        affectedStmtsT.insert(neighbour);
      }

      queue.push(pair({ neighbour, modifiedVar }));
    }
  }
  return affectedStmtsT;
}

/* ============================= Caching ============================= */

void AffectsStorage::clearCache() {
  affects.clear();
  affectsT.clear();
}

void AffectsStorage::cacheAffects(const Entity& lhs, const Entity& rhs) {
    affects[lhs].insert(rhs);
}

void AffectsStorage::cacheAffectsT(const Entity& lhs, const Entity& rhs) {
    affectsT[lhs].insert(rhs);
}

/* ============================= Helper methods ============================= */

Variable AffectsStorage::getModifiedVariable(const Entity &lhs) {
  std::unordered_set<Variable> vars =
      _modifies_storage.getStatementModifies(lhs);
  if (vars.size() != 1) {
    return "";
  }
  return *vars.begin();
}

Procedure AffectsStorage::getProcedure(const Statement &stmt) {
  std::unordered_map<Procedure, std::unordered_set<Statement>> procedures =
      _synonym_storage.getProcedureStatements();
  for (auto const &x : procedures) {
    Procedure p = x.first;
    for (auto const &y : x.second) {
      Statement s = y.c_str();
      if (s == stmt) {
        return p;
      }
    }
  }
  return "";
}

std::set<Statement>
AffectsStorage::getAssignStmtsInProcedure(const Statement &stmt) {
  std::set<Statement> assignStmtsInProcedure;
  Procedure procedure = getProcedure(stmt);
  std::unordered_map<Procedure, std::unordered_set<Statement>> procedures =
      _synonym_storage.getProcedureStatements();
  std::unordered_set<Statement> stmtsInProcedure = procedures[procedure];
  for (Statement const &s : stmtsInProcedure) {
    if (isAssignStmt(s)) {
      assignStmtsInProcedure.insert(s);
    }
  }
  return assignStmtsInProcedure;
}

bool AffectsStorage::doDirectChecksPass(const Entity &lhs, const Entity &rhs,
                                        const Variable &v) {
  // Check if they are assign statements
  if (!isAssignStmt(lhs) || !isAssignStmt(rhs)) {
    return false;
  }

  // Check if they are in the same procedure
  if (!isInSameProcedure(lhs, rhs)) {
    return false;
  }

  // Check if the variable modified in lhs is used by rhs
  if (v.empty() || !isUses(rhs, v)) {
    return false;
  }

  // Check if lhs leads to any statement
  if (!_next_storage.isNextRight(lhs)) {
    return false;
  }

  // Check if any statement leads to rhs
  if (!_next_storage.isNextLeft(rhs)) {
    return false;
  }

  return true;
}

bool AffectsStorage::doDirectTChecksPass(const Entity &lhs, const Entity &rhs) {
  // Check if they are assign statements
  if (!isAssignStmt(lhs) || !isAssignStmt(rhs)) {
    return false;
  }

  // Check if they are in the same procedure
  if (!isInSameProcedure(lhs, rhs)) {
    return false;
  }

  // Check if lhs modifies a variable
  Variable modifiedVar = getModifiedVariable(lhs);
  if (modifiedVar.empty()) {
    return false;
  }

  // Check if rhs uses at least one variable
  std::unordered_set<Variable> usedVars = _uses_storage.getStatementUses(rhs);
  if (usedVars.empty()) {
    return false;
  }

  // Check if lhs leads to any statement
  if (!_next_storage.isNextRight(lhs)) {
    return false;
  }

  // Check if any statement leads to rhs
  if (!_next_storage.isNextLeft(rhs)) {
    return false;
  }

  return true;
}

bool AffectsStorage::doRightChecksPass(const Entity &lhs, const Variable &v) {
  // Check if lhs is an assign statement
  if (!isAssignStmt(lhs)) {
    return false;
  }

  // Check if lhs modifies a variable
  if (v.empty()) {
    return false;
  }

  // Check if lhs leads to any statement
  if (!_next_storage.isNextRight(lhs)) {
    return false;
  }

  return true;
}

bool AffectsStorage::doLeftChecksPass(const Entity &rhs, const std::unordered_set<Variable> &usedVars) {
  // Check if rhs is an assign statement
  if (!isAssignStmt(rhs)) {
    return false;
  }

  // Check if rhs uses at least one variable
  if (usedVars.empty()) {
    return false;
  }

  // Check if any statement leads to rhs
  if (!_next_storage.isNextLeft(rhs)) {
    return false;
  }

  return true;
}

bool AffectsStorage::isInSameProcedure(const Statement &s1,
                                       const Statement &s2) {
  Procedure p1 = getProcedure(s1);
  Procedure p2 = getProcedure(s2);
  if (p1 == "" || p2 == "") {
    return false;
  }
  return p1 == p2;
}

bool AffectsStorage::isAssignStmt(const Statement &stmt) {
  return map_utils::isIterableContains(_synonym_storage.getAssignStmts(), stmt);
}

bool AffectsStorage::isReadStmt(const Statement &stmt) {
  return map_utils::isIterableContains(_synonym_storage.getReadStmts(), stmt);
}

bool AffectsStorage::isCallStmt(const Statement &stmt) {
  return map_utils::isIterableContains(_synonym_storage.getCallStmts(), stmt);
}

bool AffectsStorage::isVarModifiedDuringExecutionPath(const Statement& neighbour, const Variable& modifiedVar) {
    bool isAssignReadCallStmt = isAssignStmt(neighbour) || isReadStmt(neighbour) ||
        isCallStmt(neighbour);
    bool isVarModified = _modifies_storage.isModifiesS(neighbour, modifiedVar);
    return isAssignReadCallStmt || isVarModified;
}

bool AffectsStorage::isUses(const Statement& s, const Variable& v) {
    return _uses_storage.isUsesS(s, v);
}
