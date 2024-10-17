//
// Created by Eddy Chu on 2022/10/6.
//

#ifndef SPA_SELECTTARGET_H
#define SPA_SELECTTARGET_H

#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/types/AttributeType.h"

class SelectTarget {
  std::shared_ptr<Synonym> synonym{};
  AttributeType attribute = AttributeType::UNKNOWN; // default attribute

public:
  explicit SelectTarget(std::shared_ptr<Synonym> synonym);

  SelectTarget(std::shared_ptr<Synonym> synonym, AttributeType attribute);

  [[nodiscard]] std::shared_ptr<Synonym> getSynonym() const;

  [[nodiscard]] synonym_t getSynonymName() const;

  [[nodiscard]] EntityType getSynonymType() const;

  [[maybe_unused]] [[nodiscard]] AttributeType getAttribute() const;

  bool operator==(const SelectTarget &other) const;

  bool operator!=(const SelectTarget &other) const;

  SelectTarget();
};

#endif // SPA_SELECTTARGET_H
