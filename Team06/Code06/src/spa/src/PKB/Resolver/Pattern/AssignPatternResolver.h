#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_PATTERN_PATTERN_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_PATTERN_PATTERN_RESOLVER_H_

#include "PKB/Datastore/PatternStorage.h"
#include "PKB/Resolver/Resolver.h"

class AssignPatternResolver : public Resolver {
  using fptr = bool (AssignPatternResolver::*)();

  void assignStrategy() override;
  bool isDirect();
  bool isDirectAny();
  bool isDirectSubtree();
  bool isAnyDirect();
  bool isAnyAny();
  bool isAnySubtree();

  std::unordered_map<ResolutionStrategy, fptr> fmap = {
      {ResolutionStrategy::DIRECT, &AssignPatternResolver::isDirect},
      {ResolutionStrategy::DIRECT_ANY, &AssignPatternResolver::isDirectAny},
      {ResolutionStrategy::DIRECT_SUBTREE,
       &AssignPatternResolver::isDirectSubtree},
      {ResolutionStrategy::ANY_DIRECT, &AssignPatternResolver::isAnyDirect},
      {ResolutionStrategy::ANY_ANY, &AssignPatternResolver::isAnyAny},
      {ResolutionStrategy::ANY_SUBTREE, &AssignPatternResolver::isAnySubtree}};

  PatternStorage &pattern_storage;
  Entity _statement;
  std::vector<std::string> _rhsArr;

public:
  bool run(const Entity &lhs, const Entity &rhs, const Statement &st) override;
  explicit AssignPatternResolver(PatternStorage &pattern_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_PATTERN_PATTERN_RESOLVER_H_