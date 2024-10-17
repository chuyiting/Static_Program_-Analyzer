#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

class SubarrayMatching {

    private:
        int _primeNumber = 10007;
        std::vector<std::string> pattern;
        std::vector<std::string> text;

        size_t _hashString(std::string str);
        int _calcHash(int val);
        bool _iterativeCheck(int s);

    public:
        bool isSubarray(std::vector<std::string> subarray, std::vector<std::string> array);
};
