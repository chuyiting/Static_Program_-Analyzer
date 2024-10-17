#pragma once

#include "PKB/PKBGetter.h"
#include "QPS/types/EntityType.h"
#include "RelRef.h"
#include <string>

typedef std::string synonym_t;

class Synonym : public RelRef {
  EntityType type = EntityType::INVALID;
  synonym_t synonym;

public:
  /**
   * @brief Constructor for synonym declaration
   */
  Synonym();

  Synonym(EntityType type, synonym_t synonym);

  Synonym(const Synonym &other);

  EntityType getSynonymType() const;

  const synonym_t &getSynonymName() const;

  RelRefType getRefType() const override;

  bool operator==(const Synonym &other) const;

  bool operator!=(const Synonym &other) const;

  friend std::ostream &operator<<(std::ostream &output, const Synonym &data) {
    output << data.synonym << " " << entityTypeToString(data.type);
    return output;
  }
};
