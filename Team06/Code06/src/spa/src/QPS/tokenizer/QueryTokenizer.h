#pragma once

#include "QPS/exceptions/QPSException.h"
#include "QueryToken.h"
#include <string>
#include <vector>

class QueryTokenizer {
  using StrIter = std::string::const_iterator;

  static std::pair<QueryToken, StrIter> next_token(StrIter begin, StrIter end);

public:
  static std::vector<QueryToken> tokenize(const std::string &queryString);
};