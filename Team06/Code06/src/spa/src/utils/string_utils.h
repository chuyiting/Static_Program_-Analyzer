#pragma once
//
// Created by Stephen Tan  Hin Khai on 16/9/22.
//
#include <string>

namespace string_utils {
    // default args
    inline const char *ws = " \t\n\r\f\v";
    inline const char *wildcard = "_";

    inline std::string &rtrim(std::string &s, const char *t = ws) {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

// trim from beginning of string (left)
    inline std::string &ltrim(std::string &s, const char *t = ws) {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

// trim from both ends of string (right then left)
    inline std::string &trim(std::string &s, const char *t = wildcard) {
        return ltrim(rtrim(s, t), t);
    }

// trim parentheses
    inline std::string &trimParentheses(std::string &s) {
        s.erase( std::remove_if(s.begin(), s.end(),
                                [](char ch){ return ch=='(' || ch ==')'; }),s.end() );
        return s;
    }

// trim whitespaces
    inline std::string &trimWhitespace(std::string &s) {
      s.erase(std::remove_if(s.begin(), s.end(), isspace), s.end());

      return s;
    }
}