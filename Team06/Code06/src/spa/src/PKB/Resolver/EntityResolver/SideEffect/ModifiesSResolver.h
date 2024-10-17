#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_MODIFIES_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_MODIFIES_RESOLVER_H_

#include "PKB/Datastore/ModifiesStorage.h"
#include "PKB/Resolver/EntityResolver/EntityResolver.h"

class ModifiesSResolver : public EntityResolver {

  bool isDirectVariable() override;
  bool isForward() override;

  ModifiesStorage &modifies_storage;

public:
  explicit ModifiesSResolver(ModifiesStorage &modifies_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_MODIFIES_RESOLVER_H_
