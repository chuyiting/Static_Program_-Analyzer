//
// Created by Chong Jun Wei on 8/10/22.
//

#ifndef SPA_STUBPATTERNSTORAGE2_H
#define SPA_STUBPATTERNSTORAGE2_H

#include "PKB/Datastore/PatternStorage.h"

// procedure p {
//   1.  if (1 == 2) then {
//   2.      while (3 == 4) {
//   3.          x = 1; } }
//       else {
//   4.      if (x == 1) then {
//   5.          while (y == x) {
//   6.              z = 2; } }
//           else {
//   7.          y = 3;
//   } } } }

// if/while pattern
class StubPatternStorage2 : public PatternStorage {
public:
  StubPatternStorage2() : PatternStorage() {
    usesIf = {{"4", {"x"}}};

    usesWhile = {{"5", {"y", "x"}}};
  }
};

#endif // SPA_STUBPATTERNSTORAGE2_H
