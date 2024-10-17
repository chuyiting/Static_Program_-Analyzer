//
// Created by Eddy Chu on 2022/9/7.
//

#include "QueryToken.h"
#include <sstream>

std::string tokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::StringToken:
    return "String token";
  case TokenType::IntegerToken:
    return "Integer token";
  case TokenType::PunctuatorToken:
    return "Punctuator token";
  case TokenType::DotToken:
    return "Dot token";
  case TokenType::AttributeToken:
    return "Attribute token";
  case TokenType::EqualToken:
    return "Equal token";
  case TokenType::QuoteToken:
    return "Quote token";
  default:
    return "Invalid";
  }
}

const std::string QueryToken::toString() const {
  std::stringstream ss;
  ss << "<" << tokenTypeToString((type)) << " " << value << ">";
  return ss.str();
}

const std::string &QueryToken::getValue() const { return value; }

TokenType QueryToken::getType() const { return type; }
