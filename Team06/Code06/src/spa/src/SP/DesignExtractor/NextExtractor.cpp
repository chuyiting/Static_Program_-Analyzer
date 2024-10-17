#include "SP/DesignExtractor/NextExtractor.h"


NextExtractor::NextExtractor() = default;

NextExtractor::NextExtractor(std::shared_ptr<PKBSetter> pkbSetter) {
    // Reference to PKB
    _pkbSetter = pkbSetter;
}

void NextExtractor::extract(std::shared_ptr<NextExtractable> nextExtractable) {

    std::unordered_map<int, std::unordered_set<int>> edges = nextExtractable->getEdges();

    for (auto const& p : edges) {
        int parent = p.first;
        for (int child : p.second) {
            _pkbSetter->addNextRelation(std::to_string(parent), std::to_string(child));
        }
    }
}
