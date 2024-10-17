#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_NEXTT_RESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_NEXTT_RESOLVER_H_

#include "PKB/Datastore/NextStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class NextTResolver : public StatementResolver {

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  NextStorage &next_storage;

public:
  explicit NextTResolver(NextStorage &next_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CFG_NEXTT_RESOLVER_H_