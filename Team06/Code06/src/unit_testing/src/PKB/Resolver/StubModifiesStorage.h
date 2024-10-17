//
// Created by Chong Jun Wei on 9/10/22.
//

#ifndef SPA_SRC_UNIT_TESTING_SRC_PKB_RESOLVER_STUBMODIFIESSTORAGE_H_
#define SPA_SRC_UNIT_TESTING_SRC_PKB_RESOLVER_STUBMODIFIESSTORAGE_H_

#include "PKB/Datastore/ModifiesStorage.h"

/*
 * procedure foo {
 *  call bar;
 *  x = 3;
 * }
 */
class StubModifiesStorage : public ModifiesStorage {
public:
  StubModifiesStorage() : ModifiesStorage() {
    this->addModifiesS("2", {"x"});
    this->addModifiesP("foo", {"x"});
  }
};

#endif // SPA_SRC_UNIT_TESTING_SRC_PKB_RESOLVER_STUBMODIFIESSTORAGE_H_
