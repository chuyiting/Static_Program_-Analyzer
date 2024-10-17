#pragma once

#include "QPS/parser/QPSQuery.h"

class TestQPSDataBuilder {
  std::unordered_map<synonym_t, Synonym> synonymDecls;
  std::vector<std::shared_ptr<SuchThatClause>> stcs;
  std::vector<std::shared_ptr<PatternClause>> pcs;
  SelectClause select;

public:
  TestQPSDataBuilder() {}

  ~TestQPSDataBuilder() {}

  TestQPSDataBuilder *withSynonymDecl(Synonym &s);

  TestQPSDataBuilder *
  withSynonymDecls(std::unordered_map<synonym_t, Synonym> synonymDecls);

  TestQPSDataBuilder *withRelationship(const std::shared_ptr<Relationship> &r);

  TestQPSDataBuilder *withPattern(PatternClause &pattern);

  TestQPSDataBuilder *withSelectClause(SelectClause &select);

  std::shared_ptr<QPSQuery> createQPSQuery();
};