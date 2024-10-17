#include "TestQueryDataBuilder.h"

TestQPSDataBuilder *TestQPSDataBuilder::withSynonymDecl(Synonym &s) {
  synonymDecls[s.getSynonymName()] = s;
  return this;
}

TestQPSDataBuilder *TestQPSDataBuilder::withSynonymDecls(
    std::unordered_map<synonym_t, Synonym> synonymDecls) {
  for (auto i : synonymDecls) {
    this->synonymDecls[i.first] = i.second;
  }
  return this;
}

TestQPSDataBuilder *
TestQPSDataBuilder::withRelationship(const std::shared_ptr<Relationship> &r) {
  auto stc = std::make_shared<SuchThatClause>(r);
  stcs.push_back(stc);
  return this;
}

TestQPSDataBuilder *TestQPSDataBuilder::withPattern(PatternClause &pattern) {
  pcs.push_back(std::make_shared<PatternClause>(pattern));
  return this;
}

TestQPSDataBuilder *TestQPSDataBuilder::withSelectClause(SelectClause &select) {
  this->select = select;
  return this;
}

std::shared_ptr<QPSQuery> TestQPSDataBuilder::createQPSQuery() {
  return std::make_shared<QPSQuery>(synonymDecls, stcs, pcs, select);
}