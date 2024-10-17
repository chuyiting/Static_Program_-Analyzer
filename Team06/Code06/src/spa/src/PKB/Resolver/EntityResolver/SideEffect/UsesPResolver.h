//
// Created by Stephen Tan  Hin Khai on 7/10/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_USESPRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_USESPRESOLVER_H_

#include "PKB/Datastore/UsesStorage.h"
#include "PKB/Resolver/EntityResolver/EntityResolver.h"

class UsesPResolver : public EntityResolver {

  bool isDirectVariable() override;
  bool isForward() override;

  UsesStorage &uses_storage;

public:
  explicit UsesPResolver(UsesStorage &uses_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_SIDEEFFECT_USESPRESOLVER_H_
