//
// Created by Eddy Chu on 2022/10/13.
//

#ifndef SPA_SYNONYMDECLSPARSER_H
#define SPA_SYNONYMDECLSPARSER_H

#include "QPSAbstractParser.h"

class SynonymDeclsParser : public QPSAbstractParser {
  TokenIterator tryParseSynonymDecl(std::shared_ptr<QPSQueryParserAPI> result,
                                    TokenIterator begin, TokenIterator end);

  EntityType getEntityType(TokenIterator it);
  void checkNoRepeatedSynonym(Synonym &synonym,
                              std::shared_ptr<QPSQueryParserAPI> result);

public:
  TokenIterator tryParse(std::shared_ptr<QPSQueryParserAPI> result,
                         TokenIterator begin, TokenIterator end) override;
};

#endif // SPA_SYNONYMDECLSPARSER_H
