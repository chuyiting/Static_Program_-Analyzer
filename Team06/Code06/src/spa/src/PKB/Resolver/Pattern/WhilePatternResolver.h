//
// Created by Chong Jun Wei on 14/10/22.
//

#ifndef SPA_WHILEPATTERNRESOLVER_H
#define SPA_WHILEPATTERNRESOLVER_H

#include "PKB/Datastore/PatternStorage.h"
#include "PKB/Resolver/Resolver.h"

class WhilePatternResolver : public Resolver {
  using fptr = bool (WhilePatternResolver::*)();

  void assignStrategy() override;
  bool isDirectWhile();
  bool isAnyWhile();

  std::unordered_map<ResolutionStrategy, fptr> fmap = {
      {ResolutionStrategy::DIRECT_WHILE, &WhilePatternResolver::isDirectWhile},
      {ResolutionStrategy::ANY_WHILE, &WhilePatternResolver::isAnyWhile},
  };

  PatternStorage &pattern_storage;
  Entity _statement;

public:
  bool run(const Entity &lhs, const Entity &rhs, const Statement &s) override;
  explicit WhilePatternResolver(PatternStorage &pattern_storage);
};

#endif // SPA_WHILEPATTERNRESOLVER_H
