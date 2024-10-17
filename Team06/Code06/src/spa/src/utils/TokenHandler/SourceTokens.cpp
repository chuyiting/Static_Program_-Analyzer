#include "utils/TokenHandler/SourceTokens.h"

// Default constructor.
SourceToken::SourceToken() {
    _tokenType = SourceTokenType::END;
    _value = "";
}

// Constructor.
SourceToken::SourceToken(SourceTokenType tokenType, std::string value) {
    _tokenType = tokenType;
    _value = value;
}

// Get the value of the token.
std::string SourceToken::getValue () const {
  return _value;
}

// Get the type of the token.
SourceTokenType SourceToken::getTokenType () const {
  return _tokenType;
}

// ================ Methods to implement for ParseableToken. ================

// Basics - define the type of the token.
bool SourceToken::isInteger() {
    return _tokenType == SourceTokenType::INTEGER;
}

bool SourceToken::isName() {
    return _tokenType == SourceTokenType::NAME;
}

bool SourceToken::isWhitespace() {
    return _tokenType == SourceTokenType::WHITESPACE;
}

// For expressions.
bool SourceToken::isOperatorToken() {
    return std::find(_operatorTokens.begin(), _operatorTokens.end(), _tokenType) != _operatorTokens.end();
}

bool SourceToken::isPlusOrMinusToken() {
    return _tokenType == SourceTokenType::PLUS || _tokenType == SourceTokenType::MINUS;
}

bool SourceToken::isMultiplyDivideOrModuloToken() {
    return _tokenType == SourceTokenType::MULTIPLY || _tokenType == SourceTokenType::DIVIDE || _tokenType == SourceTokenType::MODULO;
}

int SourceToken::getOperatorPrecedence() {
    if (_tokenType == SourceTokenType::PLUS || _tokenType == SourceTokenType::MINUS) {
        return 1;
    } else if (_tokenType == SourceTokenType::MULTIPLY || _tokenType == SourceTokenType::DIVIDE
               || _tokenType == SourceTokenType::MODULO) {
        return 2;
    } else {
        return 0;
    }
}

// For parsing.
bool SourceToken::isLeftRoundBracketToken() {
    return _tokenType == SourceTokenType::LEFTROUNDBRACKET;
}

bool SourceToken::isRightRoundBracketToken() {
    return _tokenType == SourceTokenType::RIGHTROUNDBRACKET;
}

bool SourceToken::isLeftCurlyBracketToken() {
    return _tokenType == SourceTokenType::LEFTCURLYBRACKET;
}

bool SourceToken::isRightCurlyBracketToken() {
    return _tokenType == SourceTokenType::RIGHTCURLYBRACKET;
}

bool SourceToken::isEndToken() {
    return isEndOfStreamToken();
}

// For conditional expressions.
bool SourceToken::isComparisonToken() {
    return std::find(_comparisonTokens.begin(), _comparisonTokens.end(), _tokenType) != _comparisonTokens.end();
}

bool SourceToken::isAndOperatorToken() {
    return _tokenType == SourceTokenType::AND;
}

bool SourceToken::isOrOperatorToken() {
    return _tokenType == SourceTokenType::OR;
}

// Note that this is referring to the ! token
bool SourceToken::isNotOperatorToken() {
    return _tokenType == SourceTokenType::NOT;
}

// For debugging.
void SourceToken::print() {
    std::cout << "SourceToken(" << _value << ")" << std::endl;
}

// For equality checking.
bool SourceToken::isEqual(std::shared_ptr<ParseableToken> other) {
    std::shared_ptr<SourceToken> otherSourceToken = std::static_pointer_cast<SourceToken>(other);
    return otherSourceToken != nullptr && _tokenType == otherSourceToken->getTokenType() && _value == otherSourceToken->getValue();
}

// ================ End of methods to implement for ParseableToken. ================

// Parse an item into a token.
std::shared_ptr<SourceToken> SourceToken::parse(std::string value) {
    for (std::pair<std::regex, SourceTokenType> rule: _regexRules) {
        std::smatch matches;
        if (regex_search(value, matches, rule.first, std::regex_constants::match_continuous)) {

            SourceTokenType tokenType = rule.second;
            std::string rawValue = matches[0];

            if (tokenType == SourceTokenType::ILLEGAL) {
                throw TokenizerException();
            }

            return std::make_shared<SourceToken>(tokenType, rawValue);
        }
    }
    throw TokenizerException();
}

bool SourceToken::isProcedureKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "procedure";
}

bool SourceToken::isReadKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "read";
}

bool SourceToken::isPrintKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "print";
}

bool SourceToken::isCallKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "call";
}

bool SourceToken::isIfKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "if";
}

bool SourceToken::isThenKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "then";
}

bool SourceToken::isElseKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "else";
}

bool SourceToken::isWhileKeyword() {
    return _tokenType == SourceTokenType::NAME && _value == "while";
}

bool SourceToken::isAssignmentToken() {
    return _tokenType == SourceTokenType::ASSIGNMENT;
}

bool SourceToken::isSemicolonToken() {
    return _tokenType == SourceTokenType::SEMICOLON;
}

bool SourceToken::isEndOfStreamToken() {
    return _tokenType == SourceTokenType::END;
}

std::shared_ptr<SourceToken> SourceToken::createEndToken() {
    return std::make_shared<SourceToken>(SourceTokenType::END, "");
}

std::vector<std::shared_ptr<ParseableToken>> SourceToken::toParseableTokens(std::vector<std::shared_ptr<SourceToken>> sourceTokens) {
    std::vector<std::shared_ptr<ParseableToken>> parseableTokens;
    for (std::shared_ptr<SourceToken> sourceToken: sourceTokens) {
        parseableTokens.push_back(sourceToken);
    }
    return parseableTokens;
}

std::vector<std::shared_ptr<SourceToken>> SourceToken::fromParseableTokens(std::vector<std::shared_ptr<ParseableToken>> parseableTokens) {
    std::vector<std::shared_ptr<SourceToken>> sourceTokens;
    for (std::shared_ptr<ParseableToken> parseableToken: parseableTokens) {
        sourceTokens.push_back(std::static_pointer_cast<SourceToken>(parseableToken));
    }
    return sourceTokens;
}
