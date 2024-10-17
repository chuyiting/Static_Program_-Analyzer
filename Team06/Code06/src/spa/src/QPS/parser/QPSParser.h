//
// Created by Eddy Chu on 2022/8/31.
//

#ifndef INC_22S1_CP_SPA_TEAM_06_QPSPARSER_H
#define INC_22S1_CP_SPA_TEAM_06_QPSPARSER_H

#include "QPS/parser/QPSQuery.h"
#include "QPS/parser/parsers/QPSAbstractParser.h"
#include "QPS/parser/querystructs/selectclause/SelectClause.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/tokenizer/QueryToken.h"
#include <vector>

class QPSParser {
public:
  static std::shared_ptr<QPSQuery> parse(const std::vector<QueryToken> &tokens);
};

#endif // INC_22S1_CP_SPA_TEAM_06_QPSPARSER_H
