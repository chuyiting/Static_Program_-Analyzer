#include "ParserException.h"

ParserException::ParserException(ParserExceptionType type) {
    this->message = messageMap[type];
}

const char* ParserException::what() const noexcept {
    return this->message.c_str();
}
