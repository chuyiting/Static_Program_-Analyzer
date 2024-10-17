#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_NEXT_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_NEXT_RESOLVER_H_

#include "PKB/Datastore/NextStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class NextResolver : public StatementResolver {

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  NextStorage &next_storage;

public:
  explicit NextResolver(NextStorage &next_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_NEXT_RESOLVER_H_