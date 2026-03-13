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

    // Sort nodes by ID so we can return them in a consistent order (lambda can compare two nodes and smaller ID comes first)
    std::sort(DSortedNodes.begin(), DSortedNodes.end(),
    [](const std::shared_ptr<CStreetMap::SNode> &a, const std::shared_ptr<CStreetMap::SNode> &b) {
        return a->ID() < b->ID();
    });

    // Collecting all ways, loop through every way (road/paths) in street map by index
    for(std::size_t i = 0; i < streetmap->WayCount(); i++) {
        // Get the way at position i
        auto way = streetmap->WayByIndex(i);

        // Add to sorted list
        DSortedWays.push_back(way);

        // Build the node -> ways lookup map (differentiating and recognizing nodes that use this way so we can answer questions like "which ways pass through node X?")
        for(std::size_t j = 0; j < way->NodeCount(); j++) {
            // Get ID of j-th node in this way
            CStreetMap::TNodeID nodeID = way->GetNodeID(j);

            // Add this way to the set of ways that include his node.
            DWaysByNodeID[nodeID].insert(way);
        }
    }

    // Sort ways by their ID so we can return them in a consistent order
    std::sort(DSortedWays.begin(), DSortedWays.end(),
    [](const std::shared_ptr<CStreetMap::SWay> &a, const std::shared_ptr<CStreetMap::SWay> &b) {
        return a->ID() < b->ID();
    });
}

    // How many nodes are stored
    std::size_t NodeCount() const noexcept {
         return DSortedNodes.size();
    }

    // How many ways are stored
    std::size_t WayCount() const noexcept {
         return DSortedWays.size();
    }

    // Returns the node at a given position (in sorted-by-ID list)
    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept {
        // Return nullptr if out of range
        if(index >= DSortedNodes.size()) {
               return nullptr;
    }

         return DSortedNodes[index];
}

     // Returns the way at a given position (in sorted-by-ID list)
    std::shared_ptr<CStreetMap::SWay> SortedWayByIndex(std::size_t index) const noexcept {
            // Return nullptr if out of range
        if(index >= DSortedWays.size()) {
            return nullptr;
    }

        return DSortedWays[index];
}

    // Returns all ways that contain a specific node
    std::unordered_set<std::shared_ptr<CStreetMap::SWay>> WaysByNodeID(CStreetMap::TNodeID nodeID) const noexcept {
        // Look up the node ID in our map
         auto it = DWaysByNodeID.find(nodeID);

        // If found, return set of ways that use this node
         if(it != DWaysByNodeID.end()) {
              return it->second;
        }

        // If node wasn't part of any way, return an empty set
        return {};
    }

    // Return all ways that have AT LEAST ONE node
    std::unordered_set<std::shared_ptr<CStreetMap::SWay>> WaysInRange(const CStreetMap::SLocation &lowerleft, const CStreetMap::SLocation &upperright) const noexcept {
        // This set collects all qualifying ways (no dupes)
        std::unordered_set<std::shared_ptr<CStreetMap::SWay>> result;

    for(const auto &way : DSortedWays) {
        // Loop through all ways stored
        for(std::size_t i = 0; i < way->NodeCount(); i++) {
            // Get the node ID at position i in this way
            CStreetMap::TNodeID nodeID = way->GetNodeID(i);

            // Look up actual node object so we can get lattitude and longitude
            auto nodeIt = DNodesByID.find(nodeID);

            // Skip node if somehow not found (shouldn't happen but just in case)
            if(nodeIt == DNodesByID.end()) {
                continue;
            }

            // Get geographic location (lat + long) of node
            CStreetMap::SLocation loc = nodeIt->second->Location();

            // Check if node's location falls inside the bounding box
            
            // Latitude must be between lowerleft and upperright
            bool latInRange = false;
            if(loc.DLatitude >= lowerleft.DLatitude && loc.DLatitude <= upperright.DLatitude) {
                latInRange = true;
            }

            // Longitude must be between lowerleft and upperright
            bool longInRange = false;
            if(loc.DLongitude >= lowerleft.DLongitude && loc.DLongitude <= upperright.DLongitude) {
                longInRange = true;
            }

            // If both are in range
            if(latInRange && longInRange) {
                // This way has at least one node in the box, so add it
                result.insert(way);

                // Don't need to check the rest of the way's node, since it already qualifies
                break;
            }
        }
    }
    return result;
}


};

CStreetMapIndexer::CStreetMapIndexer(std::shared_ptr<CStreetMap> streetmap) {
    DImplementation = std::make_unique<SImplementation>(streetmap);
}

CStreetMapIndexer::~CStreetMapIndexer() {

}

// 
std::size_t CStreetMapIndexer::NodeCount() const noexcept {
    return DImplementation->NodeCount();
}

std::size_t CStreetMapIndexer::WayCount() const noexcept {
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> CStreetMapIndexer::SortedNodeByIndex(std::size_t index) const noexcept {
    return DImplementation->SortedNodeByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> CStreetMapIndexer::SortedWayByIndex(std::size_t index) const noexcept {
    return DImplementation->SortedWayByIndex(index);
}

std::unordered_set<std::shared_ptr<CStreetMap::SWay>> CStreetMapIndexer::WaysByNodeID(CStreetMap::TNodeID node) const noexcept {
    return DImplementation->WaysByNodeID(node);
}

std::unordered_set<std::shared_ptr<CStreetMap::SWay>> CStreetMapIndexer::WaysInRange(const CStreetMap::SLocation &lowerleft, const CStreetMap::SLocation &upperright) const noexcept {
    return DImplementation->WaysInRange(lowerleft, upperright);
}






