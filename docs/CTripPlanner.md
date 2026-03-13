# TripPlanner 
- Plans bus trips between two stops given either a departure time or a desired arrival time
- Wraps a CBusSystem and gives route-finding functions for trips
- Given the src and dest stop ID, it can find the best route to either leave as late possible (arrive by) or arriving as early as possible (leave at)
- Results are returned as either a single SRoute for direct trips or as a full TTravelPlan

**using TStopID = CBusSystem::TStopID**
- Brings CBusSystem's stop ID type into scope for use

**using TStopTime = CBusSystem::TStopTime**
- Brings CBusSystem's stop time type into scope
- Represents scheduled time at a stop

**using SRoute = CBusSystem::SRoute**
- Brings the SRoute struct into scope

**struct STravelStep**
- Represents a single step in a travel plan
- One stop on one route at a specific time

**TStopTime DTime**
- Scheduled time at this stop for this step

**TStopID DStopID**
- StopID where this step occurs at

**std::string DRouteName**
- Nmae of bus route being taken at this step
- Empty string means a transfer or waiting step

**using TTravelPlan = std::vector<STravelStep>**
- Ordered sequence of STravelStep objects showing the complete trip from src to dest
- Every entry is one stop along the journey in chronological order

**CTripPlanner(std::shared_ptr<CBusSystem> bussystem)**
- Constructor
- Parameters:
    - busssytem: shared ptr to CBusSystem containing the stops, routes, and schedules to plan trips with

**~CTripPlanner()**
- Destructor

**std::shared_ptr<CBusSystemIndexer> BusSystemIndexer() const**
- Returns the CBusSystemIndexer

**std::shared_ptr<SRoute> FindDirectRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat) const**
- Finds the single direct bus route to take from src to dest if leaving at or after leaveat
- Picks routes that result in earliest possible arrival at dest
- Parameters:
    - src: TStopID of starting stop
    - dest: TStopID of destination stop
    - leaveat: Earliest time the user is willing to depart from src

- Returns shared ptr to the best SRoute or nullptr if no routes connect src to dest at or after leave at

**std::shared_ptr<SRoute> FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const**
- Finds single direct bus route to take fro msrc to dest to arrive by arriveby
- Selects the route that allows latest possible departure from src while still arriving on time
- Parameters:
    - src: TStopID of the starting stop
    - dest: TStopID of the destination stop
    - arriveby: Latest acceptable arrival time at dest
- Returns a shared ptr to the best SRoute or nullptr if no direct route can get from src to dest by arriveby

**bool FindRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat, TTravelPlan &plan) const**
- Finds completed travel plan from src to dest leaving at or after leaveat, trying to get the earliest possible arrival
- Parameters:
    - src: TStopID of the starting stop
    - dest: TStopID of destination stop
    - leaveat: Earliest time the user can depart from src
    - plan: TTravelPlan that will be populated with ordered steps of the trip if one is found
- Returns true and fills plan if a valid trip is found, false if no trip exists

**bool FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TTravelPlan &plan) const**
- Finds complete travel plan from src to dest arrivign by arriveby, trying to get the latest possible departure
- Parameters:
    - src: TStopID of starting stop
    - dest: TStopID of the destination stop
    - arriveby: Latest acceptable arrival time at dest
    - plan: TTravelPlan that will be filled with ordered steps of the trip if one if one is found
- Returns true and fills plan if a valid trip was found, false if no trip exists

**struct SImplementation**
- Internal Implementation

**std::unique_ptr<SImplementation> DImplementation**
- Pointer to internal implementation