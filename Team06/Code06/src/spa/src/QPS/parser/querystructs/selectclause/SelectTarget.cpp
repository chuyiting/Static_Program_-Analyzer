//
// Created by Eddy Chu on 2022/10/6.
//

#include "SelectTarget.h"

bool SelectTarget::operator==(const SelectTarget &other) const {
  return *(this->synonym) == *(other.synonym) &&
         this->attribute == other.attribute;
}

bool SelectTarget::operator!=(const SelectTarget &other) const {
  return !((*this) == other);
}

[[maybe_unused]] AttributeType SelectTarget::getAttribute() const {
  return attribute;
}

SelectTarget::SelectTarget(std::shared_ptr<Synonym> synonym)
    : synonym(synonym) {}

SelectTarget::SelectTarget(std::shared_ptr<Synonym> synonym,
                           AttributeType attribute)
    : synonym(synonym), attribute(attribute) {}

std::shared_ptr<Synonym> SelectTarget::getSynonym() const { return synonym; }

SelectTarget::SelectTarget() {}

synonym_t SelectTarget::getSynonymName() const {
  return synonym->getSynonymName();
}

EntityType SelectTarget::getSynonymType() const {
  return synonym->getSynonymType();
}
