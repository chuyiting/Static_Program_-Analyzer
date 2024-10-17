#pragma once

#include "utils/map_utils.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <queue>
#include "utils/map_utils.h"

class CFG {
private:
	std::unordered_map<std::string, std::unordered_set<std::string>> next;
	std::unordered_map<std::string, std::unordered_set<std::string>> nextT;
	std::unordered_map<std::string, std::unordered_set<std::string>> prev;
public:
  // Constructor.
  CFG();
	// Getters.
	std::unordered_map<std::string, std::unordered_set<std::string>> getNextGraph();
	std::unordered_map<std::string, std::unordered_set<std::string>> getNextTGraph();
	std::unordered_map<std::string, std::unordered_set<std::string>> getPrevGraph();

	// Querying.
	void addEdge(const std::string &src, const std::string &dst);
	bool isNodeDirectlyNext(const std::string &src, const std::string &dst);
	bool isNodeIndirectlyNext(const std::string &src, const std::string &dst);
	bool hasNext(const std::string &src);
	bool hasPrev(const std::string &dst);
	bool isAnyNextPresent();

	// Set returns.
	std::unordered_set<std::string> getNextStatements(const std::string &stmt);
	std::unordered_set<std::string> getNextTStatements(const std::string &stmt);
	std::unordered_set<std::string> getPrevStatements(const std::string &stmt);
	std::unordered_set<std::string> getPrevTStatements(const std::string &stmt);

	// Graph traversal.
	void traverseGraphForward(const std::string &stmt, const std::string &src,
		std::unordered_set<std::string> &visitedStmts);
	void traverseGraphBackward(const std::string &stmt, const std::string &src,
		std::unordered_set<std::string> &visitedStmts,
		std::unordered_set<std::string> &outputStmts);
	bool isDstReachableFromSrc(const std::string &src, const std::string &dst);
	void isDstReachableFromSrcHelper(const std::string &stmt, const std::string &src, const std::string &dst,
		std::unordered_set<std::string> &visitedStmts,
		bool &isDstReachable);

	// Methods to get all paths from src to dst.
	std::vector<std::vector<std::string>> getAllPaths(const std::string &src, const std::string &dst);
	void findPaths(const std::string &src, const std::string &dst, std::vector<std::vector<std::string>> &allPaths);
	bool isVisited(const std::string &stmt, std::vector<std::string> &path);

	// Caching.
	void clearCache();
	void cacheNextT(const std::string &lhs, const std::string &rhs);
};