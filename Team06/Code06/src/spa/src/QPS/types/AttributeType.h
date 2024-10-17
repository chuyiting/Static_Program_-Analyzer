//
// Created by Eddy Chu on 2022/10/1.
//

#ifndef SPA_ATTRIBUTETYPE_H
#define SPA_ATTRIBUTETYPE_H

#include "EntityType.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

enum class AttributeType { PROCNAME, VARNAME, VALUE, STMTNUM, UNKNOWN };

bool isNameValue(AttributeType type);

/**
 * Function to convert string literal to AttributeType
 * @param type
 * @return
 */
AttributeType toAttributeType(const std::string &type);

bool isValidAttributeName(const std::string &attr);

bool isValidAttribute(EntityType entityType, AttributeType attrName);

#endif // SPA_ATTRIBUTETYPE_H
