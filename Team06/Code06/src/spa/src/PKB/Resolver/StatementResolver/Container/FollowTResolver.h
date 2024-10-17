#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_FOLLOWTRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_FOLLOWTRESOLVER_H_

#include "PKB/Datastore/FollowStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class FollowTResolver : public StatementResolver {
  using fptr = bool (FollowTResolver::*)();

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  FollowStorage &follow_storage;

public:
  explicit FollowTResolver(FollowStorage &follow_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_FOLLOWTRESOLVER_H_
