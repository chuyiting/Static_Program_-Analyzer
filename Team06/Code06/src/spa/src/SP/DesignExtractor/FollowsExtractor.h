#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_set>

#include "PKB/PKBSetter.h"
#include "SP/DesignExtractor/Extractables.h"


// Extracts from a class that inherits FollowsExtractable
class FollowsExtractor {

private:

    // Reference to PKB
    std::shared_ptr<PKBSetter> _pkbSetter;

public:

    FollowsExtractor();

    FollowsExtractor(std::shared_ptr<PKBSetter> pkbSetter);

    void extract(std::shared_ptr<FollowsExtractable> followsExtractable);

};
