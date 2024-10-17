#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

#include "PKB/PKBSetter.h"
#include "SP/DesignExtractor/Extractables.h"


// Extracts from a class that inherits CallsExtractable
class CallsExtractor {

private:

    // Reference to PKB
    std::shared_ptr<PKBSetter> _pkbSetter;

public:

    CallsExtractor();

    CallsExtractor(std::shared_ptr<PKBSetter> pkbSetter);

    void extract(std::shared_ptr<CallsExtractable> callsExtractable);

};
