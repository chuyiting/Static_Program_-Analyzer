//
// Created by Eddy Chu on 2022/9/8.
//

#ifndef SPA_EXPRESSIONSPEC_H
#define SPA_EXPRESSIONSPEC_H

#include "QPS/evaluator/relationships/relationshipStructs/EntRef.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include <string>

class ExpressionSpec {

public:
  enum FactorType { WILDCARD, EXPRESSION };

  /**
   * Constructor for wildcard expression spec
   */
  ExpressionSpec();

  /**
   * Constructor for general expression spec
   * @param isPartialMatch
   * @param expression
   */
  ExpressionSpec(bool isPartialMatch, const std::string &expression);

  bool operator==(const ExpressionSpec &other) const {
    switch (type) {
    case WILDCARD:
      return other.type == WILDCARD;
    case EXPRESSION:
      return isPartialMatch == other.isPartialMatch && factor == other.factor;
    default:
      return false;
    }
  };

  std::string getExpression();

  bool operator!=(const ExpressionSpec &other) const {
    return !((*this) == other);
  };

private:
  bool isPartialMatch;
  std::string factor;
  FactorType type;
};

#endif // SPA_EXPRESSIONSPEC_H
