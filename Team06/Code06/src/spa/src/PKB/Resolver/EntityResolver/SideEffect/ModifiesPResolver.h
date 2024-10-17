//
// Created by Stephen Tan  Hin Khai on 7/10/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_MODIFIESPRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_MODIFIESPRESOLVER_H_

#include "PKB/Datastore/ModifiesStorage.h"
#include "PKB/Resolver/EntityResolver/EntityResolver.h"

class ModifiesPResolver : public EntityResolver {

  bool isDirectVariable() override;
  bool isForward() override;

  ModifiesStorage &modifies_storage;

public:
  explicit ModifiesPResolver(ModifiesStorage &modifies_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_MODIFIESPRESOLVER_H_
