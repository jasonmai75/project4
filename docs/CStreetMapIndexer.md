# CStreetMapIndexer
- Index wrapper around CStreetMap allowing sorted and spacial lookups of nodes and ways
- Takes CStreetMap and creates internal indexes so nodes can be retrieved in sorted ID order, ways in sorted ID order, and spatial or node-based lookups are fast.

**CStreetMapIndexer(std::shared_ptr<CStreetMap> streetmap)**
- Constructor
- Parameter:
    - streetmap: shared ptr to CStreetMap to index, nodes and ways are sorted by IDs and indexed for spatial lookup

**~CStreetMapIndexer()**
- Destructor

**std::size_t NodeCount() const noexcept**
- Returns number of nodes in indexed street map
- Equivalent of calling NodeCount() in CStreetMap

**std::size_t WayCount() const noexcept**
- Returns number of ways in indexed street map
- Equivalent to calling WayCount() on original CStreetMap

**std::shared_ptrCStreetMap::SNode SortedNodeByIndex(std::size_t index) const noexcept**
- Returns the node at given index when all nodes are sorted by their TNodeID (ascending order)
- Parameters
    - index: Position in the sorted node list
- Returns a shared ptr to the SNode, or nullptr if index is >= NodeCount()

**std::shared_ptrCStreetMap::SWay SortedWayByIndex(std::size_t index) const noexcept**
- Returns the way at the given index when all ways are sorted in ascending order by TWayID
- Parameters:
    - index: Position in sorted way list
- Returns shared ptr to the SWay, or nullptr if index is >= WayCount()

**std::unordered_set<std::shared_ptrCStreetMap::SWay> WaysInRange(const CStreetMap::SLocation &lowerleft, const CStreetMap::SLocation &upperright) const noexcept**
- Returns all ways that have at least one node within the boudning box defined by lowerleft and upperright
- Parameters:
    - lowerleft: SLocation representing the lower left corner of the counding box (min latitude and longitude)
    - upperright: SLocation representing upper right corner of the bounding box (max latitude and longitude)

- Returns unorderd set of shared ptrs to all SWay objects that are within the range. Returns empty set if no ways are found

**std::unordered_set<std::shared_ptrCStreetMap::SWay> WaysByNodeID(CStreetMap::TNodeID node) const noexcept**
- Returns all the ways containing the given node ID in their node list
- Parameters:
    - node: TNodeID of the node to search for
- Returns an unordered set of shared ptrs to all SWay objects that reference the node. Returns empty set if the node is not part of any way

**struct SImplementation**
- Internal Implementation struct

**std::unique_ptr<SImplementation> DImplementation**
- Pointer to internal implementation