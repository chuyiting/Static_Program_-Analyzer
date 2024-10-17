//
// Created by Stephen Tan  Hin Khai on 15/9/22.
//

#include "Resolver.h"

Resolver::Resolver() = default;

void Resolver::resolve(const Entity &lhs, const Entity &rhs) {
  this->_lhs_value = lhs;
  this->_rhs_value = rhs;

  _lhs_type = ArgumentType::UNINITIALISED;
  _rhs_type = ArgumentType::UNINITIALISED;

  for (const std::pair<std::regex, ArgumentType> &rule : _regexRules) {
    if (_lhs_type == ArgumentType::UNINITIALISED &&
        std::regex_match(_lhs_value, rule.first)) {
      _lhs_type = rule.second;
    }
    if (_rhs_type == ArgumentType::UNINITIALISED &&
        std::regex_match(_rhs_value, rule.first)) {
      _rhs_type = rule.second;
    }
  }
}
