//
// Created by Eddy Chu on 2022/9/1.
//

#ifndef SPA_QPSEXCEPTION_H
#define SPA_QPSEXCEPTION_H

#include <exception>
#include <ostream>
#include <string>

enum class QpsExceptionType {
  // UNKNOWN is reserved for only special cases, use it with caution
  SYNTACTIC,
  SEMANTIC,
  UNKNOWN
};

namespace QPSParseExceptionMessage {
// parser exceptions
constexpr auto QPS_PARSER_SYNTAX_ERROR_PQL = "Input PQL has syntax error...";
constexpr auto QPS_PARSER_EMPTY_QUERY = "Empty query";
constexpr auto QPS_PARSER_SYNONYM_DECLARATION_TERMINAL =
    "Synonym declaration does not end with a semi-colon";
constexpr auto QPS_PARSER_SYNONYM_INVALID_SYNONYM_DECLARATION =
    "Invalid synonoym declaration";
constexpr auto QPS_PARSER_SYNONYM_INVALID_SYNONYM_TYPE = "Invalid synonym type";
constexpr auto QPS_PARSER_REPEATED_SYNONYM_DECLARE =
    "Synonym name has already been declared";
constexpr auto QPS_PARSER_SYNONYM_MISSING_SYNONYM = "Synonym is missing";
constexpr auto QPS_PARSER_INVALID_SYNONYM = "Synonym is missing";
constexpr auto QPS_PARSER_SYNONYM_ERROR_PARSING =
    "Error occurs while parsing the qps query";
constexpr auto QPS_PARSER_MISSING_SELECT_STATEMENT =
    "Select statement is missing";
constexpr auto QPS_PARSER_ILLEGAL_SELECT_STATEMENT = "Illegal select syntax";
constexpr auto QPS_INVALID_CLAUSE = "Invalid clause";
constexpr auto QPS_INVALID_SELECT_SYNONYM_NOT_DEFINED =
    "Invalid select statement: synonym not defined";
constexpr auto QPS_PARSER_INVALID_VARIABLE_NAME =
    "Invalid variable name. Variable name should alphanumeric and the first "
    "character must be an alphabet";
constexpr auto QPS_PARSER_INVALID_LITERAL = "Invalid literal";
constexpr auto QPS_PARSER_INVALID_SUCH_THAT_CLAUSE_SYNTAX =
    "invalid such that clause syntax";
constexpr auto QPS_PARSER_INVALID_STMTREF = "invalid statement reference";
constexpr auto QPS_PARSER_INVALID_ENTREF = "invalid entity reference";
constexpr auto QPS_PARSER_ENTITY_UNDEFINED = "entity undefined";
constexpr auto QPS_PARSER_INVALID_WILDCARD_USAGE =
    "Wildcard cannot be the first argument of Uses and Modifies";
constexpr auto QPS_PARSER_SYNONYM_REDECLARED =
    "Synonyms can only declared once.";
constexpr auto QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX =
    "Invalid syntax for pattern clause";
constexpr auto QPS_PARSER_INVALID_WITH_CLAUSE_SYNTAX =
    "Invalid syntax for with clause";

} // namespace QPSParseExceptionMessage

namespace QPSTokenizerExceptionMessage {
constexpr auto QPS_TOKENIZER_INVALID_TOKEN =
    "Invalid token: valid tokens should be either keywords/terminals or "
    "alphanum";
constexpr auto QPS_TOKENIZER_INVALID_Syntax = "Invalid Syntax";
} // namespace QPSTokenizerExceptionMessage

class QPSException : public virtual std::exception {
  std::string message;
  QpsExceptionType type;

public:
  explicit QPSException(
      const std::string &message,
      QpsExceptionType type = QpsExceptionType::SYNTACTIC) noexcept;

  [[nodiscard]] const char *what() const noexcept override;

  QpsExceptionType getType() const;

  operator std::string() const {
    switch (type) {
    case QpsExceptionType::SEMANTIC:
      return "SemanticError";
    case QpsExceptionType::SYNTACTIC:
      return "SyntaxError";
    default:
      return "";
    }
  }
};

#endif // SPA_QPSEXCEPTION_H
