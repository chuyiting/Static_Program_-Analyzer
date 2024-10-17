//
// Created by Chong Jun Wei on 14/10/22.
//

#ifndef SPA_IFPATTERNRESOLVER_H
#define SPA_IFPATTERNRESOLVER_H

#include "PKB/Datastore/PatternStorage.h"
#include "PKB/Resolver/Resolver.h"

class IfPatternResolver : public Resolver {
  using fptr = bool (IfPatternResolver::*)();

  void assignStrategy() override;
  bool isDirectIf();
  bool isAnyIf();

  std::unordered_map<ResolutionStrategy, fptr> fmap = {
      {ResolutionStrategy::DIRECT_IF, &IfPatternResolver::isDirectIf},
      {ResolutionStrategy::ANY_IF, &IfPatternResolver::isAnyIf},
  };

  PatternStorage &pattern_storage;
  Entity _statement;

public:
  bool run(const Entity &lhs, const Entity &rhs, const Statement &s) override;
  explicit IfPatternResolver(PatternStorage &pattern_storage);
};

#endif // SPA_IFPATTERNRESOLVER_H
