//
// Created by Eddy Chu on 2022/9/3.
//

#include "StmtRef.h"
#include "QPS/exceptions/QPSException.h"
#include "QPS/parser/querystructs/suchthatclause/Literal.h"
#include <algorithm>
#include <cassert>

StmtRef::StmtRef(std::shared_ptr<RelRef> ref) {
  if (ref->getRefType() == RelRefType::SYNONYM) {
    auto pSynonym = std::dynamic_pointer_cast<Synonym>(ref);
    checkStmtRefValidity(pSynonym);
    this->synonym = *pSynonym;
    this->type = SYNONYM;
  } else if (ref->getRefType() == RelRefType::LITERAL) {
    auto literal = std::dynamic_pointer_cast<Literal>(ref);
    checkStmtRefValidity(literal);
    this->stmtNum = stoi(literal->getVal());
    this->type = INTEGER;
  } else {
    this->type = WILDCARD;
  }
}

void StmtRef::checkStmtRefValidity(std::shared_ptr<Synonym> synonym) {
  EntityType type = synonym->getSynonymType();
  if (type == EntityType::STMT || type == EntityType::WHILE ||
      type == EntityType::ASSIGN || type == EntityType::PRINT ||
      type == EntityType::IF || type == EntityType::READ ||
      type == EntityType::CALL) {
    return;
  }

  throw QPSException(QPSParseExceptionMessage::QPS_PARSER_INVALID_STMTREF,
                     QpsExceptionType::SEMANTIC);
}

void StmtRef::checkStmtRefValidity(std::shared_ptr<Literal> literal) {
  if (literal->getType() == Literal::STMT) {
    return;
  }
  throw QPSException(QPSParseExceptionMessage::QPS_PARSER_INVALID_STMTREF);
}

bool StmtRef::operator==(const StmtRef &other) {
  if (this->type != other.type) {
    return false;
  }

  switch (type) {
  case StmtRefType::INTEGER:
    return this->stmtNum == other.stmtNum;
  case StmtRefType::WILDCARD:
    return true;
  case StmtRefType::SYNONYM:
    return this->synonym == other.synonym;
  default:
    return false;
  }
}

bool StmtRef::operator!=(const StmtRef &other) { return !((*this) == other); }

StmtRef::StmtRefType StmtRef::getType() const { return type; }

bool StmtRef::isSynonym() const { return this->getType() == SYNONYM; }

bool StmtRef::isWildcard() const { return this->getType() == WILDCARD; }

const Synonym &StmtRef::getSynonym() const { return this->synonym; }

StmtRef::StmtRef(uint64_t stmtNum, const Synonym &synonym,
                 StmtRef::StmtRefType type)
    : stmtNum(stmtNum), synonym(synonym), type(type) {}

std::string StmtRef::getStmtString() {
  switch (type) {
  case SYNONYM:
    return synonym.getSynonymName();
  case WILDCARD:
    return "_";
  case INTEGER:
    return std::to_string(stmtNum);
  default: {
#ifdef DEBUG
    assert(false && "you should never reach here...");
#endif
  }
  }
}

StmtRef::StmtRef() {}

bool StmtRef::operator==(const synonym_t &synonym) {
  if (type != SYNONYM) {
    return false;
  }
  return synonym == this->synonym.getSynonymName();
}

std::string StmtRef::getStmtValue(assign_t &assign) {
  switch (type) {
  case SYNONYM:
    return assign[synonym.getSynonymName()];
  case WILDCARD:
    return "_";
  default:
    return std::to_string(stmtNum);
  }
}


