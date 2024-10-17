#include "SP/DesignExtractor/ParentsExtractor.h"

ParentsExtractor::ParentsExtractor() = default;

ParentsExtractor::ParentsExtractor(std::shared_ptr<PKBSetter> pkbSetter) {
    // Reference to PKB
    _pkbSetter = pkbSetter;
}

void ParentsExtractor::extract(std::shared_ptr<ParentsExtractable> parentsExtractable) {

    std::vector< std::pair<int, std::vector<int> > > parentsRelationships = parentsExtractable->getParentsRelationships();

    for (std::pair<int, std::vector<int> > parentsRelationship: parentsRelationships) {
        int parent = parentsRelationship.first;
        std::vector<int> children = parentsRelationship.second;

        // Convert to strings
        std::string parentLineNoString = std::to_string(parent);
        std::vector<std::string> childrenLineNosStrings;
        for (int child: children) {
            childrenLineNosStrings.push_back(std::to_string(child));
        }

        _pkbSetter->addParent(parentLineNoString, childrenLineNosStrings);
    }

}
