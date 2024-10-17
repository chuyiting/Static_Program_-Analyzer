//
// Created by Eddy Chu on 2022/9/3.
//

#include "Literal.h"
#include "QPS/exceptions/QPSException.h"
#include "QPS/parser/qps_parse_utils.h"
#include <algorithm>
#include <string>

inline bool isAlphanum(std::string str) {
  return std::all_of(str.begin(), str.end(),
                     [](char c) { return std::isalnum(c); }) &&
         std::isalpha(str[0]);
}

Literal::Literal(const QueryToken &token) : val(token.getValue()) {
  if (token.getType() == TokenType::IntegerToken) {
    int num = stoi(token.getValue());
    type = STMT;
  } else if (token.getType() == TokenType::QuoteToken) {
    if (!qps_parse_utils::is_ident(token.getValue())) {
      throw QPSException(
          QPSParseExceptionMessage::QPS_PARSER_INVALID_VARIABLE_NAME);
    }

    this->val = token.getValue();
    type = ENTITY;
  } else {
    throw QPSException(QPSParseExceptionMessage::QPS_PARSER_INVALID_LITERAL);
  }
}

bool Literal::operator==(const Literal &other) const {
  return (val == other.val && type == other.getType());
}

bool Literal::operator!=(const Literal &other) const {
  return !(other == *this);
}

const std::string &Literal::getVal() const { return val; }

RelRefType Literal::getRefType() const { return RelRefType::LITERAL; }

Literal::LiteralType Literal::getType() const { return type; }
