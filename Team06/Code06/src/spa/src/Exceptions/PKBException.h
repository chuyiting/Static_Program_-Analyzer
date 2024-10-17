//
// Created by Stephen Tan  Hin Khai on 1/10/22.
//

#ifndef SPA_SRC_SPA_SRC_PKB_PKBEXCEPTION_H_
#define SPA_SRC_SPA_SRC_PKB_PKBEXCEPTION_H_

#include <string>
#include <exception>
#include <ostream>

enum class PKBExceptionType {
    // UNKNOWN is reserved for only special cases, use it with caution
    // DEFENSIVE is used for functions that by all means should not throw, except within unittesting
    // PARSING is defensive and also used to test the parsing of inputs
    STORAGE, PARSING, UNKNOWN
};

class PKBException : public virtual std::exception {
    std::string message;
    PKBExceptionType type;
  public:
    explicit PKBException(std::string message, PKBExceptionType type = PKBExceptionType::STORAGE) noexcept;

    [[nodiscard]] const char *what() const noexcept override;

    operator std::string() const {
      switch (type) {
	case PKBExceptionType::STORAGE:
	  return message;
	case PKBExceptionType::PARSING:
	  return message;
	default:
	  return "";
      }
    }

};

namespace PKBStorageExceptionMessage {
    constexpr auto PKB_STORAGE_INVALID_INPUT = "Invalid Input Range";
    constexpr auto PKB_STORAGE_OUT_OF_RANGE = "Range Input is out of scope of the program";
    constexpr auto PKB_STORAGE_PROCEDURE_DOES_NOT_EXIST = "This Procedure Entity does not exist";
}

#endif //SPA_SRC_SPA_SRC_PKB_PKBEXCEPTION_H_
