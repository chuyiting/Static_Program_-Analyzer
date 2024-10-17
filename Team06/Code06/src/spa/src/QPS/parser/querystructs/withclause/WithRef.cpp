//
// Created by Eddy Chu on 2022/10/2.
//

#include "WithRef.h"
#include "QPS/exceptions/QPSException.h"
#include "QPS/parser/qps_parse_utils.h"
#include "QPS/types/AttributeType.h"

const std::string &LiteralWithRef::getString() const { return value; }

LiteralWithRef::LiteralWithRef(const std::string &value) : value(value) {
  if (!qps_parse_utils::is_ident(value)) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WITH_CLAUSE_SYNTAX,
        QpsExceptionType::SEMANTIC);
  }
}

WithRefType LiteralWithRef::getRefType() const { return WithRefType::LITERAL; }

bool LiteralWithRef::operator==(const LiteralWithRef &other) {
  return this->value == other.value;
}

bool LiteralWithRef::operator!=(const LiteralWithRef &other) {
  return !((*this) == other);
}

bool LiteralWithRef::operator==(const WithRef &other) {
  try {
    auto &derived = dynamic_cast<const LiteralWithRef &>(other);
    return (*this) == derived;
  } catch (std::bad_cast &err) {
    return false;
  }
}

bool LiteralWithRef::operator!=(const WithRef &other) {
  return !((*this) == other);
}

bool LiteralWithRef::isName() const { return true; }
const std::string &LiteralWithRef::getValue(assign_t &assign) const {
  return value;
}

IntegerWithRef::IntegerWithRef(const std::string &value) : value(value) {
  if (!qps_parse_utils::is_integer(value)) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WITH_CLAUSE_SYNTAX,
        QpsExceptionType::SEMANTIC);
  }
}

const std::string &IntegerWithRef::getString() const { return value; }

WithRefType IntegerWithRef::getRefType() const { return WithRefType::INTEGER; }

bool IntegerWithRef::operator==(const IntegerWithRef &other) {
  return this->value == other.value;
}

bool IntegerWithRef::operator!=(const IntegerWithRef &other) {
  return !((*this) == other);
}

bool IntegerWithRef::operator==(const WithRef &other) {
  try {
    auto &derived = dynamic_cast<const IntegerWithRef &>(other);
    return (*this) == derived;
  } catch (std::bad_cast &err) {
    return false;
  }
}

bool IntegerWithRef::operator!=(const WithRef &other) {
  return !((*this) == other);
}

bool IntegerWithRef::isName() const { return false; }
const std::string &IntegerWithRef::getValue(assign_t &assign) const {
  return value;
}

/**
 * Constructor for AttrWithRef
 * @param synonym should be a valid synonym type. It can't be INVALID TYPE
 * @param attrName should be a valid attribute type. It can't be UNKNOWN TYPE
 */
AttrWithRef::AttrWithRef(const Synonym &synonym, AttributeType attrName)
    : synonym(synonym), attrName(attrName) {
#ifdef DEBUG
  assert(synonym.getSynonymType() != EntityType::INVALID &&
         attrName != AttributeType::UNKNOWN && "invalid argument...");
#endif
  if (!isValidAttribute(synonym.getSynonymType(), attrName)) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_WITH_CLAUSE_SYNTAX,
        QpsExceptionType::SEMANTIC);
  }
}

const Synonym &AttrWithRef::getSynonym() const { return synonym; }

AttributeType AttrWithRef::getAttrName() const { return attrName; }

WithRefType AttrWithRef::getRefType() const { return WithRefType::ATTRREF; }

bool AttrWithRef::operator==(const AttrWithRef &other) {
  return this->attrName == other.attrName && this->synonym == other.synonym;
}

bool AttrWithRef::operator!=(const AttrWithRef &other) {
  return !((*this) == other);
}

bool AttrWithRef::operator==(const WithRef &other) {
  try {
    auto &derived = dynamic_cast<const AttrWithRef &>(other);
    return (*this) == derived;
  } catch (std::bad_cast &err) {
    return false;
  }
}

bool AttrWithRef::operator!=(const WithRef &other) {
  return !((*this) == other);
}

bool AttrWithRef::isName() const { return isNameValue(this->attrName); }

/**
 * There is no value for AttrWithRef yet, therefore return the synonym name
 * @return synonym name
 */
const std::string &AttrWithRef::getString() const {
  return synonym.getSynonymName();
}

const std::string &AttrWithRef::getValue(assign_t &assign) const {
  return assign[synonym.getSynonymName()];
}
bool AttrWithRef::isEntityType(EntityType type) {
  return synonym.getSynonymType() == type;
}

const std::string AttrWithRef::getValue(assign_t &assign,
                                        std::shared_ptr<PKBGetter> pkb) const {
  if ((synonym.getSynonymType() == EntityType::CALL &&
       attrName == AttributeType::PROCNAME) ||
      (synonym.getSynonymType() == EntityType::READ &&
       attrName == AttributeType::VARNAME) ||
      (synonym.getSynonymType() == EntityType::PRINT &&
       attrName == AttributeType::VARNAME)) {
    return pkb->getVarAt(getValue(assign));
  }

  return getValue(assign);
}
