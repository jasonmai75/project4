# CBusSystemIndexer
- Indexing a wrapper around CBusSYstem that allows sorted and name-based lookups of stops and routes
- Takes existing CBusSystem and creates internal indexes that stops can be retrived in sorted ID order, routes in sorted name order, and lookups by name or stopID are fast.
- Provides the SRouteIndexer which extends SRoute with additional path finding functionality

**using TStopID = CBusSystem::TStopID**
- Brings CBusSystem's stop ID type itno scope for use for this class

**using SStop = CBusSystem::SStop**
- Brings SStop struct into scope so callers dont need to prefix with CBusSystem

**using SRoute = CBusSystem::SRoute**
- Brings SRoute struct into scope for use in this class and SRouteIndexer

**struct SRouteIndexer: public SRoute**
- Creates SRoute with two additional functions for searching inside a route's stop list
- Pure virtual

**virtual ~SRouteIndexer()**
- Virtual destructor, deleting SRouteIndexer objects

**virtual size_t FindStopIndex(TStopID stopid, size_t start = 0) const = 0**
- Searches route's stop list for given stopID beginning at position start
- Parameters:
    - stopid: The TStopID to search for inside the route
    - start: The index to begin searching from(defaults to 0 for a full search)
- Returns index of the first matching stop at or after start or if std::numeric_limits<size_t>::max() is not found

**virtual std::vector<TStopID> StopIDsSourceDestination(TStopID src, TStopID dest) const = 0**
- Returns ordered list of stopIDs between src and dest
- Parameters:
    - src: TStopID of the starting stop
    - dest: TStopID of the destination stop
- Returns vector of TStopIDs from src to dest in route order, or an empty vector if neither stop is on the route

**CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem)**
- Constructs indexer by building sorted indexes over the provided bus system
- Parameters:
    - bussytem: shared pointer to the CBusSystem to index. Stops will be indexed by ID, routes by name

**~CBusSystemIndexer()**
- Destructor

**std::size_t StopCount() const noexcept**
- Returns total number of fstops in indexed bus system
- Like calling StopCount() on original CBusSystem

**std::size_t RouteCount() const noexcept**
- Returns number of routes in the indexed bus system
- like calling RouteCount() in original CBusSystem

**std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept**
- Returns stop at the given index when all stops are seorted in ascending order by their TStopID
- Parameters:
    - index: zero-based position in the sorted stop list
- Returns shared pointer to the SStop, or nullptr if index is >= StopCount()

**std::shared_ptr<SRouteIndexer> SortedRouteByIndex(std::size_t index) const noexcept**
- Returns the route at given index when all routes are sorted in ascending alphabetical order by name
- Parameters:
    - index: zero based position in sorted stop list
- Returns a shared ptr to the SRouteIndexer, or nullptr if index is >= RouteCount()

**std::shared_ptr<SRouteIndexer> RouteByName(const std::string &name) const noexcept**
- Looks up routes directly through its name string
- Parameters:
    - name: the exact route name to search for
- Returns a shared ptr to the matching SRouteIndexer, or nullptr if no route with that name exists

**bool RoutesByStopID(TStopID stopid, std::unordered_setstd::string &routes) const noexcept**
- Finds routes that include the given stop ID anywhere in their stop list
- Parameters:
    - stopid: TStopID of the stop to search for
    - routes: Output set that will be populated with names of all routes serving that stop
- Returns true if at least one route is found, false if no routes are found

**bool RoutesByStopIDs(TStopID src, TStopID dest, std::unordered_setstd::string &routes) const noexcept**
- Finds all routes containing both src and dest in their stop list
- Parameters:
    - src: TStopID of the starting stop
    - dest: TStopID of the destination stop
    - routes: Output set that will be populated with names of routes connecting src to dest in order
- Returns true if at least one valid route was found, false otherwise

**bool StopIDsByRoutes(const std::string &route1, const std::string &route2, std::unordered_set<TStopID> &stops) const noexcept**
- Finds stops that appear in both the named routes
- Parameters:
    - route1: name of first route
    - route 2: name of second route
    - stops: Output set that will be populated with TStopIDs shared by both routes
- Returns true if at least one similar stop is found, false if the routes don't shared any stops or neither route exists

**struct SImplementation**
- Implementation struct holding sorted stop list, sorted route list, and lookup maps

**std::unique_ptr<SImplementation> DImplementation**
- Pointer to internal implementation