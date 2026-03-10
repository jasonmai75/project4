#include "StreetMapIndexer.h"

struct CStreetMapIndexer::SImplementation {
    std::shared_ptr<CStreetMap> DStreetMap;
    
    
    SImplementation(std::shared_ptr<CStreetMap> streetmap) {
        DStreetMap = streetmap;
        // build sorted indexes in here and do stuff
    }
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







