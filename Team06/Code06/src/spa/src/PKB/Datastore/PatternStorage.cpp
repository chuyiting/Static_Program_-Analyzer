#include "PatternStorage.h"
#include "utils/ExpressionHandler/ExpressionHandler.h"
#include "utils/map_utils.h"

// d is the number of characters in the input alphabet
#define d 256

PatternStorage::PatternStorage() {
  modifiesS = std::unordered_map<Statement, Variable>();
  assignmentExpr = std::unordered_map<Statement, std::vector<std::string>>();
  usesIf = std::unordered_map<Statement, std::unordered_set<Variable>>();
  usesWhile = std::unordered_map<Statement, std::unordered_set<Variable>>();
}

/* pat -> pattern
    txt -> text
    Checks whether the pattern is present in the text or not
*/
bool PatternStorage::search(const std::vector<std::string> pat,
                            std::vector<std::string> txt) {
  SubarrayMatching subarrayMatching;
  return subarrayMatching.isSubarray(pat, txt);
}

void PatternStorage::addModifiesS(const Statement &s, const Variable &v) {
  modifiesS.insert({s, v});
}

void PatternStorage::addAssignmentExpr(const Statement &s,
                                       const std::vector<std::string> &e) {
  assignmentExpr.insert({s, e});
}

void PatternStorage::addUsesIf(const Statement &s,
                               const std::unordered_set<Variable> &v) {
  usesIf.insert({s, v});
}

void PatternStorage::addUsesWhile(const Statement &s,
                                  const std::unordered_set<Variable> &v) {
  usesWhile.insert({s, v});
}

bool PatternStorage::matchExpr(const std::vector<std::string> &rhs,
                               const Entity &statement,
                               const ExpressionMatchType &expression) {
  auto expr = assignmentExpr.find(statement);
  // input infix = rhs

  // input postfix
  ExpressionHandler e = ExpressionHandler(ExpressionType::EXPRESSION, rhs);
  e.parse();
  std::vector<std::string> inputPostfix = e.getPostfixNotation();

  // stored infix
  std::vector<std::string> storedInfix = expr->second;

  // stored postfix
  e = ExpressionHandler(ExpressionType::EXPRESSION, storedInfix);
  e.parse();
  std::vector<std::string> storedPostfix = e.getPostfixNotation();

  // remove brackets from input and stored infix expressions
  std::vector<std::string> inputInfix = rhs;
  inputInfix.erase(std::remove(inputInfix.begin(), inputInfix.end(), "("),
                   inputInfix.end());
  inputInfix.erase(std::remove(inputInfix.begin(), inputInfix.end(), ")"),
                   inputInfix.end());

  storedInfix.erase(std::remove(storedInfix.begin(), storedInfix.end(), "("),
                    storedInfix.end());
  storedInfix.erase(std::remove(storedInfix.begin(), storedInfix.end(), ")"),
                    storedInfix.end());

  if (expression == ExpressionMatchType::DIRECT_EXPR) {
    if (inputInfix.size() != storedInfix.size() ||
        inputPostfix.size() != inputInfix.size())
      return false;

    for (int i = 0; i < inputInfix.size(); i++) {
      if (inputInfix.at(i) != storedInfix.at(i))
        return false;
    }

    for (int i = 0; i < inputPostfix.size(); i++) {
      if (inputPostfix.at(i) != storedPostfix.at(i))
        return false;
    }

    return true;
  } else if (expression == ExpressionMatchType::SUB_EXPR) {
    return search(inputInfix, storedInfix) &&
           search(inputPostfix, storedPostfix);
  }

  return false;
}

bool PatternStorage::isDirect(const Entity &lhs,
                              const std::vector<std::string> &rhs,
                              const Entity &statement) {
  return modifiesS.find(statement)->second == lhs &&
         matchExpr(rhs, statement, ExpressionMatchType::DIRECT_EXPR);
}

bool PatternStorage::isDirectAny(const Entity &lhs, const Entity &statement) {
  return modifiesS.find(statement)->second == lhs;
}

bool PatternStorage::isDirectSubtree(const Entity &lhs,
                                     const std::vector<std::string> &rhs,
                                     const Entity &statement) {
  return modifiesS.find(statement)->second == lhs &&
         matchExpr(rhs, statement, ExpressionMatchType::SUB_EXPR);
}

bool PatternStorage::isAnyDirect(const std::vector<std::string> &rhs,
                                 const Entity &statement) {
  return map_utils::isIterableContains(modifiesS, statement) &&
         map_utils::isIterableContains(assignmentExpr, statement) &&
         matchExpr(rhs, statement, ExpressionMatchType::DIRECT_EXPR);
}

bool PatternStorage::isAnyAny(const Entity &statement) {
  return map_utils::isIterableContains(modifiesS, statement) &&
         map_utils::isIterableContains(assignmentExpr, statement);
}

bool PatternStorage::isAnySubtree(const std::vector<std::string> &rhs,
                                  const Entity &statement) {
  return map_utils::isIterableContains(modifiesS, statement) &&
         map_utils::isIterableContains(assignmentExpr, statement) &&
         matchExpr(rhs, statement, ExpressionMatchType::SUB_EXPR);
}

bool PatternStorage::isDirectIf(const Entity &lhs, const Entity &statement) {
  auto varsIf = usesIf.find(statement);
  return varsIf != usesIf.end() &&
         map_utils::isIterableContains(varsIf->second, lhs);
}

bool PatternStorage::isAnyIf(const Entity &statement) {
  return map_utils::isIterableContains(usesIf, statement);
}

bool PatternStorage::isDirectWhile(const Entity &lhs, const Entity &statement) {
  auto varsWhile = usesWhile.find(statement);
  return varsWhile != usesWhile.end() &&
         map_utils::isIterableContains(varsWhile->second, lhs);
}

bool PatternStorage::isAnyWhile(const Entity &statement) {
  return map_utils::isIterableContains(usesWhile, statement);
}

std::unordered_map<Statement, Variable> &PatternStorage::getModifiesS() {
  return modifiesS;
}
std::unordered_map<Statement, std::vector<std::string>> &
PatternStorage::getAssignmentExpr() {
  return assignmentExpr;
}

std::unordered_map<Statement, std::unordered_set<Variable>> &
PatternStorage::getUsesIf() {
  return usesIf;
}

std::unordered_map<Statement, std::unordered_set<Variable>> &
PatternStorage::getUsesWhile() {
  return usesWhile;
}
