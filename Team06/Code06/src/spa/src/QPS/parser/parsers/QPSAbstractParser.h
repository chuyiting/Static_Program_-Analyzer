//
// Created by Eddy Chu on 2022/10/13.
//

#ifndef SPA_QPSABSTRACTPARSER_H
#define SPA_QPSABSTRACTPARSER_H

#include "QPS/parser/QPSParser.h"
#include "QPS/parser/QPSQuery.h"
#include "QPS/tokenizer/QueryToken.h"

class QPSAbstractParser {
public:
  using TokenIterator = std::vector<QueryToken>::const_iterator;

  virtual TokenIterator tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                                 TokenIterator begin, TokenIterator end) = 0;
};

#endif // SPA_QPSABSTRACTPARSER_H
