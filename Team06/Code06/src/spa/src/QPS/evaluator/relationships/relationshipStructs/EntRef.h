//
// Created by Eddy Chu on 2022/9/3.
//

#ifndef SPA_ENTREF_H
#define SPA_ENTREF_H

#include "QPS/parser/querystructs/suchthatclause/Literal.h"
#include "QPS/parser/querystructs/suchthatclause/RelRef.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/evaluator/QPSConstraint.h"
#include <cstdint>
#include <utility>

class EntRef {

public:
  enum EntRefType { SYNONYM, WILDCARD, STRING };

  /**
   * The major constructor for entity reference. It checks the validity of the
   * value. Valid inputs include: Synonym: constant, variable, constant Wildcard
   * String: IDENT
   * `QPSException` thrown if the input is invalid
   * @param ref
   */
  explicit EntRef(std::shared_ptr<RelRef> ref);

  EntRef();

  /**
   * Get the string representation of the entref
   * <b>This method serves as a bridge method between QPS and PKB API</b>
   * If the entref is a synonym, a synonym name is returned
   * If the entref is a string, the IDENT string is returned
   * @return
   */
  std::string getEntString();

  /**
   * Get the string value of the entref
   * Return the assigned value if entref is of SYNONYM type
   * Return literal if entref is of LITERAL type
   * Return _ if entref of WILDCARD type
   * @param assign
   * @return
   */
  std::string getEntValue(assign_t &assign);

  bool operator==(const EntRef &other) const;

  bool operator==(const synonym_t &syn) const;

  bool operator!=(const EntRef &other) const;

  [[nodiscard]] EntRefType getType() const;

  [[nodiscard]] const Synonym &getSynonym() const;

  [[nodiscard]] const std::string &getEntity() const;

  // constructor only used in unit testing
  explicit EntRef(std::string str) : entity(std::move(str)) {
    this->type = EntRefType::STRING;
  }

  // constructor only used in unit testing
  explicit EntRef(const Synonym &syn) : synonym(syn) {
    this->type = EntRefType::SYNONYM;
  }

  // constructor only used in unit testing
  EntRef(std::string entity, const Synonym &synonym, EntRefType type);

  [[nodiscard]] bool isSynonym() const;

  [[nodiscard]] bool isWildcard() const;

  [[nodiscard]] bool isString() const;

  bool isSynonymType(const EntityType &type) const;

private:
  std::string entity = "";
  Synonym synonym;
  EntRefType type;

  static void checkEntRefValidity(std::shared_ptr<Synonym> synonym);

  static void checkEntRefValidity(std::shared_ptr<Literal> literal);
};

#endif // SPA_ENTREF_H
