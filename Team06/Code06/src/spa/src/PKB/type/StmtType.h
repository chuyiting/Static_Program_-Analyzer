//
// Created by Chong Jun Wei on 8/10/22.
//

#ifndef SPA_STMTTYPE_H
#define SPA_STMTTYPE_H

#include "QPS/types/EntityType.h"

enum class StmtType { ASSIGN, IF, WHILE, INVALID };

StmtType entityTypeToStmtType(EntityType type);

#endif // SPA_STMTTYPE_H
