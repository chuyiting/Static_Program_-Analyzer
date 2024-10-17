#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_CALLSRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_CALLSRESOLVER_H_

#include "PKB/Datastore/CallStorage.h"
#include "PKB/Resolver/Resolver.h"

class CallsResolver : public Resolver {
  using fptr = bool (CallsResolver::*)();

  void assignStrategy() override;
  bool isDirect();
  bool isAnyLeft();
  bool isAnyRight();
  bool isAny();

  std::unordered_map<ResolutionStrategy, fptr> fmap = {
      {ResolutionStrategy::DIRECT, &CallsResolver::isDirect},
      {ResolutionStrategy::ANY_FORWARDS, &CallsResolver::isAnyRight},
      {ResolutionStrategy::ANY_BACKWARDS, &CallsResolver::isAnyLeft},
      {ResolutionStrategy::ANY_BIDIRECT, &CallsResolver::isAny},
  };

  CallStorage &call_storage;

public:
  bool run(const Entity &lhs, const Entity &rhs, const Statement &) override;
  explicit CallsResolver(CallStorage &call_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_CALLSRESOLVER_H_
