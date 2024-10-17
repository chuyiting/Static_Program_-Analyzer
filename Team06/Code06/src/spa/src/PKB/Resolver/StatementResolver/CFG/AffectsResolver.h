#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_AFFECTS_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_AFFECTS_RESOLVER_H_

#include "PKB/Datastore/AffectsStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class AffectsResolver : public StatementResolver {

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  AffectsStorage &affects_storage;

public:
  explicit AffectsResolver(AffectsStorage &affects_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_AFFECTS_RESOLVER_H_