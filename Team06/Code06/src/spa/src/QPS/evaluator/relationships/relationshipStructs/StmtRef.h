//
// Created by Eddy Chu on 2022/9/3.
//

#ifndef SPA_STMTREF_H
#define SPA_STMTREF_H

#include "QPS/parser/querystructs/suchthatclause/Literal.h"
#include "QPS/parser/querystructs/suchthatclause/RelRef.h"
#include "QPS/parser/querystructs/suchthatclause/Synonym.h"
#include "QPS/evaluator/QPSConstraint.h"
#include <cstdint>
#include <string>
#include <memory>

/**
 * StmtRef is either an unsigned integer or a synonym
 */
class StmtRef {

public:
  static const int DEFAUlT_STMTNUM = -1;

  enum StmtRefType { SYNONYM, WILDCARD, INTEGER };

  /**
   * The major constructor for statement reference. It checks the validity of
   * the value. Valid inputs include: Synonym: statement, assign, while, if,
   * print, read Wildcard Literal: integer that is greater than equal to 0
   * `QPSException` thrown if the input is invalid
   * @param ref
   */
  explicit StmtRef(std::shared_ptr<RelRef> ref);

  StmtRef();

  /**
   * Get the string representation of the stmtref
   * <b>This method serves as a bridge method between QPS and PKB API</b>
   * If the stmtref is a synonym, a synonym name is returned
   * If the stmtref is a literal, the statement number is returned
   * @return
   */
  std::string getStmtString();

  /**
   * Get the string value of the stmtRef
   * Return the assigned value if stmtRef is of SYNONYM type
   * Return literal if entref is of LITERAL type
   * Return _ if entref of WILDCARD type
   * @param assign
   * @return
   */
  std::string getStmtValue(assign_t &assign);

  bool operator==(const StmtRef &other);

  bool operator==(const synonym_t &synonym);

  bool operator!=(const StmtRef &other);

  [[nodiscard]] StmtRefType getType() const;

  [[nodiscard]] const Synonym &getSynonym() const;

  // Constructor used only for unit testing
  explicit StmtRef(const Synonym &syn) : synonym(syn) {
    this->type = StmtRefType::SYNONYM;
  }

  // Constructor used only for unit testing
  explicit StmtRef(uint64_t num) : stmtNum(num) {
    this->type = StmtRefType::INTEGER;
  }

  // Constructor used only for unit testing
  StmtRef(uint64_t stmtNum, const Synonym &synonym, StmtRefType type);

  [[nodiscard]] bool isSynonym() const;

  [[nodiscard]] bool isWildcard() const;

private:
  uint64_t stmtNum = DEFAUlT_STMTNUM;
  Synonym synonym;
  StmtRef::StmtRefType type;

  static void checkStmtRefValidity(std::shared_ptr<Synonym> synonym);

  static void checkStmtRefValidity(std::shared_ptr<Literal> literal);
};

#endif // SPA_STMTREF_H