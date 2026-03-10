#include "StreetMapIndexer.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct CStreetMapIndexer::SImplementation {
    // Data Storage

    // All nodes from street map sorted by ID (smallest to largest)
    std::vector<std::shared_ptr<CStreetMap::SNode>> DSortedNodes;
    // All ways from street map sorted by ID (smallest to largest)
    std::vector<std::shared_ptr<CStreetMap::SWay>> DSortedWays;
    // Map node ID to all the ways that can contain that node
    std::unordered_map<CStreetMap::TNodeID, std::unordered_set<std::shared_ptr<CStreetMap::SWay>>> DWaysByNodeID;
    // Map node ID to the actual node object (used in WaysInRange)
    std::unordered_map<CStreetMap::TNodeID, std::shared_ptr<CStreetMap::SNode>> DNodesByID;

    // Build sorted indexes in here
    SImplementation(std::shared_ptr<CStreetMap> streetmap) {
         // Collect all nodes by index
         for(std::size_t i = 0; i < streetmap->NodeCount(); i++) {
            // Get node from position i
            auto node = streetmap->NodeByIndex(i);

            // Add it to sorted list
            DSortedNodes.push_back(node);

            // Also store it in our ID->node map for fast lookup later
            DNodesByID[node->ID()] = node;
         }
    }

    // Sort nodes by ID so we can return them in a consistent order (lambda can compare two nodes and smaller ID comes first)
    std::sort(DSortedNodes.begin(), DSortedNodes.end(),
    [](const std::shared_ptr<CStreetMap::SNode> &a, const std::shared_ptr<CStreetMap::SNode> &b) {
        return a->ID() < b->ID();
    });




    
};

CStreetMapIndexer::CStreetMapIndexer(std::shared_ptr<CStreetMap> streetmap) {
    DImplementation = std::make_unique<SImplementation>(streetmap);
}

CStreetMapIndexer::~CStreetMapIndexer() {

}

std::size_t CStreetMapIndexer::NodeCount() const noexcept {
    return 0; // DO STUFF
}

std::size_t CStreetMapIndexer::WayCount() const noexcept {
    return 0; // DO STUFF
}

std::shared_ptr<CStreetMap::SNode> CStreetMapIndexer::SortedNodeByIndex(std::size_t index) const noexcept {
    return nullptr; // DO STUFF
}

std::shared_ptr<CStreetMap::SWay> CStreetMapIndexer::SortedWayByIndex(std::size_t index) const noexcept {
    return nullptr; // DO STUFF
}

std::unordered_set<std::shared_ptr<CStreetMap::SWay>> CStreetMapIndexer::WaysInRange(const CStreetMap::SLocation &lowerleft, const CStreetMap::SLocation &upperright) const noexcept {
    return {}; // DO STUFF
}

std::unordered_set<std::shared_ptr<CStreetMap::SWay>> CStreetMapIndexer::WaysByNodeID(CStreetMap::TNodeID node) const noexcept {
    return {}; // DO STUFF
}







