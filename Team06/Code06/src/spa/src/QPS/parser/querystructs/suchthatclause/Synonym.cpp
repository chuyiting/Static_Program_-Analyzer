//
// Created by Eddy Chu on 2022/9/1.
//

#include "Synonym.h"
#include "QPS/exceptions/QPSException.h"
#include "QPS/parser/qps_parse_utils.h"
#include "QPS/types/EntityType.h"

Synonym::Synonym(EntityType type, std::string synonym)
    : type(type), synonym(synonym) {
  if (!qps_parse_utils::is_ident(synonym)) {
    throw QPSException(QPSParseExceptionMessage::QPS_PARSER_INVALID_SYNONYM,
                       QpsExceptionType::SEMANTIC);
  }
}

Synonym::Synonym(const Synonym &other)
    : type(other.type), synonym(other.synonym) {}

EntityType Synonym::getSynonymType() const { return type; }

const std::string &Synonym::getSynonymName() const { return synonym; }

bool Synonym::operator==(const Synonym &other) const {
  return synonym == other.synonym && type == other.type;
}

bool Synonym::operator!=(const Synonym &other) const {
  return !(other == *this);
}

RelRefType Synonym::getRefType() const { return RelRefType::SYNONYM; }

Synonym::Synonym() {}
