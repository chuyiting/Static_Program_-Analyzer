#include "utils/CycleDetector/CycleDetector.h"


CycleDetector::CycleDetector(int size) {
    numVertices = size;
}


void CycleDetector::addEdge(std::string from, std::string to) {

    // Stores nodes as integers
    if (stringToIntMap.find(from) == stringToIntMap.end()) {
        stringToIntMap[from] = stringToIntMap.size();
    }
    
    if (stringToIntMap.find(to) == stringToIntMap.end()) {
        stringToIntMap[to] = stringToIntMap.size();
    }

    // Track for adjacency list
    int fromInt = stringToIntMap[from];
    int toInt = stringToIntMap[to];
    if (adjacencyList.find(fromInt) == adjacencyList.end()) {
        adjacencyList[fromInt] = std::vector<int>();
    }
    adjacencyList[fromInt].push_back(toInt);

}

bool CycleDetector::findCycle() {
    for (auto it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
        int node = it->first;
        
        // if node not in discovered and node not in finished
        if ((discovered.find(node) == discovered.end()) && (finished.find(node) == finished.end())) {
            visit(node);
        }
    }

    return hasCycle;
}

void CycleDetector::visit(int node) {

    // we have discovered this node
    discovered.insert(node);

    // iterate over neighbours
    for (int neighbour : adjacencyList[node]) {

        // if we have discovered this node before
        if (discovered.find(neighbour) != discovered.end()) {

            // there is a back edge from node to neighbour
            hasCycle = true;
            break;
        } 
        
        // if node is not in finished, recurse
        if (finished.find(neighbour) == finished.end()) {
            visit(neighbour);
        }
    }

    // remove from discovered, for other nodes to use
    discovered.erase(node);

    // we have finished looking at this node
    finished.insert(node);
}
