//
// Created by Eddy Chu on 2022/9/1.
//

#include "RelationshipFactory.h"
#include "QPS/evaluator/relationships/concreteRelationships/Affects.h"
#include "QPS/evaluator/relationships/concreteRelationships/AffectsT.h"
#include "QPS/evaluator/relationships/concreteRelationships/Calls.h"
#include "QPS/evaluator/relationships/concreteRelationships/CallsT.h"
#include "QPS/evaluator/relationships/concreteRelationships/FollowsT.h"
#include "QPS/evaluator/relationships/concreteRelationships/Next.h"
#include "QPS/evaluator/relationships/concreteRelationships/NextT.h"
#include "QPS/evaluator/relationships/concreteRelationships/Parent.h"
#include "QPS/evaluator/relationships/concreteRelationships/ParentT.h"
#include "QPS/exceptions/QPSException.h"

void RelationshipFactory::checkEntRefSynonymType(
    EntRef &ent, std::unordered_set<EntityType> expectedTypes) {
  if (!ent.isSynonym())
    return;

  EntityType type = ent.getSynonym().getSynonymType();
  if (expectedTypes.find(type) == expectedTypes.end()) {
    throw QPSException("", QpsExceptionType::SEMANTIC);
  }
}

std::shared_ptr<Relationship>
RelationshipFactory::createRelationship(QueryToken &relTerminal,
                                        std::shared_ptr<RelRef> lhs,
                                        std::shared_ptr<RelRef> rhs) {
  if (relTerminal.getValue() == USES) {
    return createUses(lhs, rhs);
  } else if (relTerminal.getValue() == MODIFIES) {
    return createModifies(lhs, rhs);
  } else if (relTerminal.getValue() == FOLLOWS) {
    return std::make_shared<Follows>(StmtRef(lhs), StmtRef(rhs));
  } else if (relTerminal.getValue() == FOLLOWS_T) {
    return std::make_shared<FollowsT>(StmtRef(lhs), StmtRef(rhs));
  } else if (relTerminal.getValue() == PARENT) {
    return std::make_shared<Parent>(StmtRef(lhs), StmtRef(rhs));
  } else if (relTerminal.getValue() == PARENT_T) {
    return std::make_shared<ParentT>(StmtRef(lhs), StmtRef(rhs));
  } else if (relTerminal.getValue() == CALLS ||
             relTerminal.getValue() == CALLS_T) {
    return createCalls(relTerminal.getValue(), lhs, rhs);
  } else if (relTerminal.getValue() == NEXT) {
    return std::make_shared<Next>(StmtRef(lhs), StmtRef(rhs));
  } else if (relTerminal.getValue() == NEXT_T) {
    return std::make_shared<NextT>(StmtRef(lhs), StmtRef(rhs));
  } else if (relTerminal.getValue() == AFFECTS) {
    return std::make_shared<Affects>(StmtRef(lhs), StmtRef(rhs));
  } else if (relTerminal.getValue() == AFFECTS_T) {
    return std::make_shared<AffectsT>(StmtRef(lhs), StmtRef(rhs));
  } else {
    throw QPSException("Unknown or unimplemented Relationship!");
  }
}

std::shared_ptr<Relationship>
RelationshipFactory::createUses(std::shared_ptr<RelRef> lhs,
                                std::shared_ptr<RelRef> rhs) {
  // the first argument of Modifies cannot be wildcard
  if (lhs->getRefType() == RelRefType::WILDCARD) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WILDCARD_USAGE,
        QpsExceptionType::SEMANTIC);
  }

  // second argument can either be variable or constant
  EntRef entRef2 = EntRef(rhs);
  checkEntRefSynonymType(entRef2, {EntityType::VARIABLE});

  try {
    StmtRef stmtRef = StmtRef(lhs);
    return std::make_shared<UsesS>(stmtRef, entRef2);
  } catch (QPSException &e) {
    EntRef entRef = EntRef(lhs);
    checkEntRefSynonymType(entRef, {EntityType::PROCEDURE});

    return std::make_shared<UsesP>(entRef, entRef2);
  }
}

std::shared_ptr<Relationship>
RelationshipFactory::createModifies(std::shared_ptr<RelRef> lhs,
                                    std::shared_ptr<RelRef> rhs) {
  // the first argument of Modifies cannot be wildcard
  if (lhs->getRefType() == RelRefType::WILDCARD) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WILDCARD_USAGE,
        QpsExceptionType::SEMANTIC);
  }

  // second argument must be a variable syn if it is a syn
  EntRef entRef2 = EntRef(rhs);
  checkEntRefSynonymType(entRef2, {EntityType::VARIABLE});

  try {
    StmtRef stmtRef = StmtRef(lhs);
    return std::make_shared<ModifiesS>(stmtRef, entRef2);
  } catch (QPSException &e) {
    EntRef entRef = EntRef(lhs);
    checkEntRefSynonymType(entRef, {EntityType::PROCEDURE});

    return std::make_shared<ModifiesP>(entRef, entRef2);
  }
}

std::shared_ptr<Relationship>
RelationshipFactory::createCalls(const std::string &rel,
                                 std::shared_ptr<RelRef> lhs,
                                 std::shared_ptr<RelRef> rhs) {
  EntRef lhsEnt = EntRef(lhs);
  EntRef rhsEnt = EntRef(rhs);
  checkEntRefSynonymType(lhsEnt, {EntityType::PROCEDURE});
  checkEntRefSynonymType(rhsEnt, {EntityType::PROCEDURE});

  if (rel == CALLS) {
    return std::make_shared<Calls>(lhsEnt, rhsEnt);
  } else {
    return std::make_shared<CallsT>(lhsEnt, rhsEnt);
  }
}
