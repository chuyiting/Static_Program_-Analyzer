#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

enum class ParserExceptionType {
    // invalid grammar
    INVALID_PROGRAM,
    INVALID_STMT,
    INVALID_STMT_LST,
    INVALID_VARIABLE_NAME,
    INVALID_PROCEDURE_NAME,
    INVALID_EXPR,
    INVALID_OPERATOR,
    INVALID_FACTOR,
    INVALID_TERM,
    INVALID_COND_EXPR,
    INVALID_REL_FACTOR,

    // missing tokens
    MISSING_PROCEDURE_KEYWORD,
    MISSING_READ_KEYWORD,
    MISSING_PRINT_KEYWORD,
    MISSING_CALL_KEYWORD,
    MISSING_PROCEDURE_NAME,
    MISSING_IF_KEYWORD,
    MISSING_THEN_KEYWORD,
    MISSING_ELSE_KEYWORD,
    MISSING_WHILE_KEYWORD,

    // missing punctuation
    MISSING_LEFT_ROUND_BRACKET,
    MISSING_RIGHT_ROUND_BRACKET,
    MISSING_LEFT_CURLY_BRACKET,
    MISSING_RIGHT_CURLY_BRACKET,
    MISSING_SEMICOLON,
    MISSING_ASSIGNMENT,
    MISSING_OPERATOR_TOKEN,
    MISSING_COMPARISON_TOKEN,
    MISSING_BINARY_LOGIC_TOKEN,
    MISSING_UNARY_LOGIC_TOKEN,
    MISSING_AND_OPERATOR_TOKEN,
    MISSING_OR_OPERATOR_TOKEN,
    MISSING_NOT_OPERATOR_TOKEN,
    MISSING_NAME,
    MISSING_CONSTANT,

    // semantic errors in program
    DUPLICATE_PROCEDURE_NAME,
    NO_SUCH_PROCEDURE_TO_CALL,
    RECURSIVE_CALL,
    CYCLIC_CALL
};

class ParserException : std::exception {
public:
    ParserException(ParserExceptionType type);
    [[nodiscard]] const char* what() const noexcept override;
private:
    std::string message;
    static inline std::unordered_map<ParserExceptionType, std::string> messageMap =
            {{ParserExceptionType::INVALID_PROGRAM,             "Invalid program!"},
             {ParserExceptionType::INVALID_STMT,                "Invalid statement!"},
             {ParserExceptionType::INVALID_STMT_LST,            "Invalid statement list!"},
             {ParserExceptionType::INVALID_VARIABLE_NAME,       "Invalid variable name!"},
             {ParserExceptionType::INVALID_PROCEDURE_NAME,      "Invalid procedure name!"},
             {ParserExceptionType::INVALID_EXPR,                "Invalid expression!"},
             {ParserExceptionType::INVALID_OPERATOR,            "Invalid operator!"},
             {ParserExceptionType::INVALID_FACTOR,              "Invalid factor!"},
             {ParserExceptionType::INVALID_TERM,                "Invalid term!"},
             {ParserExceptionType::INVALID_COND_EXPR,           "Invalid conditional expression!"},
             {ParserExceptionType::INVALID_REL_FACTOR,          "Invalid relational factor!"},
             {ParserExceptionType::MISSING_PROCEDURE_KEYWORD,   "Missing the 'procedure' keyword!"},
             {ParserExceptionType::MISSING_READ_KEYWORD,        "Missing the 'read' keyword!"},
             {ParserExceptionType::MISSING_PRINT_KEYWORD,       "Missing the 'print' keyword!"},
             {ParserExceptionType::MISSING_CALL_KEYWORD,        "Missing the 'call' keyword!"},
             {ParserExceptionType::MISSING_PROCEDURE_NAME,      "Missing the procedure name!"},
             {ParserExceptionType::MISSING_IF_KEYWORD,          "Missing the 'if' keyword!"},
             {ParserExceptionType::MISSING_THEN_KEYWORD,        "Missing the 'then' keyword!"},
             {ParserExceptionType::MISSING_ELSE_KEYWORD,        "Missing the 'else' keyword!"},
             {ParserExceptionType::MISSING_WHILE_KEYWORD,       "Missing the 'while' keyword!"},
             {ParserExceptionType::MISSING_LEFT_ROUND_BRACKET,  "Missing a left round bracket!"},
             {ParserExceptionType::MISSING_RIGHT_ROUND_BRACKET, "Missing a right round bracket!"},
             {ParserExceptionType::MISSING_LEFT_CURLY_BRACKET,  "Missing a left curly bracket!"},
             {ParserExceptionType::MISSING_RIGHT_CURLY_BRACKET, "Missing a right curly bracket!"},
             {ParserExceptionType::MISSING_SEMICOLON,           "Missing a semicolon!"},
             {ParserExceptionType::MISSING_ASSIGNMENT,          "Missing an assignment sign!"},
             {ParserExceptionType::MISSING_OPERATOR_TOKEN,      "Missing an operator sign!"},
             {ParserExceptionType::MISSING_COMPARISON_TOKEN,    "Missing a comparison sign!"},
             {ParserExceptionType::MISSING_BINARY_LOGIC_TOKEN,  "Missing a binary logic sign!"},
             {ParserExceptionType::MISSING_UNARY_LOGIC_TOKEN,   "Missing an unary logic sign!"},
             {ParserExceptionType::MISSING_AND_OPERATOR_TOKEN,  "Missing the and operator (&&) !"},
             {ParserExceptionType::MISSING_OR_OPERATOR_TOKEN,   "Missing the or operator (||) !"},
             {ParserExceptionType::MISSING_NOT_OPERATOR_TOKEN,  "Missing the not operator (!) !"},
             {ParserExceptionType::MISSING_NAME,                "Missing name!"},
             {ParserExceptionType::MISSING_CONSTANT,            "Missing constant!"},
             {ParserExceptionType::DUPLICATE_PROCEDURE_NAME,    "Cannot have two procedures with the same name!"},
             {ParserExceptionType::NO_SUCH_PROCEDURE_TO_CALL,   "Cannot call a procedure that does not exist!"},
             {ParserExceptionType::RECURSIVE_CALL,              "A procedure cannot call itself!"},
             {ParserExceptionType::CYCLIC_CALL,                 "Cycle detected in procedure calls!"},
            };
};