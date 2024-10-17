#include "TokenizerException.h"

TokenizerException::TokenizerException() {
    this->message = "Invalid token!";
}

const char* TokenizerException::what() const noexcept {
	return this->message.c_str();
}