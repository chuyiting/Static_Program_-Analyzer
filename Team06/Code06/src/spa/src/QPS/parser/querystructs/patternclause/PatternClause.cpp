//
// Created by Eddy Chu on 2022/9/8.
//

#include "PatternClause.h"
#include "QPS/evaluator/CSPUtils.h"

PatternClause::PatternClause(const Synonym &syn, const EntRef &lhs,
                             const ExpressionSpec &rhs)
    : syn(syn), lhs(lhs), rhs(rhs) {
  checkSynonym(syn);
  checkLhs(lhs);
}

bool PatternClause::isSatisfied(std::shared_ptr<PKBGetter> pkb,
                                assign_t &assign) {
  std::string synValue = assign[syn.getSynonymName()];
  std::string ent = lhs.getEntString();

  if (lhs.getType() == EntRef::SYNONYM) {
    ent = assign[ent];
  }

  // value unassigned evaluates to true
  if (synValue == CSPUtils::UNASSIGNED || ent == CSPUtils::UNASSIGNED) {
    return true;
  }
  std::string right = rhs.getExpression();

  return pkb->isPatternMatches(
      synValue, entityTypeToStmtType(syn.getSynonymType()), ent, right);
};

std::vector<synonym_t> PatternClause::getVariables() {
  std::vector<synonym_t> result;
  result.push_back(syn.getSynonymName());
  if (lhs.getType() == EntRef::EntRefType::SYNONYM) {
    result.push_back(lhs.getSynonym().getSynonymName());
  }

  return result;
}

void PatternClause::checkSynonym(const Synonym &syn) {
  if (validPatternSyn.find(syn.getSynonymType()) == validPatternSyn.end()) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX,
        QpsExceptionType::SEMANTIC);
  }
}

void PatternClause::checkLhs(const EntRef &ent) {
  if (ent.isSynonym() && !ent.isSynonymType(EntityType::VARIABLE)) {
    throw QPSException(
        QPSParseExceptionMessage::QPS_PARSER_INVALID_PATTERN_CLAUSE_SYNTAX,
        QpsExceptionType::SEMANTIC);
  }
}

bool PatternClause::queryDomain(std::shared_ptr<PKBGetter> pkb,
                                assign_t &assign, orderDomain_t &orderDomain,
                                orderDomain_t &queryResult) {
  return false;
}

int PatternClause::priority() { return CSPUtils::PRIORITY_PATTERN_CLAUSE; }
