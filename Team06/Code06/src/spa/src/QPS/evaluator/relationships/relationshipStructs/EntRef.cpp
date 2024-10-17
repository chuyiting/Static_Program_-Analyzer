//
// Created by Eddy Chu on 2022/9/3.
//

#include "EntRef.h"
#include "QPS/exceptions/QPSException.h"
#include <string>
#include <utility>

EntRef::EntRef(std::shared_ptr<RelRef> ref) {
  if (ref->getRefType() == RelRefType::SYNONYM) {
    auto syn = std::dynamic_pointer_cast<Synonym>(ref);
    checkEntRefValidity(syn);
    this->synonym = *syn;
    this->type = SYNONYM;
  } else if (ref->getRefType() == RelRefType::LITERAL) {
    auto literal = std::dynamic_pointer_cast<Literal>(ref);
    checkEntRefValidity(literal);
    this->entity = literal->getVal();
    this->type = STRING;
  } else {
    this->type = WILDCARD;
  }
}

void EntRef::checkEntRefValidity(std::shared_ptr<Synonym> synonym) {
  EntityType type = synonym->getSynonymType();
  if (type == EntityType::CONSTANT || type == EntityType::VARIABLE ||
      type == EntityType::PROCEDURE) {
    return;
  }

  throw QPSException(QPSParseExceptionMessage::QPS_PARSER_INVALID_ENTREF,
                     QpsExceptionType::SEMANTIC);
}

void EntRef::checkEntRefValidity(std::shared_ptr<Literal> literal) {
  if (literal->getType() == Literal::ENTITY) {
    return;
  }

  throw QPSException(QPSParseExceptionMessage::QPS_PARSER_INVALID_ENTREF);
}

bool EntRef::operator==(const EntRef &other) const {
  if (this->type != other.type) {
    return false;
  }

  switch (type) {
  case EntRefType::STRING:
    return this->entity == other.entity;
  case EntRefType::WILDCARD:
    return true;
  case EntRefType::SYNONYM:
    return this->synonym == other.synonym;
  default:
    return false;
  }
}

bool EntRef::operator!=(const EntRef &other) const {
  return !((*this) == other);
}

bool EntRef::isSynonym() const { return this->getType() == SYNONYM; }

bool EntRef::isString() const { return this->getType() == STRING; }

bool EntRef::isSynonymType(const EntityType &type) const {
  if (this->type != EntRefType::SYNONYM) {
    return false;
  }
  return this->synonym.getSynonymType() == type;
}

const Synonym &EntRef::getSynonym() const { return this->synonym; }

EntRef::EntRefType EntRef::getType() const { return type; }

EntRef::EntRef(std::string entity, const Synonym &synonym,
               EntRef::EntRefType type)
    : entity(std::move(entity)), synonym(synonym), type(type) {}

std::string EntRef::getEntString() {
  switch (type) {
  case SYNONYM:
    return synonym.getSynonymName();
  case WILDCARD:
    return "_";
  case STRING:
    return entity;
  default: {
#ifdef DEBUG
    assert(false && "you should never reach here...");
#endif
  }
  }
}

EntRef::EntRef() {}

const std::string &EntRef::getEntity() const { return entity; }

bool EntRef::operator==(const synonym_t &syn) const {
  if (type != SYNONYM) {
    return false;
  }

  return this->synonym.getSynonymName() == syn;
}

bool EntRef::isWildcard() const { return this->getType() == WILDCARD; }

std::string EntRef::getEntValue(assign_t &assign) {
  switch (type) {
  case SYNONYM:
    return assign[synonym.getSynonymName()];
  case WILDCARD:
    return "_";
  default:
    return entity;
  }
}
