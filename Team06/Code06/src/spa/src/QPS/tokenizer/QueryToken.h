#ifndef QUERYTOKEN_H
#define QUERYTOKEN_H

#include <string>

// may be refactored into proper token classes in the future
enum TokenType {
  StringToken,
  IntegerToken,
  QuoteToken,
  PunctuatorToken,
  WildcardToken,
  DotToken,
  AttributeToken,
  EqualToken
};

class QueryToken {
  std::string value;
  TokenType type;

public:
  const std::string &getValue() const;

  TokenType getType() const;

public:
  // should only be used for testing purposes
  QueryToken(std::string value1) : value(value1), type(StringToken) {}

  QueryToken(std::string value1, TokenType type1)
      : value(value1), type(type1) {}

  const std::string toString() const;

  bool operator==(const QueryToken &other) const {
    return value == other.value && type == other.type;
  };

  bool operator!=(const QueryToken &other) const {
    return !((*this) == other);
  };

  bool operator==(const std::string literal) const { return value == literal; };

  bool operator!=(const std::string literal) const {
    return !(value == literal);
  };

  friend std::ostream &operator<<(std::ostream &out, QueryToken const &token) {
    out << token.toString();
    return out;
  }
};

#endif