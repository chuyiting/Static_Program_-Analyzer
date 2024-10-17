#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_USES_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_USES_RESOLVER_H_

#include "PKB/Datastore/UsesStorage.h"
#include "PKB/Resolver/EntityResolver/EntityResolver.h"

class UsesSResolver : public EntityResolver {

  bool isDirectVariable() override;
  bool isForward() override;

  UsesStorage &uses_storage;

public:
  explicit UsesSResolver(UsesStorage &uses_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_USES_RESOLVER_H_
