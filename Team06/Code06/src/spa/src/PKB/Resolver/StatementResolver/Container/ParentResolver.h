#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_PARENTRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_PARENTRESOLVER_H_

#include "PKB/Datastore/ParentStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class ParentResolver : public StatementResolver {

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  ParentStorage &parent_storage;

public:
  explicit ParentResolver(ParentStorage &parent_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_PARENTRESOLVER_H_
