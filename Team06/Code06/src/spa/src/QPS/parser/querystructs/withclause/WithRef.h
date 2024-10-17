//
// Created by Eddy Chu on 2022/10/2.
//

#ifndef SPA_WITHREF_H
#define SPA_WITHREF_H

#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/types/AttributeType.h"
#include "QPS/evaluator/QPSConstraint.h"
#include <cassert>
#include <string>

enum class WithRefType { LITERAL, INTEGER, ATTRREF };

class WithRef {
public:
  /**
   * Get the value of the with ref. Return synonym name if withref is AttrRef
   */
  [[nodiscard]] virtual const std::string &getString() const = 0;

  [[nodiscard]] virtual const std::string &
  getValue(assign_t &assign) const = 0;

  [[nodiscard]] virtual WithRefType getRefType() const = 0;

  [[nodiscard]] virtual bool isName() const = 0;

  virtual bool operator==(const WithRef &) = 0;

  virtual bool operator!=(const WithRef &) = 0;
};

class LiteralWithRef : public WithRef {
  std::string value;

public:
  explicit LiteralWithRef(const std::string &value);

  [[nodiscard]] const std::string &getString() const override;

  [[nodiscard]] const std::string &
  getValue(assign_t &assign) const override;

  [[nodiscard]] WithRefType getRefType() const override;

  [[nodiscard]] bool isName() const override;

  bool operator==(const WithRef &other) override;

  bool operator!=(const WithRef &other) override;

  bool operator==(const LiteralWithRef &other);

  bool operator!=(const LiteralWithRef &other);
};

class IntegerWithRef : public WithRef {
  std::string value;

public:
  explicit IntegerWithRef(const std::string &value);

  [[nodiscard]] const std::string &getString() const override;

  [[nodiscard]] const std::string &
  getValue(assign_t &assign) const override;

  [[nodiscard]] WithRefType getRefType() const override;

  [[nodiscard]] bool isName() const override;

  bool operator==(const WithRef &other) override;

  bool operator!=(const WithRef &other) override;

  bool operator==(const IntegerWithRef &other);

  bool operator!=(const IntegerWithRef &other);
};

class AttrWithRef : public WithRef {
  Synonym synonym;
  AttributeType attrName;

public:
  AttrWithRef(const Synonym &synonym, AttributeType attrName);

  [[nodiscard]] const std::string &getString() const override;

  [[nodiscard]] const std::string &
  getValue(assign_t &assign) const override;

  [[nodiscard]] const std::string
  getValue(assign_t &assign, std::shared_ptr<PKBGetter> pkb) const;

  [[nodiscard]] const Synonym &getSynonym() const;

  bool isEntityType(EntityType type);

  [[nodiscard]] AttributeType getAttrName() const;

  [[nodiscard]] WithRefType getRefType() const override;

  [[nodiscard]] bool isName() const override;

  bool operator==(const WithRef &other) override;

  bool operator!=(const WithRef &other) override;

  bool operator==(const AttrWithRef &other);

  bool operator!=(const AttrWithRef &other);
};

#endif // SPA_WITHREF_H
