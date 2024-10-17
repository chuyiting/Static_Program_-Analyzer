#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_FOLLOWRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_FOLLOWRESOLVER_H_

#include "PKB/Datastore/FollowStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class FollowResolver : public StatementResolver {

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  FollowStorage &follow_storage;

public:
  explicit FollowResolver(FollowStorage &follow_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_FOLLOWRESOLVER_H_
