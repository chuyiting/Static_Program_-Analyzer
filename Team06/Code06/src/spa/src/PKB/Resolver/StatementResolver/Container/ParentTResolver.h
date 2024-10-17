#ifndef SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_PARENTTRESOLVER_H_
#define SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_PARENTTRESOLVER_H_

#include "PKB/Datastore/ParentStorage.h"
#include "PKB/Resolver/StatementResolver/StatementResolver.h"

class ParentTResolver : public StatementResolver {

  bool isDirect() override;
  bool isAnyRight() override;
  bool isAnyLeft() override;
  bool isAny() override;

  ParentStorage &parent_storage;

public:
  explicit ParentTResolver(ParentStorage &parent_storage);
};

#endif // SPA_SRC_SPA_SRC_PKB_RESOLVER_CONTAINER_PARENTTRESOLVER_H_
