#include "CFG.h"

/* ============================= Constructor ============================= */

CFG::CFG() {
    next = std::unordered_map<std::string, std::unordered_set<std::string>>();
    nextT = std::unordered_map<std::string, std::unordered_set<std::string>>();
	prev = std::unordered_map<std::string, std::unordered_set<std::string>>();
}

/* ============================= Getters ============================= */

std::unordered_map<std::string, std::unordered_set<std::string>> CFG::getNextGraph() {
    return next;
}

std::unordered_map<std::string, std::unordered_set<std::string>> CFG::getNextTGraph() {
    return nextT;
}

std::unordered_map<std::string, std::unordered_set<std::string>> CFG::getPrevGraph() {
    return prev;
}

/* ============================= Querying ============================= */

void CFG::addEdge(const std::string &src, const std::string &dst) {
    // Populate prev graph
    if (!map_utils::isIterableContains(prev, dst)) {
        prev.insert({ dst, {src} });
    }
    else {
        prev[dst].insert(src);
    }
    // Populate next graph
    if (!map_utils::isIterableContains(next, src)) {
        next.insert({ src, {dst} });
    }
    else {
        next[src].insert(dst);
    }
}

bool CFG::isNodeDirectlyNext(const std::string &src, const std::string &dst) {
    return map_utils::isIterableContains(next[src], dst) && dst != "0";
}

bool CFG::isNodeIndirectlyNext(const std::string &src, const std::string &dst) {
    // Try to get cached result from Next
    bool isLhsInNext = map_utils::isIterableContains(next, src);
    bool isRhsInNextLhs = map_utils::isIterableContains(next[src], dst);
    if (isLhsInNext && isRhsInNextLhs) {
        cacheNextT(src, dst);
        return true;
    }

    // Try to get cached result NextT
    bool isLhsInNextT = map_utils::isIterableContains(nextT, src);
    bool isRhsInNextTLhs = map_utils::isIterableContains(nextT[src], dst);
    if (isLhsInNextT && isRhsInNextTLhs) {
        return true;
    }

    // Calculate isNextT
    std::unordered_set<std::string> visitedStmts;
    bool isNextT = isDstReachableFromSrc(src, dst);
    if (isNextT) {
        cacheNextT(src, dst);
    }
    return isNextT;
}

bool CFG::hasNext(const std::string &src) {
    if (!map_utils::isIterableContains(next, src)) {
        return false;
    }

    if (next[src].size() > 1) {
        return true;
    }

    if (map_utils::isIterableContains(next[src], "0")) {
        return false;
    }

  return true;
}

bool CFG::hasPrev(const std::string &dst) {
    return map_utils::isIterableContains(prev, dst) && prev[dst].size() > 0 && dst != "0";
}

bool CFG::isAnyNextPresent() {
    for (auto const &x : next) {
        std::string src = x.first;
        if (hasNext(src)) {
            return true;
        }
    }
  return false;
}

/* ============================= Set returns ============================= */

std::unordered_set<std::string> CFG::getNextStatements(const std::string &stmt) {
    if (!map_utils::isIterableContains(next, stmt)) {
        return std::unordered_set<std::string>();
    }

    return next[stmt];
}

std::unordered_set<std::string> CFG::getPrevStatements(const std::string &stmt) {
    if (!map_utils::isIterableContains(prev, stmt)) {
        return std::unordered_set<std::string>();
    }
    return prev[stmt];
}

std::unordered_set<std::string> CFG::getNextTStatements(const std::string &stmt) {
    std::unordered_set<std::string> visitedStmts;
    traverseGraphForward(stmt, stmt, visitedStmts);

    if (!map_utils::isIterableContains(nextT, stmt)) {
        return std::unordered_set<std::string>();
    }

    return nextT[stmt];
}

std::unordered_set<std::string> CFG::getPrevTStatements(const std::string &stmt) {
    std::unordered_set<std::string> visitedStmts;
    std::unordered_set<std::string> prevStmtsT;
    traverseGraphBackward(stmt, stmt, visitedStmts, prevStmtsT);
    return prevStmtsT;
}

/* ============================= Graph traversal ============================= */

void CFG::traverseGraphForward(const std::string &stmt, const std::string &src,
    std::unordered_set<std::string> &visitedStmts) {
    // Mark statement as visited.
    visitedStmts.insert(stmt);

    // Get neighbours.
    std::unordered_set<std::string> neighbours = getNextStatements(stmt);

    // For each neighbour.
    for (std::string const &neighbour : neighbours) {
        // Perform caching.
        cacheNextT(src, neighbour);

        // Visit neighbour.
        if (!map_utils::isIterableContains(visitedStmts, neighbour)) {
            traverseGraphForward(neighbour, src, visitedStmts);
        }
    }
}

void CFG::traverseGraphBackward(const std::string &stmt, const std::string &src,
    std::unordered_set<std::string> &visitedStmts, std::unordered_set<std::string> &outputStmts) {
    // Mark statement as visited.
    visitedStmts.insert(stmt);

    // Get neighbours.
    std::unordered_set<std::string> neighbours = getPrevStatements(stmt);

    // For each neighbour.
    for (std::string const &neighbour : neighbours) {
        // Add neighbour to the output.
        outputStmts.insert(neighbour);

        // Perform caching.
        cacheNextT(neighbour, src);

        // Visit neighbour.
        if (!map_utils::isIterableContains(visitedStmts, neighbour)) {
            traverseGraphBackward(neighbour, src, visitedStmts, outputStmts);
        }
    }
}

bool CFG::isDstReachableFromSrc(const std::string& src, const std::string& dst) {
    bool isDstReachable = false;
    std::unordered_set<std::string> visitedStmts;
    isDstReachableFromSrcHelper(src, src, dst, visitedStmts, isDstReachable);
    return isDstReachable;
}

void CFG::isDstReachableFromSrcHelper(const std::string &stmt, const std::string &src, const std::string &dst,
    std::unordered_set<std::string> &visitedStmts, bool &isDstReachable) {
    // Mark statement as visited.
    visitedStmts.insert(stmt);

    // Get neighbours.
    std::unordered_set<std::string> neighbours = getNextStatements(stmt);

    // For each neighbour.
    for (std::string const &neighbour : neighbours) {
        // Check reachability.
        if (neighbour == dst) {
            isDstReachable = true;
            return;
        }

        // Perform caching.
        cacheNextT(src, neighbour);

        // Visit neighbour.
        if (!map_utils::isIterableContains(visitedStmts, neighbour)) {
            isDstReachableFromSrcHelper(neighbour, src, dst, visitedStmts, isDstReachable);
        }
    }
}

/* ============================= Methods to get all paths from src to dst ============================= */

std::vector<std::vector<std::string>> CFG::getAllPaths(const std::string &src, const std::string &dst) {
    std::vector<std::vector<std::string>> allPaths;
    findPaths(src, dst, allPaths);
    return allPaths;
}

void CFG::findPaths(const std::string &src, const std::string &dst, std::vector<std::vector<std::string>> &allPaths) {
    std::queue<std::vector<std::string>> queue;
    std::vector<std::string> path;

    path.push_back(src);
    queue.push(path);

    while (!queue.empty()) {
        path = queue.front();
        queue.pop();
        std::string last = path.back();

        if (last == dst) {
            allPaths.push_back(path);
        }

        for (std::string const &neighbour : next[last]) {
            if (!isVisited(neighbour, path)) {
                std::vector<std::string> newPath(path);
                newPath.push_back(neighbour);
                queue.push(newPath);
            }
        }
    }
}

bool CFG::isVisited(const std::string &stmt, std::vector<std::string> &path) {
    int count = 0;
    for (std::string const &s : path) {
        if (s == stmt) {
            count++;
        }
    }
    if (count > 1) {
        return true;
    }
    return false;
}

/* ============================= Caching ============================= */

void CFG::clearCache() {
    nextT.clear();
}

void CFG::cacheNextT(const std::string &lhs, const std::string &rhs) {
    nextT[lhs].insert(rhs);
}
