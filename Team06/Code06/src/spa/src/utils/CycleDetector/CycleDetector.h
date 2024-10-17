#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Class to detect cycles in a directed graph, given an edgelist.
// This is used to detect cyclic procedure calls in the program.
class CycleDetector {

    private:

        // storing vertices as integers
        std::unordered_map<std::string, int> stringToIntMap;

        // initialisation
        int numVertices = 0;

        // adjacency list
        std::unordered_map<int, std::vector<int>> adjacencyList;
        
        // result
        bool hasCycle = false;

        // helper function
        void visit(int node);
        std::unordered_set<int> discovered;
        std::unordered_set<int> finished;


    public:

        // constructor
        CycleDetector(int size);

        // calls to add edge
        void addEdge(std::string from, std::string to);

        // main running function
        bool findCycle();
};
