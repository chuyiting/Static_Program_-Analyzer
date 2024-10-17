//
// Created by Eddy Chu on 2022/9/1.
//

#include "SelectClause.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"

bool SelectClause::operator==(const SelectClause &other) const {
  return this->selected == other.selected;
}

bool SelectClause::operator!=(const SelectClause &other) const {
  return !(other == (*this));
}

SelectClause::SelectClause() {}

SelectClause::SelectClause(synonym_t synonym, EntityType entityType) {
  this->isBool = false;
  SelectTarget selectTarget =
      SelectTarget(std::make_shared<Synonym>(entityType, synonym));
  addSelectTarget(selectTarget);
}

void SelectClause::addSelectTarget(SelectTarget selectTarget) {
  this->isBool = false;
  this->selected.push_back(selectTarget);
  this->selectedSynonyms.insert(selectTarget.getSynonym()->getSynonymName());
}

const std::unordered_set<synonym_t> &SelectClause::getSelectedSynonyms() const {
  return selectedSynonyms;
}

bool SelectClause::isSynonymSelected(synonym_t synonym) const {
  return selectedSynonyms.find(synonym) != selectedSynonyms.end();
}

bool SelectClause::isBoolean() const { return isBool; }

const std::vector<SelectTarget> &SelectClause::getSelected() const {
  return selected;
}
