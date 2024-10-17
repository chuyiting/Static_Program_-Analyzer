//
// Created by Eddy Chu on 2022/10/13.
//

#ifndef SPA_WITHCLAUSEPARSER_H
#define SPA_WITHCLAUSEPARSER_H

#include "QPSAbstractParser.h"

class WithClauseParser : public QPSAbstractParser {
  static TokenIterator
  tryParseAttrCompare(std::shared_ptr<QPSQueryParserAPI> result,
                      TokenIterator begin, TokenIterator end);

  static std::pair<std::shared_ptr<WithRef>, TokenIterator>

  tryParseWithRef(std::shared_ptr<QPSQueryParserAPI> result,
                  TokenIterator begin, TokenIterator end);

public:
  TokenIterator tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                         TokenIterator begin, TokenIterator end) override;
};

#endif // SPA_WITHCLAUSEPARSER_H
