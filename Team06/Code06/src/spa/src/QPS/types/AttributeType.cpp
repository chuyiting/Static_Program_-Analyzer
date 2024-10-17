//
// Created by Eddy Chu on 2022/10/2.
//

#include "AttributeType.h"
#include "EntityType.h"
#include <cassert>
#include <unordered_map>
#include <unordered_set>

std::unordered_map<std::string, AttributeType> attributeMap = {
    {"procName", AttributeType::PROCNAME},
    {"varName", AttributeType::VARNAME},
    {"value", AttributeType::VALUE},
    {"stmt#", AttributeType::STMTNUM}};

std::unordered_map<EntityType, std::unordered_set<AttributeType>>
    entityAttributeMap = {
        {EntityType::PROCEDURE, {AttributeType::PROCNAME}},
        {EntityType::CALL, {AttributeType::PROCNAME, AttributeType::STMTNUM}},
        {EntityType::VARIABLE, {AttributeType::VARNAME}},
        {EntityType::READ, {AttributeType::VARNAME, AttributeType::STMTNUM}},
        {EntityType::PRINT, {AttributeType::VARNAME, AttributeType::STMTNUM}},
        {EntityType::CONSTANT, {AttributeType::VALUE}},
        {EntityType::STMT, {AttributeType::STMTNUM}},
        {EntityType::WHILE, {AttributeType::STMTNUM}},
        {EntityType::IF, {AttributeType::STMTNUM}},
        {EntityType::ASSIGN, {AttributeType::STMTNUM}},
};

bool isNameValue(AttributeType type) {
  if (type == AttributeType::PROCNAME || type == AttributeType::VARNAME) {
    return true;
  }

  if (type == AttributeType::VALUE || type == AttributeType::STMTNUM) {
    return false;
  }

#ifdef DEBUG
  assert(false && "you should never reach here... (UNKNOWN AttributeType "
                  "should be checked before using this function)");
#endif
  return false;
}

/**
 * Inline function to convert string literal to AttributeType
 * @param type
 * @return
 */
AttributeType toAttributeType(const std::string &type) {
  if (attributeMap.find(type) == attributeMap.end()) {
    return AttributeType::UNKNOWN;
  }

  return attributeMap.find(type)->second;
}

bool isValidAttributeName(const std::string &attr) {
  if (attributeMap.find(attr) == attributeMap.end()) {
    return false;
  }

  return true;
}

bool isValidAttribute(EntityType entityType, AttributeType attrName) {
#ifdef DEBUG
  assert(entityAttributeMap.find(entityType) != entityAttributeMap.end() &&
         "entityType supplied for this function must be a valid entityType");
#endif

  auto attrSet = entityAttributeMap.find(entityType)->second;
  return attrSet.find(attrName) != attrSet.end();
}