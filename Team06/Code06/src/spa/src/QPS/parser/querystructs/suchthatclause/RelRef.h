//
// Created by Eddy Chu on 2022/9/3.
//

#ifndef SPA_RELREF_H
#define SPA_RELREF_H

enum class RelRefType { LITERAL, SYNONYM, WILDCARD };

class RelRef {
public:
  virtual RelRefType getRefType() const = 0;
};

class Wildcard : public RelRef {
public:
  RelRefType getRefType() const override { return RelRefType::WILDCARD; }
};

#endif // SPA_RELREF_H
