//
// Created by Stephen Tan  Hin Khai on 2/11/22.
//

#ifndef SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_RESOLVER_ENTITYRESOLVER_ENTITYRESOLVER_H_
#define SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_RESOLVER_ENTITYRESOLVER_ENTITYRESOLVER_H_
#include "PKB/Resolver/Resolver.h"

class EntityResolver : public Resolver {
  using fptr = bool (EntityResolver::*)();

protected:
  void assignStrategy() override;
  virtual bool isDirectVariable() = 0;
  virtual bool isForward() = 0;

  std::unordered_map<ResolutionStrategy, fptr> fmap = {
      {ResolutionStrategy::DIRECT_VAR, &EntityResolver::isDirectVariable},
      {ResolutionStrategy::FORWARDS, &EntityResolver::isForward},
  };

public:
  bool run(const Entity &lhs, const Entity &rhs, const Statement &) override;
  EntityResolver();
};

#endif // SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_RESOLVER_ENTITYRESOLVER_ENTITYRESOLVER_H_
