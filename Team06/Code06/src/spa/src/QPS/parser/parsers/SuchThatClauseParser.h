//
// Created by Eddy Chu on 2022/10/13.
//

#ifndef SPA_SUCHTHATCLAUSEPARSER_H
#define SPA_SUCHTHATCLAUSEPARSER_H

#include "QPSAbstractParser.h"

class SuchThatClauseParser : public QPSAbstractParser {
public:
  TokenIterator tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                         TokenIterator begin, TokenIterator end) override;
};

#endif // SPA_SUCHTHATCLAUSEPARSER_H
