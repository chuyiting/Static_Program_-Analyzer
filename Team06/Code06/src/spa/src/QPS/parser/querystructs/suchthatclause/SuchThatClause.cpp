//
// Created by Eddy Chu on 2022/9/1.
//

#include "SuchThatClause.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include <vector>

void SuchThatClause::setRelationship(std::shared_ptr<Relationship> rel) {
  this->relationship = std::move(rel);
}

bool SuchThatClause::isSatisfied(std::shared_ptr<PKBGetter> pkb,
                                 assign_t &assign) {
  return this->relationship->isSatisfied(pkb, assign);
}

std::vector<synonym_t> SuchThatClause::getVariables() {
  return this->relationship->getSynonyms();
}

bool SuchThatClause::queryDomain(std::shared_ptr<PKBGetter> pkb,
                                 assign_t &assign, orderDomain_t &orderDomain,
                                 orderDomain_t &queryResult) {
  if (!hasExactOneVarUnassigned(assign)) {
    return false;
  }

  return this->relationship->queryDomain(pkb, assign, orderDomain, queryResult);
}

bool SuchThatClause::operator==(const SuchThatClause &other) const {
  return (*this->relationship) == (*other.relationship);
}

bool SuchThatClause::operator!=(const SuchThatClause &other) const {
  return !((*this) == other);
}

int SuchThatClause::priority() { return this->relationship->priority(); }
