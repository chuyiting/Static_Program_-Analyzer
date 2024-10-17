//
// Created by Stephen Tan  Hin Khai on 7/10/22.
//

#ifndef SPA_SRC_UNIT_TESTING_SRC_PKB_RESOLVER_STUBUSESSTORAGE_H_
#define SPA_SRC_UNIT_TESTING_SRC_PKB_RESOLVER_STUBUSESSTORAGE_H_

#include "PKB/Datastore/UsesStorage.h"

//"procedure foo {"
//"	while ((1 == 2) && (2 == 3)) {" // 1
//"		b = 2;" // 2
//"		while ((4 == 5) || (5 == 5)) {" // 3
//"			d = 7 - 8 / 4;" // 4
//"		}"
//"	}"
//"}";
class StubUsesStorageOnlyConst : public UsesStorage {
public:
  StubUsesStorageOnlyConst() : UsesStorage() {
    addUsesP("foo", {});
    addUsesS("1", {});
    addUsesS("2", {}); // assignment
    addUsesS("3", {}); // container
    addUsesS("4", {}); // assignment
  }
};

#endif // SPA_SRC_UNIT_TESTING_SRC_PKB_RESOLVER_STUBUSESSTORAGE_H_
