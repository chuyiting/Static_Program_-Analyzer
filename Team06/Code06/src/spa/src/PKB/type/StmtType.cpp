//
// Created by Eddy Chu on 2022/10/16.
//

#include "StmtType.h"

StmtType entityTypeToStmtType(EntityType type) {
  switch (type) {
  case EntityType::ASSIGN:
    return StmtType::ASSIGN;
  case EntityType::IF:
    return StmtType::IF;
  case EntityType::WHILE:
    return StmtType::WHILE;
  default: {
#ifdef DEBUG
    assert(false && "type should be either assign, if or while")
#endif
        return StmtType::INVALID;
  }
  }
}
