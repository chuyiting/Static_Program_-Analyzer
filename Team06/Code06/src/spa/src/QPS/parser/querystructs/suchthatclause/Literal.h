//
// Created by Eddy Chu on 2022/9/3.
//

#ifndef SPA_LITERAL_H
#define SPA_LITERAL_H

#include "QPS/tokenizer/QueryToken.h"
#include "QPS/types/EntityType.h"
#include "RelRef.h"
#include <string>

class Literal : public RelRef {
public:
  enum LiteralType { STMT, ENTITY };

  /**
   * Constructor of Literal class. It makes sure the format of Literal meets
   * requirement <i> STMT Literal is integer that is greater than 0 <ii> ENTITY
   * Literal is wrapped by quotation marks ,and the content is alphanum and does
   * not start with number
   * @param token
   * @throw QPSException if the format of Literal is invalid
   */
  explicit Literal(const QueryToken &token);

  LiteralType getType() const;

  const std::string &getVal() const;

  RelRefType getRefType() const override;

  bool operator==(const Literal &other) const;

  bool operator!=(const Literal &other) const;

private:
  LiteralType type;
  std::string val;
};

#endif // SPA_LITERAL_H
