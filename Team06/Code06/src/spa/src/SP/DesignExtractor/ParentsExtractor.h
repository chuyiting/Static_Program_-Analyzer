#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>

#include "PKB/PKBSetter.h"
#include "SP/DesignExtractor/Extractables.h"


// Extracts from a class that inherits ParentsExtractable
class ParentsExtractor {

private:

    // Reference to PKB
    std::shared_ptr<PKBSetter> _pkbSetter;

public:

    ParentsExtractor();

    ParentsExtractor(std::shared_ptr<PKBSetter> pkbSetter);

    void extract(std::shared_ptr<ParentsExtractable> parentsExtractable);

};
