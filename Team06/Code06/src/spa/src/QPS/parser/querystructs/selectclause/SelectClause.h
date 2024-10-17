//
// Created by Eddy Chu on 2022/9/1.
//

#ifndef SPA_SELECTCLAUSE_H
#define SPA_SELECTCLAUSE_H

#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/types/EntityType.h"
#include "SelectTarget.h"
#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

class SelectClause {
  bool isBool = true;
  std::unordered_set<synonym_t> selectedSynonyms;
  std::vector<SelectTarget> selected;

public:
  SelectClause();

  // used to handle test regressions @todo remove this constructor
  SelectClause(synonym_t synonym, EntityType entityType);

  void addSelectTarget(SelectTarget selectTarget);

  [[nodiscard]] const std::vector<SelectTarget> &getSelected() const;

  [[nodiscard]] const std::unordered_set<synonym_t> &
  getSelectedSynonyms() const;

  bool isSynonymSelected(synonym_t synonym) const;

  bool isBoolean() const;

  bool operator==(const SelectClause &other) const;

  bool operator!=(const SelectClause &other) const;
};

#endif // SPA_SELECTCLAUSE_H
