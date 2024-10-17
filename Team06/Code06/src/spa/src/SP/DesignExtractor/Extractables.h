#pragma once

#include <string>
#include <unordered_set>

// Indicator to check if the ModifiesExtractable / UsesExtractable is a procedure or a statement.
enum ExtractableType {
    PROCEDURE,
    STATEMENT
};


// Abstract class that can be operated on by UsesExtractor constants.
class UsesExtractable {

public:

    virtual std::unordered_set<std::string> getConstantsUsed() = 0;
    virtual std::unordered_set<std::string> getVariablesUsed() = 0;
    virtual ExtractableType getExtractableType() = 0;

};



// Abstract class that modifies constants.
class ModifiesExtractable {

public:

    virtual std::unordered_set<std::string> getVariablesModified() = 0;
    virtual ExtractableType getExtractableType() = 0;

};

// Abstract class that has statement lists with follows relationships.
class FollowsExtractable {

public:

    // A set of vectors
    // Each vector is a vector of integers, in ascending order
    // E.g. {1, 2, 3, 4, 7} -> would imply that 1 < 2 < 3 < 4 < 7 in terms of Follows
    virtual std::vector<std::vector<int>> getFollowsStatements() = 0;

};

// Abstract class that has nesting with parent relationships.
class ParentsExtractable {

public:

    virtual std::vector< std::pair<int, std::vector<int> > > getParentsRelationships() = 0;

};

// Abstract class that has procedure calls.
class CallsExtractable {

public:

    virtual std::string getCallerName() = 0;
    virtual std::vector< std::string > getProceduresCalled() = 0;
    
};

// Abstract class that has statement lists with next relationships.
class NextExtractable {
public:

    virtual std::unordered_map<int, std::unordered_set<int>> getEdges() = 0;

};