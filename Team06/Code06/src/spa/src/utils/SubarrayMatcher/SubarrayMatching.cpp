#include "utils/SubarrayMatcher/SubarrayMatching.h"


size_t SubarrayMatching::_hashString(std::string str) {
    std::hash<std::string> str_hash;
    return _calcHash(str_hash(str));
}

int SubarrayMatching::_calcHash(int val) {
    return (val % _primeNumber + _primeNumber) % _primeNumber;
}

// Checks if pattern[0...M-1] = text[s, s+1, ...s+M-1]
bool SubarrayMatching::_iterativeCheck(int s) {
    for (int i = 0; i < pattern.size(); i++) {
        if (pattern.at(i) != text.at(s + i)) {
            return false;
        }
    }
    return true;
}


// Method that checks if pattern is a subarray of text.
// Modified implementation of Rabin-Karp algorithm.
bool SubarrayMatching::isSubarray(std::vector<std::string> pattern, std::vector<std::string> text) {

    this->pattern = pattern;
    this->text = text;

    int patternLength = pattern.size();
    int textLength = text.size();

    if (patternLength > textLength) {
        return false;
    }

    // Hashes and offset
    int offset = 1;
    int hashPattern = 0;
    int rollingHash = 0;

    for (int i = 0; i < patternLength; i++) {
        offset = _calcHash(offset * 2);
        hashPattern = _calcHash(hashPattern * 2 + _hashString(pattern[i]));
        rollingHash = _calcHash(rollingHash * 2 + _hashString(text[i]));
    }
    
    int toExclude;
    int toInclude;
    
    for (int s = 0; s < textLength - patternLength + 1; s++) {  // check n - m + 1 times

        if (hashPattern == rollingHash) {  // hash collision, possibly a match
            if (_iterativeCheck(s)) {
                return true;
            }
        }

        if (s < textLength - patternLength) {  // roll
            toExclude = _hashString(text[s]) * offset;
            toInclude = _hashString(text[s + patternLength]);
            rollingHash = _calcHash(rollingHash * 2 - toExclude + toInclude);
        }
    }

    return false;
}
