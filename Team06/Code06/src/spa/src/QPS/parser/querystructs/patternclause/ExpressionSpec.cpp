//
// Created by Eddy Chu on 2022/9/8.
//

#include "ExpressionSpec.h"

ExpressionSpec::ExpressionSpec() {
  type = FactorType::WILDCARD;
  isPartialMatch = true;
}

ExpressionSpec::ExpressionSpec(bool isPartialMatch,
                               const std::string &expression)
    : isPartialMatch(isPartialMatch), factor(expression) {
  type = FactorType::EXPRESSION;
}

std::string ExpressionSpec::getExpression() {
  if (this->type == WILDCARD) {
    return "_";
  }

  if (isPartialMatch) {
    return "_" + this->factor + "_";
  } else {
    return this->factor;
  }
}
