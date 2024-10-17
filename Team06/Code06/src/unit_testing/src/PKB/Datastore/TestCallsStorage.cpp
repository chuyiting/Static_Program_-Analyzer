//
// Created by Stephen Tan  Hin Khai on 2/10/22.
//
#include "PKB/Datastore/CallStorage.h"
#include "catch.hpp"

using tuple = std::pair<std::string, std::unordered_set<std::string>>;
using calls = std::unordered_set<std::string>;
/**
 * Recursive and cyclic calls are not allowed. --> DAG is allowed
 * E.g. Procedure A calls procedure B, procedure B calls procedure C, and
 * procedure C calls procedure A. E.g. Procedure A calls procedure A. x(){}
 * y(){}
 * foo() {
 * call bar
 * call tau
 * call soh
 * }
 * bar(){
 * call tau
 * call soh
 * }
 * tau() {
 * call soh
 * call rho
 * }
 * soh() {}
 */
TEST_CASE("Call Storage Relations 1") {
  CallStorage call_storage = CallStorage();
  call_storage.addProcedure("x");
  call_storage.addProcedure("y");
  calls fooCalls = {"bar", "tau", "soh"};
  calls barCalls = {"tau", "soh"};
  calls tauCalls = {"soh", "rho"};
  calls muCalls = {"tau"};
  for (const auto &v : {tuple("foo", fooCalls), tuple("bar", barCalls),
                        tuple("tau", tauCalls), tuple("mu", muCalls)}) {
    std::string caller = v.first;
    for (const auto &p : v.second) {
      call_storage.addCall(caller, p);
    }
  }

  SECTION("getCalled") {
    REQUIRE(call_storage.getCalled("bar") == calls({"foo"}));
    REQUIRE(call_storage.getCalled("tau") == calls({"foo", "bar", "mu"}));
    REQUIRE(call_storage.getCalled("soh") == calls({"foo", "bar", "tau"}));
    REQUIRE(call_storage.getCalled("mu") == calls({}));
    // negative testcase
    REQUIRE(call_storage.getCalled("foobar") == calls({}));
  }

  SECTION("getCalls") {
    REQUIRE(call_storage.getCalls("foo") == fooCalls);
    REQUIRE(call_storage.getCalls("bar") == barCalls);
    REQUIRE(call_storage.getCalls("tau") == tauCalls);
    REQUIRE(call_storage.getCalls("mu") == muCalls);
    REQUIRE(call_storage.getCalls("soh") == calls({}));
    // Invalid input
    REQUIRE(call_storage.getCalls("Bedok") == calls({}));
  }

  SECTION("getCallsT") {
    REQUIRE(call_storage.getCallsT("foo") ==
            calls({"bar", "tau", "soh", "rho"}));
    REQUIRE(call_storage.getCallsT("bar") == calls({"tau", "soh", "rho"}));
    REQUIRE(call_storage.getCallsT("soh") == calls({}));
    REQUIRE(call_storage.getCallsT("tau") == calls({"soh", "rho"}));
    REQUIRE(call_storage.getCallsT("mu") == calls({"tau", "soh", "rho"}));
    // Invalid input
    REQUIRE(call_storage.getCallsT("Bedok") == calls({}));
  }

  SECTION("getCalledT") {
    REQUIRE(call_storage.getCalledT("foo") == calls({}));
    REQUIRE(call_storage.getCalledT("bar") == calls({"foo"}));
    REQUIRE(call_storage.getCalledT("soh") ==
            calls({"tau", "bar", "foo", "mu"}));
    REQUIRE(call_storage.getCalledT("tau") == calls({"bar", "foo", "mu"}));
    REQUIRE(call_storage.getCalledT("mu") == calls({}));
    // Invalid input
    REQUIRE(call_storage.getCalledT("Bedok") == calls({}));
  }

  SECTION("Direct Calls") {
    // Recursive
    REQUIRE_FALSE(call_storage.isCallsDirect("x", "x"));
    REQUIRE_FALSE(call_storage.isCallsDirect("Bedok", "Bedok"));

    // Non existing links
    REQUIRE_FALSE(call_storage.isCallsDirect("x", "y"));
    REQUIRE_FALSE(call_storage.isCallsDirect("x", "foo"));
    REQUIRE_FALSE(call_storage.isCallsDirect("soh", "foo"));
    REQUIRE_FALSE(call_storage.isCallsDirect("Bedok", "foo"));
    REQUIRE_FALSE(call_storage.isCallsDirect("foo", "Bedok"));

    REQUIRE(call_storage.isCallsDirect("foo", "soh"));
    REQUIRE(call_storage.isCallsDirect("mu", "tau"));
    REQUIRE(call_storage.isCallsDirect("tau", "soh"));
    REQUIRE(call_storage.isCallsDirect("foo", "bar")); // 3, 5
    REQUIRE(call_storage.isCallsDirect("bar", "tau"));

    REQUIRE_FALSE(call_storage.isCallsDirect("tau", "bar"));
    REQUIRE_FALSE(call_storage.isCallsDirect("tau", "mu"));
    REQUIRE_FALSE(call_storage.isCallsDirect("foo", "mu"));
  }

  SECTION("Direct CallT") {
    REQUIRE_FALSE(call_storage.isCallsDirect("x", "x"));
    REQUIRE_FALSE(call_storage.isCallsDirect("Bedok", "Bedok"));

    REQUIRE(call_storage.isCallsTDirect("bar", "rho"));
    REQUIRE(call_storage.isCallsTDirect("mu", "rho"));
    REQUIRE(call_storage.isCallsTDirect("foo", "soh"));
    REQUIRE(call_storage.isCallsTDirect("mu", "soh"));

    REQUIRE_FALSE(call_storage.isCallsTDirect("rho", "bar"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("x", "rho"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("foo", "x"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("x", "foo"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("foo", "mu"));
  }

  SECTION("AnyLeft Calls") {
    // invalid input
    REQUIRE_FALSE(call_storage.isCallsLeft("Bedok"));

    // intended behavior
    REQUIRE_FALSE(call_storage.isCallsLeft("foo"));
    REQUIRE_FALSE(call_storage.isCallsLeft("mu"));

    REQUIRE(call_storage.isCallsLeft("soh"));
    REQUIRE(call_storage.isCallsLeft("tau"));
    REQUIRE(call_storage.isCallsLeft("bar"));
  }

  SECTION("AnyLeft CallsT") {
    // invalid input
    REQUIRE_FALSE(call_storage.isCallsLeft("Bedok"));

    // intended behavior
    REQUIRE_FALSE(call_storage.isCallsLeft("foo"));
    REQUIRE_FALSE(call_storage.isCallsLeft("mu"));

    REQUIRE(call_storage.isCallsLeft("soh"));
    REQUIRE(call_storage.isCallsLeft("tau"));
    REQUIRE(call_storage.isCallsLeft("bar"));
  }

  SECTION("AnyRight Calls") {
    REQUIRE_FALSE(call_storage.isCallsRight("soh"));
    REQUIRE_FALSE(call_storage.isCallsRight("rho"));
    REQUIRE_FALSE(call_storage.isCallsRight("Bedok"));

    REQUIRE(call_storage.isCallsRight("tau"));
    REQUIRE(call_storage.isCallsRight("bar"));
    REQUIRE(call_storage.isCallsRight("foo"));
    REQUIRE(call_storage.isCallsRight("mu"));
  }

  SECTION("AnyRight CallsT") {
    REQUIRE_FALSE(call_storage.isCallsRight("soh"));
    REQUIRE_FALSE(call_storage.isCallsRight("rho"));
    REQUIRE_FALSE(call_storage.isCallsRight("Bedok"));

    REQUIRE(call_storage.isCallsRight("tau"));
    REQUIRE(call_storage.isCallsRight("bar"));
    REQUIRE(call_storage.isCallsRight("foo"));
    REQUIRE(call_storage.isCallsRight("mu"));
  }

  SECTION("Any Calls/T") { REQUIRE(call_storage.isCallsExists()); }
}

TEST_CASE("Call Storage Relations no calls") {
  CallStorage call_storage = CallStorage();
  calls procedures = {"x", "y", "z", "a", "b", "c", "d", "e"};
  for (auto p : procedures) {
    call_storage.addProcedure(p);
  }

  SECTION("getCalls") {
    REQUIRE(call_storage.getCalls("x") == calls({}));
    REQUIRE(call_storage.getCalls("y") == calls({}));
    REQUIRE(call_storage.getCalls("z") == calls({}));
    REQUIRE(call_storage.getCalls("a") == calls({}));
    REQUIRE(call_storage.getCalls("b") == calls({}));
    REQUIRE(call_storage.getCalls("c") == calls({}));
  }

  SECTION("getCallsT") {
    REQUIRE(call_storage.getCallsT("x") == calls({}));
    REQUIRE(call_storage.getCallsT("y") == calls({}));
    REQUIRE(call_storage.getCallsT("z") == calls({}));
    REQUIRE(call_storage.getCallsT("a") == calls({}));
    REQUIRE(call_storage.getCallsT("b") == calls({}));
    REQUIRE(call_storage.getCallsT("c") == calls({}));
  }

  SECTION("getCalled") {
    REQUIRE(call_storage.getCalled("x") == calls({}));
    REQUIRE(call_storage.getCalled("y") == calls({}));
    REQUIRE(call_storage.getCalled("z") == calls({}));
    REQUIRE(call_storage.getCalled("a") == calls({}));
    REQUIRE(call_storage.getCalled("b") == calls({}));
    REQUIRE(call_storage.getCalled("c") == calls({}));
  }

  SECTION("getCalledT") {
    REQUIRE(call_storage.getCalledT("x") == calls({}));
    REQUIRE(call_storage.getCalledT("y") == calls({}));
    REQUIRE(call_storage.getCalledT("z") == calls({}));
    REQUIRE(call_storage.getCalledT("a") == calls({}));
    REQUIRE(call_storage.getCalledT("b") == calls({}));
    REQUIRE(call_storage.getCalledT("c") == calls({}));
  }

  SECTION("Direct Calls") {
    REQUIRE_FALSE(call_storage.isCallsDirect("x", "y"));
    REQUIRE_FALSE(call_storage.isCallsDirect("a", "b"));
    REQUIRE_FALSE(call_storage.isCallsDirect("b", "a"));
    REQUIRE_FALSE(call_storage.isCallsDirect("c", "d"));
    REQUIRE_FALSE(call_storage.isCallsDirect("d", "e"));
  }

  SECTION("Direct CallT") {
    REQUIRE_FALSE(call_storage.isCallsTDirect("x", "y"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("a", "b"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("b", "a"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("c", "d"));
    REQUIRE_FALSE(call_storage.isCallsTDirect("d", "e"));
  }

  SECTION("AnyLeft Calls") {
    REQUIRE_FALSE(call_storage.isCallsLeft("x"));
    REQUIRE_FALSE(call_storage.isCallsLeft("y"));
    REQUIRE_FALSE(call_storage.isCallsLeft("a"));
    REQUIRE_FALSE(call_storage.isCallsLeft("b"));
    REQUIRE_FALSE(call_storage.isCallsLeft("c"));
  }

  SECTION("AnyLeft CallsT") {
    REQUIRE_FALSE(call_storage.isCallsTLeft("x"));
    REQUIRE_FALSE(call_storage.isCallsTLeft("y"));
    REQUIRE_FALSE(call_storage.isCallsTLeft("a"));
    REQUIRE_FALSE(call_storage.isCallsTLeft("b"));
    REQUIRE_FALSE(call_storage.isCallsTLeft("c"));
  }

  SECTION("AnyRight Calls") {
    REQUIRE_FALSE(call_storage.isCallsRight("x"));
    REQUIRE_FALSE(call_storage.isCallsRight("y"));
    REQUIRE_FALSE(call_storage.isCallsRight("a"));
    REQUIRE_FALSE(call_storage.isCallsRight("b"));
    REQUIRE_FALSE(call_storage.isCallsRight("c"));
  }

  SECTION("AnyRight CallsT") {
    REQUIRE_FALSE(call_storage.isCallsTRight("x"));
    REQUIRE_FALSE(call_storage.isCallsTRight("y"));
    REQUIRE_FALSE(call_storage.isCallsTRight("a"));
    REQUIRE_FALSE(call_storage.isCallsTRight("b"));
    REQUIRE_FALSE(call_storage.isCallsTRight("c"));
  }

  SECTION("Any Calls/T") {
    // honestly this was the main point
    REQUIRE_FALSE(call_storage.isCallsExists());
  }
}
