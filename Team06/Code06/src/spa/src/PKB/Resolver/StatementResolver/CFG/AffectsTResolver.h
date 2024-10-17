#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_AFFECTST_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_AFFECTST_RESOLVER_H_

#include "PKB/Datastore/AffectsStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class AffectsTResolver : public StatementResolver {

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  AffectsStorage &affects_storage;

public:
  explicit AffectsTResolver(AffectsStorage &affects_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_AFFECTST_RESOLVER_H_