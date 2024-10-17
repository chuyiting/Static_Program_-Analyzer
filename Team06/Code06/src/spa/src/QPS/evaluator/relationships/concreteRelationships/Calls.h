#pragma
#include <utility>
#include <memory>

#include "QPS/evaluator/relationships/EntEntRelationship.h"
#include "QPS/evaluator/relationships/Relationship.h"
#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"

class Calls : public EntEntRelationship {
public:
  Calls(EntRef lhs, EntRef rhs);

protected:
  bool isSatisfiedConcrete(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                           std::string rhs) override;

  bool queryLeftDomain(std::shared_ptr<PKBGetter> pkb, std::string rhs,
                       domain_t &result) override;

  bool queryRightDomain(std::shared_ptr<PKBGetter> pkb, std::string lhs,
                        domain_t &result) override;
};