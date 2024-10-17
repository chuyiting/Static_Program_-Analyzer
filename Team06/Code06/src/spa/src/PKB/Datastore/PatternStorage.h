#ifndef SPA_SRC_SPA_SRC_PKB_DATASTORE_PATTERNSTORAGE_H_
#define SPA_SRC_SPA_SRC_PKB_DATASTORE_PATTERNSTORAGE_H_

#include "PKB/Resolver/Resolver.h"
#include "PKB/type/pkb_types.h"
#include "utils/SubarrayMatcher/SubarrayMatching.h"
#include "utils/string_utils.h"
#include <unordered_map>
#include <unordered_set>

using namespace pkb_types;

enum ExpressionMatchType { DIRECT_EXPR, SUB_EXPR };

class PatternStorage {
protected:
  std::unordered_map<Statement, Variable> modifiesS;
  std::unordered_map<Statement, std::vector<std::string>> assignmentExpr;
  std::unordered_map<Statement, std::unordered_set<Variable>> usesIf;
  std::unordered_map<Statement, std::unordered_set<Variable>> usesWhile;

public:
  PatternStorage();

  void addModifiesS(const Statement &s, const Variable &v);
  void addAssignmentExpr(const Statement &s, const std::vector<std::string> &e);
  void addUsesIf(const Statement &s, const std::unordered_set<Variable> &v);
  void addUsesWhile(const Statement &s, const std::unordered_set<Variable> &v);

  bool isDirect(const Entity &lhs, const std::vector<std::string> &rhs,
                const Entity &statement);
  bool isDirectAny(const Entity &lhs, const Entity &statement);
  bool isDirectSubtree(const Entity &lhs, const std::vector<std::string> &rhs,
                       const Entity &statement);
  bool isAnyDirect(const std::vector<std::string> &rhs,
                   const Entity &statement);
  bool isAnyAny(const Entity &statement);
  bool isAnySubtree(const std::vector<std::string> &rhs,
                    const Entity &statement);
  bool isDirectIf(const Entity &lhs, const Entity &statement);
  bool isAnyIf(const Entity &statement);
  bool isDirectWhile(const Entity &lhs, const Entity &statement);
  bool isAnyWhile(const Entity &statement);

  std::unordered_map<Statement, Variable> &getModifiesS();
  std::unordered_map<Statement, std::vector<std::string>> &getAssignmentExpr();
  std::unordered_map<Statement, std::unordered_set<Variable>> &getUsesIf();
  std::unordered_map<Statement, std::unordered_set<Variable>> &getUsesWhile();

  bool search(const std::vector<std::string> pat, std::vector<std::string> txt);
  bool matchExpr(const std::vector<std::string> &rhs, const Entity &statement,
                 const ExpressionMatchType &expression);
};

#endif // SPA_SRC_SPA_SRC_PKB_DATASTORE_PATTERNSTORAGE_H_