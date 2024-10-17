//
// Created by Stephen Tan  Hin Khai on 2/11/22.
//

#ifndef SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_RESOLVER_STATEMENTRESOLVER_STATEMENTRESOLVER_H_
#define SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_RESOLVER_STATEMENTRESOLVER_STATEMENTRESOLVER_H_
#include "PKB/Resolver/Resolver.h"

class StatementResolver : public Resolver {
  using fptr = bool (StatementResolver::*)();

protected:
  void assignStrategy() override;
  virtual bool isDirect() = 0;
  virtual bool isAnyRight() = 0;
  virtual bool isAnyLeft() = 0;
  virtual bool isAny() = 0;

  std::unordered_map<ResolutionStrategy, fptr> fmap = {
      {ResolutionStrategy::DIRECT, &StatementResolver::isDirect},
      {ResolutionStrategy::ANY_FORWARDS, &StatementResolver::isAnyRight},
      {ResolutionStrategy::ANY_BACKWARDS, &StatementResolver::isAnyLeft},
      {ResolutionStrategy::ANY_BIDIRECT, &StatementResolver::isAny},
  };

public:
  bool run(const Entity &lhs, const Entity &rhs, const Statement &) override;
  StatementResolver();
};

#endif // SPA_TEAM06_CODE06_SRC_SPA_SRC_PKB_RESOLVER_STATEMENTRESOLVER_STATEMENTRESOLVER_H_
