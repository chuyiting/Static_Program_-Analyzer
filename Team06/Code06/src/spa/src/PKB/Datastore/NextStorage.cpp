#include "NextStorage.h"

/* ============================= Constructor ============================= */

NextStorage::NextStorage() {
	cfg = CFG();
}

/* ============================= Add next relation ============================= */

void NextStorage::addNext(const Statement &src, const Statement &dst) {
	cfg.addEdge(src, dst);
}

/* ============================= Getter ============================= */

std::unordered_map<Statement, std::unordered_set<Statement>> NextStorage::getNext() {
	return cfg.getNextGraph();
}

/* ============================= Methods for querying ============================= */

bool NextStorage::isNextDirect(const Entity &lhs, const Entity &rhs) {
	return cfg.isNodeDirectlyNext(lhs, rhs);
}

bool NextStorage::isNextTDirect(const Entity &lhs, const Entity &rhs) {
	return cfg.isNodeIndirectlyNext(lhs, rhs);
}

bool NextStorage::isNextRight(const Entity &parent) {
	return cfg.hasNext(parent);
}

bool NextStorage::isNextLeft(const Entity &child) {
	return cfg.hasPrev(child);
}

bool NextStorage::isNextExists() {
	return cfg.isAnyNextPresent();
}

/* ============================= Methods for set returns ============================= */

std::unordered_set<Statement> NextStorage::getNextStatements(const Statement &src) {
	std::unordered_set<Statement> nextStatements = cfg.getNextStatements(src);

	if (map_utils::isIterableContains(nextStatements, "0")) {
		nextStatements.erase("0");
	}

	return nextStatements;
}

std::unordered_set<Statement> NextStorage::getPrevStatements(const Statement &src) {
	return cfg.getPrevStatements(src);
}

std::unordered_set<Statement> NextStorage::getNextTStatements(const Statement &dst) {
	std::unordered_set<Statement> nextTStatements = cfg.getNextTStatements(dst);

	if (map_utils::isIterableContains(nextTStatements, "0")) {
		nextTStatements.erase("0");
	}

	return nextTStatements;
}

std::unordered_set<Statement> NextStorage::getPrevTStatements(const Statement &dst) {
	return cfg.getPrevTStatements(dst);
}

/* ============================= Methods to get all paths from src to dst ============================= */

std::vector<std::vector<Statement>> NextStorage::getAllPathsSrcToDst(const Statement &src, const Statement &dst) {
	return cfg.getAllPaths(src, dst);
}

std::vector<std::vector<Statement>> NextStorage::getAllPathsSrcToEnd(const Statement &src) {
	Statement end = "0";
	return cfg.getAllPaths(src, end);
}

/* ============================= Method to clear cache ============================= */

void NextStorage::clearCache() {
	cfg.clearCache();
}