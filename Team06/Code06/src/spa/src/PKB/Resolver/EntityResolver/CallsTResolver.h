#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_CALLSTRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_CALLSTRESOLVER_H_

#include "PKB/Datastore/CallStorage.h"
#include "PKB/Resolver/Resolver.h"

class CallsTResolver : public Resolver {
  using fptr = bool (CallsTResolver::*)();

  void assignStrategy() override;
  bool isDirect();
  bool isAnyLeft();
  bool isAnyRight();
  bool isAny();

  std::unordered_map<ResolutionStrategy, fptr> fmap = {
      {ResolutionStrategy::DIRECT, &CallsTResolver::isDirect},
      {ResolutionStrategy::ANY_FORWARDS, &CallsTResolver::isAnyRight},
      {ResolutionStrategy::ANY_BACKWARDS, &CallsTResolver::isAnyLeft},
      {ResolutionStrategy::ANY_BIDIRECT, &CallsTResolver::isAny},
  };

  CallStorage &call_storage;

public:
  bool run(const Entity &lhs, const Entity &rhs, const Statement &) override;
  explicit CallsTResolver(CallStorage &call_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_CALLSTRESOLVER_H_
