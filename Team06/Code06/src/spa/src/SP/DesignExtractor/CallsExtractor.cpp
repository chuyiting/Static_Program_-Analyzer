#include "SP/DesignExtractor/CallsExtractor.h"


CallsExtractor::CallsExtractor() = default;

CallsExtractor::CallsExtractor(std::shared_ptr<PKBSetter> pkbSetter) {
    // Reference to PKB
    _pkbSetter = pkbSetter;
}

void CallsExtractor::extract(std::shared_ptr<CallsExtractable> callsExtractable) {

    std::string callerName = callsExtractable->getCallerName();
    std::vector< std::string > callsRelationships = callsExtractable->getProceduresCalled();
    
    for (std::string procedureCalled : callsRelationships) {
        _pkbSetter->addCallRelation(callerName, procedureCalled);
    }

}
