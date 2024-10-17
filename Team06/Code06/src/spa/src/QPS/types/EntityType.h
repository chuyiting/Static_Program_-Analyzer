//
// Created by Eddy Chu on 2022/9/1.
//

#ifndef SPA_ENTITYTYPE_H
#define SPA_ENTITYTYPE_H

#include "QPS/exceptions/QPSException.h"
#include <string>

enum class EntityType {
  PROCEDURE,
  STMT,
  READ,
  PRINT,
  ASSIGN,
  CALL,
  WHILE,
  IF,
  VARIABLE,
  CONSTANT,
  INVALID
};

/**
 * Inline function to convert string literal to EntityType
 * @param type
 * @return
 */
inline const EntityType toEntityType(std::string type) {
  if (type == "procedure") {
    return EntityType::PROCEDURE;
  } else if (type == "stmt") {
    return EntityType::STMT;
  } else if (type == "read") {
    return EntityType::READ;
  } else if (type == "print") {
    return EntityType::PRINT;
  } else if (type == "assign") {
    return EntityType::ASSIGN;
  } else if (type == "call") {
    return EntityType::CALL;
  } else if (type == "while") {
    return EntityType::WHILE;
  } else if (type == "if") {
    return EntityType::IF;
  } else if (type == "variable") {
    return EntityType::VARIABLE;
  } else if (type == "constant") {
    return EntityType::CONSTANT;
  } else if (type == "call") {
    return EntityType::CALL;
  } else {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_SYNONYM_INVALID_SYNONYM_TYPE);
  }
}

inline const std::string entityTypeToString(EntityType type) {
  switch (type) {
  case EntityType::PROCEDURE:
    return "procedure";
  case EntityType::STMT:
    return "stmt";
  case EntityType::READ:
    return "read";
  case EntityType::PRINT:
    return "print";
  case EntityType::ASSIGN:
    return "assign";
  case EntityType::CALL:
    return "call";
  case EntityType::WHILE:
    return "while";
  case EntityType::IF:
    return "if";
  case EntityType::VARIABLE:
    return "variable";
  case EntityType::CONSTANT:
    return "constant";
  default:
    return "invalid";
  }
}

#endif // SPA_ENTITYTYPE_H
