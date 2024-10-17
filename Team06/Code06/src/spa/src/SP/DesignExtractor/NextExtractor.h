
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>

#include "PKB/PKBSetter.h"
#include "SP/DesignExtractor/Extractables.h"


// Extracts from a class that inherits FollowsExtractable
class NextExtractor {

private:

    // Reference to PKB
    std::shared_ptr<PKBSetter> _pkbSetter;

public:

    NextExtractor();

    NextExtractor(std::shared_ptr<PKBSetter> pkbSetter);

    void extract(std::shared_ptr<NextExtractable> nextExtractable);

};