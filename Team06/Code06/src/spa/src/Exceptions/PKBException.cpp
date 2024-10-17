//
// Created by Stephen Tan  Hin Khai on 1/10/22.
//

#include "PKBException.h"

#include <utility>

const char *PKBException::what() const noexcept {
  return message.c_str();
}

PKBException::PKBException(std::string message, PKBExceptionType type) noexcept: message(std::move(message)), type(type) {}

