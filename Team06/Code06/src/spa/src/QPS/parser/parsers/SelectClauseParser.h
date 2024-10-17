//
// Created by Eddy Chu on 2022/10/13.
//

#ifndef SPA_SELECTCLAUSEPARSER_H
#define SPA_SELECTCLAUSEPARSER_H

#include "QPSAbstractParser.h"

class SelectClauseParser : public QPSAbstractParser {
private:
  TokenIterator tryParseSelectTarget(std::shared_ptr<QPSQueryParserAPI> result,
                                     TokenIterator begin, TokenIterator end);

  inline static const std::string KEYWORD_SELECT = "Select";
  inline static const std::string KEYWORD_BOOLEAN = "BOOLEAN";

public:
  TokenIterator tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                         TokenIterator begin, TokenIterator end) override;
};

#endif // SPA_SELECTCLAUSEPARSER_H
