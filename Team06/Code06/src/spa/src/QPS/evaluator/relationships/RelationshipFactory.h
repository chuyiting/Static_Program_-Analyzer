//
// Created by Eddy Chu on 2022/9/1.
//

#ifndef SPA_RELATIONSHIPFACTORY_H
#define SPA_RELATIONSHIPFACTORY_H

#include "QPS/evaluator/relationships/concreteRelationships/Follows.h"
#include "QPS/evaluator/relationships/concreteRelationships/ModifiesP.h"
#include "QPS/evaluator/relationships/concreteRelationships/ModifiesS.h"
#include "QPS/evaluator/relationships/concreteRelationships/UsesP.h"
#include "QPS/evaluator/relationships/concreteRelationships/UsesS.h"
#include "QPS/parser/querystructs/suchthatclause/RelRef.h"
#include "QPS/tokenizer/QueryToken.h"
#include "Relationship.h"

class RelationshipFactory {
public:
  static std::shared_ptr<Relationship>
  createRelationship(QueryToken &relTerminal, std::shared_ptr<RelRef> lhs,
                     std::shared_ptr<RelRef> rhs);

private:
  inline static const std::string USES = "Uses";
  inline static const std::string MODIFIES = "Modifies";
  inline static const std::string FOLLOWS = "Follows";
  inline static const std::string FOLLOWS_T = "Follows*";
  inline static const std::string PARENT = "Parent";
  inline static const std::string PARENT_T = "Parent*";
  inline static const std::string CALLS = "Calls";
  inline static const std::string CALLS_T = "Calls*";
  inline static const std::string NEXT = "Next";
  inline static const std::string NEXT_T = "Next*";
  inline static const std::string AFFECTS = "Affects";
  inline static const std::string AFFECTS_T = "Affects*";

  static std::shared_ptr<Relationship> createUses(std::shared_ptr<RelRef> lhs,
                                                  std::shared_ptr<RelRef> rhs);

  static std::shared_ptr<Relationship>
  createModifies(std::shared_ptr<RelRef> lhs, std::shared_ptr<RelRef> rhs);

  static std::shared_ptr<Relationship> createCalls(const std::string &rel,
                                                   std::shared_ptr<RelRef> lhs,
                                                   std::shared_ptr<RelRef> rhs);

  static void
  checkEntRefSynonymType(EntRef &ent,
                         std::unordered_set<EntityType> expectedTypes);
};

#endif // SPA_RELATIONSHIPFACTORY_H
