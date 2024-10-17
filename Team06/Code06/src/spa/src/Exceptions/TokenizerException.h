#pragma once

#include <stdexcept>
#include <string>

class TokenizerException : public std::exception {
    public:
        std::string message;
        TokenizerException();
        [[nodiscard]] const char* what() const noexcept override;
};
