//
// Created by Eddy Chu on 2022/9/1.
//

#ifndef INC_22S1_CP_SPA_TEAM_06_USESS_H
#define INC_22S1_CP_SPA_TEAM_06_USESS_H

#include "QPS/evaluator/relationships/StmtEntRelationship.h"
#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"
#include "QPS/evaluator/relationships/relationshipStructs/StmtRef.h"
#include <memory>
#include <string>

class UsesS : public StmtEntRelationship {
public:
  UsesS(StmtRef lhs, EntRef rhs);

protected:
  bool isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                           std::string rhs) override;

  bool queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                       domain_t &result) override;

  bool queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                        domain_t &result) override;
};

#endif // INC_22S1_CP_SPA_TEAM_06_USESS_H
