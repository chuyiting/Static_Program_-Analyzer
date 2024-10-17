//
// Created by Eddy Chu on 2022/9/1.
//

#include "QPSException.h"

const char *QPSException::what() const noexcept { return message.c_str(); }

QPSException::QPSException(const std::string &message,
                           QpsExceptionType type) noexcept
    : message(message), type(type) {}

QpsExceptionType QPSException::getType() const { return type; }
