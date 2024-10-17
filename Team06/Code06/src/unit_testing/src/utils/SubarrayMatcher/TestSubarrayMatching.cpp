#include "catch.hpp"

#include "utils/SubarrayMatcher/SubarrayMatching.h"

TEST_CASE("checkValidSubarray") {
  std::vector<std::string> pattern = {"a", "b", "c"};
  std::vector<std::string> text = {"a", "b", "c", "d", "e", "f"};

  SubarrayMatching subarrayMatching;
  REQUIRE(subarrayMatching.isSubarray(pattern, text) == true);
}

TEST_CASE("checkValidSubarray2") {
  std::vector<std::string> pattern = {"a", "+", "b-c", "*", "c"};
  std::vector<std::string> text = {"x", "y",   "z", "asd+", "a",
                                   "+", "b-c", "*", "c",    "g2x"};

  SubarrayMatching subarrayMatching;
  REQUIRE(subarrayMatching.isSubarray(pattern, text) == true);
}

TEST_CASE("checkInvalidSubarray") {
  std::vector<std::string> pattern = {"a", "b", "c"};
  std::vector<std::string> text = {"a", "b", "d", "e", "f"};

  SubarrayMatching subarrayMatching;
  REQUIRE(subarrayMatching.isSubarray(pattern, text) == false);
}

TEST_CASE("checkInvalidSubarray2") {
  std::vector<std::string> pattern = {"a+1", "b+2", "c"};
  std::vector<std::string> text = {"b+2", "a+1", "c", "d+c", "a+1", "c", "g"};

  SubarrayMatching subarrayMatching;
  REQUIRE(subarrayMatching.isSubarray(pattern, text) == false);
}
