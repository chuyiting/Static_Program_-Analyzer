//
// Created by Eddy Chu on 2022/8/31.
//

#include "QPSQuery.h"

#include "QPS/exceptions/QPSException.h"
#include <utility>

const std::unordered_map<synonym_t, Synonym> &
QPSQuery::getSynonymDecls() const {
  return synonymDecls;
}

const SelectClause &QPSQuery::getSelectClause() const { return selectClause; }

void QPSQuery::addSynonymDecl(Synonym &synonymDecl) {
  this->synonymDecls.insert(
      std::make_pair(synonymDecl.getSynonymName(), synonymDecl));
}

void QPSQuery::setSelectClause(SelectClause &selectClause) {
  this->selectClause = selectClause;
}

void QPSQuery::addSuchThatClause(std::shared_ptr<SuchThatClause> clause) {
  this->stcs.push_back(clause);
}

void QPSQuery::addPatternClause(std::shared_ptr<PatternClause> clause) {
  this->pcs.push_back(clause);
}

void QPSQuery::addAttrCompare(std::shared_ptr<AttrCompare> attrCompare) {
  this->acs.push_back(attrCompare);
}

void QPSQuery::addSelectTarget(SelectTarget selectTarget) {
  this->selectClause.addSelectTarget(selectTarget);
}

bool QPSQuery::isSynonymDefined(const synonym_t &synonym) const {
  if (synonymDecls.find(synonym) == synonymDecls.end()) {
    return false;
  }
  return true;
}

EntityType QPSQuery::getType(const synonym_t &synonym) const {
  return this->synonymDecls.find(synonym)->second.getSynonymType();
}

std::shared_ptr<Synonym>
QPSQuery::getSynonymFromLiteral(std::string literal) const {
  auto declIter = synonymDecls.find(literal);
  if (declIter == synonymDecls.end()) {
    throw QPSException(QPSParseExceptionMessage::QPS_PARSER_ENTITY_UNDEFINED);
  }
  return std::make_shared<Synonym>(declIter->second);
}

bool QPSQuery::operator==(const QPSQuery &other) const {
  if (this->synonymDecls != other.synonymDecls)
    return false;
  if (this->selectClause != other.selectClause)
    return false;

  if ((this->stcs).size() != other.stcs.size())
    return false;
  if ((this->acs).size() != other.acs.size())
    return false;

  for (int i = 0; i < this->stcs.size(); i++) {
    if (*(this->stcs[i]) != *(other.stcs[i])) {
      return false;
    }
  }

  for (int i = 0; i < this->acs.size(); i++) {
    if (*(this->acs[i]) != *(other.acs[i])) {
      return false;
    }
  }

  return true;
}

bool QPSQuery::operator!=(const QPSQuery &other) const {
  return !(other == *this);
}

std::vector<std::shared_ptr<QPSConstraint>>
QPSQuery::getAllConstraints() const {
  std::vector<std::shared_ptr<QPSConstraint>> cstList;
  cstList.reserve(stcs.size() + pcs.size());
  cstList.insert(cstList.end(), stcs.begin(), stcs.end());
  cstList.insert(cstList.end(), pcs.begin(), pcs.end());
  cstList.insert(cstList.end(), acs.begin(), acs.end());
  return cstList;
}

void QPSQuery::setSynonymDecls(
    const std::unordered_map<synonym_t, Synonym> &synonymDecls) {
  QPSQuery::synonymDecls = synonymDecls;
}

void QPSQuery::setWithClause(
    const std::vector<std::shared_ptr<AttrCompare>> &acs) {
  QPSQuery::acs = acs;
}

bool QPSQuery::isVarSelected(synonym_t var) {
  return selectClause.isSynonymSelected(var);
}

QueryResultType QPSQuery::getQueryResultType() const {
  if (this->selectClause.getSelected().size() == 0) {
    return QueryResultType::BOOLEAN;
  } else if (this->selectClause.getSelected().size() == 1) {
    return QueryResultType::SINGLE;
  } else {
    return QueryResultType::TUPLE;
  }
}

const std::vector<std::shared_ptr<SuchThatClause>> &QPSQuery::getStcs() const {
  return stcs;
}

QPSQuery::QPSQuery(std::unordered_map<synonym_t, Synonym> synonymDecls,
                   std::vector<std::shared_ptr<SuchThatClause>> stcs,
                   std::vector<std::shared_ptr<PatternClause>> pcs,
                   const SelectClause &selectClause)
    : synonymDecls(std::move(synonymDecls)), stcs(std::move(stcs)),
      pcs(std::move(pcs)), selectClause(selectClause) {}

const std::unordered_set<synonym_t> &QPSQuery::extractSelectedSynonyms() const {
  return this->selectClause.getSelectedSynonyms();
}

bool QPSQuery::isBoolean() { return selectClause.isBoolean(); }
