#pragma once

#include <stdio.h>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "Exceptions/TokenizerException.h"
#include "utils/TokenHandler/ParseableToken.h"

enum class SourceTokenType {
    ILLEGAL,
    WHITESPACE,
    NAME,
    INTEGER,
    LEFTROUNDBRACKET,
    RIGHTROUNDBRACKET,
    SEMICOLON,
    LEFTCURLYBRACKET,
    RIGHTCURLYBRACKET,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    NOT,
    AND,
    OR,
    ASSIGNMENT,
    EQUAL,
    NOTEQUAL,
    GREATERTHAN,
    GREATERTHANOREQUAL,
    LESSTHAN,
    LESSTHANOREQUAL,
    END,
};

class SourceToken : public ParseableToken {
private:
    // Static class variables.
    static inline std::vector<SourceTokenType> _operatorTokens = {
            SourceTokenType::PLUS, SourceTokenType::MINUS, SourceTokenType::MULTIPLY, SourceTokenType::DIVIDE,
            SourceTokenType::MODULO,
    };
    static inline std::vector<SourceTokenType> _comparisonTokens = {
            SourceTokenType::EQUAL, SourceTokenType::NOTEQUAL, SourceTokenType::GREATERTHANOREQUAL,
            SourceTokenType::GREATERTHAN, SourceTokenType::LESSTHANOREQUAL, SourceTokenType::LESSTHAN,
    };
    static inline std::vector<std::pair<std::regex, SourceTokenType>> _regexRules =
            {{std::regex("\\s+"),                 SourceTokenType::WHITESPACE},
             {std::regex("[0-9]+[a-zA-Z]+"),      SourceTokenType::ILLEGAL},
             {std::regex("[0][0-9]+"),            SourceTokenType::ILLEGAL},
             {std::regex("0|[1-9][0-9]*"),        SourceTokenType::INTEGER},
             {std::regex("[a-zA-Z][a-zA-Z0-9]*"), SourceTokenType::NAME},
             {std::regex("\\("),                  SourceTokenType::LEFTROUNDBRACKET},
             {std::regex("\\)"),                  SourceTokenType::RIGHTROUNDBRACKET},
             {std::regex(";"),                    SourceTokenType::SEMICOLON},
             {std::regex("\\{"),                  SourceTokenType::LEFTCURLYBRACKET},
             {std::regex("\\}"),                  SourceTokenType::RIGHTCURLYBRACKET},
             {std::regex("\\+"),                  SourceTokenType::PLUS},
             {std::regex("-"),                    SourceTokenType::MINUS},
             {std::regex("\\*"),                  SourceTokenType::MULTIPLY},
             {std::regex("/"),                    SourceTokenType::DIVIDE},
             {std::regex("%"),                    SourceTokenType::MODULO},
             {std::regex("&&"),                   SourceTokenType::AND},
             {std::regex("\\|\\|"),               SourceTokenType::OR},
             {std::regex("=="),                   SourceTokenType::EQUAL},
             {std::regex("="),                    SourceTokenType::ASSIGNMENT},
             {std::regex("!="),                   SourceTokenType::NOTEQUAL},
             {std::regex("!"),                    SourceTokenType::NOT},
             {std::regex(">="),                   SourceTokenType::GREATERTHANOREQUAL},
             {std::regex(">"),                    SourceTokenType::GREATERTHAN},
             {std::regex("<="),                   SourceTokenType::LESSTHANOREQUAL},
             {std::regex("<"),                    SourceTokenType::LESSTHAN},
            };

    // Field variables.
    SourceTokenType _tokenType;
    std::string _value;
public:
    // Constructors.
    SourceToken();
    SourceToken(SourceTokenType tokenType, std::string value);

    // Getters.
    SourceTokenType getTokenType () const;
    std::string getValue() const;

    // ================ Methods to implement for ParseableToken. ================

    // Basics - define the type of the token.
    bool isInteger();
    bool isName();
    bool isWhitespace();

    // For expressions.
    bool isOperatorToken();
    bool isPlusOrMinusToken();
    bool isMultiplyDivideOrModuloToken();
    int getOperatorPrecedence();

    // For parsing.
    bool isLeftRoundBracketToken();
    bool isRightRoundBracketToken();
    bool isLeftCurlyBracketToken();
    bool isRightCurlyBracketToken();
    bool isEndToken();
    
    // For conditional expressions.
    bool isComparisonToken();
    bool isAndOperatorToken();
    bool isOrOperatorToken();
    bool isNotOperatorToken();

    // For debugging.
    void print();

    // For equality checking.
    bool isEqual(std::shared_ptr<ParseableToken> otherToken);

    // ================ SourceToken-specific methods. ================

    // Methods.
    static std::shared_ptr<SourceToken> parse(std::string val);
    bool isProcedureKeyword();
    bool isReadKeyword();
    bool isPrintKeyword();
    bool isCallKeyword();
    bool isIfKeyword();
    bool isThenKeyword();
    bool isElseKeyword();
    bool isWhileKeyword();
    bool isAssignmentToken();
    bool isSemicolonToken();
    bool isEndOfStreamToken();

    static std::shared_ptr<SourceToken> createEndToken();
    static std::vector<std::shared_ptr<ParseableToken>> toParseableTokens(std::vector<std::shared_ptr<SourceToken>> sourceTokens);
    static std::vector<std::shared_ptr<SourceToken>> fromParseableTokens(std::vector<std::shared_ptr<ParseableToken>> parseableTokens);
};
