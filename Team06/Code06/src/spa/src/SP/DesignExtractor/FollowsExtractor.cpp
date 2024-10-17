#include "SP/DesignExtractor/FollowsExtractor.h"


FollowsExtractor::FollowsExtractor() = default;

FollowsExtractor::FollowsExtractor(std::shared_ptr<PKBSetter> pkbSetter) {
    // Reference to PKB
    _pkbSetter = pkbSetter;
}

void FollowsExtractor::extract(std::shared_ptr<FollowsExtractable> followsExtractable) {

    std::vector<std::vector<int>> setsOfFollowsStatements = followsExtractable->getFollowsStatements();

    for (std::vector<int> setOfFollowsStatements: setsOfFollowsStatements) {

        // Convert to vector of strings
        std::vector<std::string> stringSetOfFollowsStatements;
        for (int statement: setOfFollowsStatements) {
            stringSetOfFollowsStatements.push_back(std::to_string(statement));
        }

        _pkbSetter->addFollowsT(stringSetOfFollowsStatements);
    }

}
