#include "TripPlanner.h"
#include "BusSystemIndexer.h"
#include <limits>
#include <chrono>

static long long ToSeconds(CBusSystem::TStopTime t) {
    return t.hours().count() * 3600LL + t.minutes().count() * 60LL + t.seconds().count();

}

static CBusSystem::TStopTime FromSeconds(long long sec) {
    return CBusSystem::TStopTime(std::chrono::seconds(sec));
}

struct CTripPlanner::SImplementation{
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<CBusSystemIndexer> DIndexer;
    SImplementation(std::shared_ptr<CBusSystem> bussystem) : DBusSystem(bussystem){
        
        if(bussystem) {
            DIndexer = std::make_shared<CBusSystemIndexer>(bussystem);
        }
        
    }

    ~SImplementation() = default;

    std::shared_ptr<CBusSystemIndexer> BusSystemIndexer() const{
        return DIndexer;
    }
    
    std::shared_ptr< SRoute > FindDirectRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat) const{
        // Check if indexer works
        if(!DIndexer) {
            return nullptr;
        }

        // What route contains both the src and dest
        std::unordered_set<std::string> routeNames;

        // Check if routes connect src and dest
        if(!DIndexer->RoutesByStopIDs(src, dest, routeNames)) {
            return nullptr;
        }
        
        // Convert leave time to seconds for easy comparison
        long long leaveAtSec = ToSeconds(leaveat);

        // Start with worst possible arrival time
        long long bestArrival = std::numeric_limits<long long>::max();

        std::shared_ptr<SRoute> bestRoute;

        for(const auto &name : routeNames) {
            auto ri = DIndexer->RouteByName(name);
            if(!ri) {
                continue;
            }

            // Where is src in route
            std::size_t srcIdx = ri->FindStopIndex(src);
                if(srcIdx == std::numeric_limits<std::size_t>::max()) {
                continue;
            }
            
            // Where is dest AFTER src in route
            std::size_t destIdx = ri->FindStopIndex(dest, srcIdx + 1);
            if(destIdx == std::numeric_limits<std::size_t>::max()) {
                continue;
            }

            // Check all trips
            for(std::size_t trip = 0; trip < ri->TripCount(); ++trip) {
                long long depSec = ToSeconds(ri->GetStopTime(srcIdx, trip));

                // skip trips that already left
                if(depSec < leaveAtSec) {
                    continue;
                }
                
                long long arrSec = ToSeconds(ri->GetStopTime(destIdx, trip));
                
                // Is this the earliest arrival?
                if (arrSec < bestArrival) {
                    bestArrival = arrSec;
                    bestRoute = DBusSystem->RouteByName(name);
                }
            }
        }

        // Return the best route
        return bestRoute;
    }
    
    std::shared_ptr< SRoute > FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const{
        // Check if Indexer is good
        if(!DIndexer) {
            return nullptr;
        }

        std::unordered_set<std::string> routeNames;

        // Check if routes connect src and dest
        if(!DIndexer->RoutesByStopIDs(src, dest, routeNames)) {
            return nullptr;
        }

        long long arriveBySec = ToSeconds(arriveby);

        // start at -1 so any real departure can replace it
        long long bestDepart = -1LL;
        std::shared_ptr<SRoute> bestRoute;

        for(const auto &name : routeNames) {
            auto ri = DIndexer->RouteByName(name);
            if(!ri) {
                continue;
            }

            // Where is src in route
            std::size_t srcIdx = ri->FindStopIndex(src);
                if(srcIdx == std::numeric_limits<std::size_t>::max()) {
                continue;
            }
            
            // Where is dest AFTER src in route
            std::size_t destIdx = ri->FindStopIndex(dest, srcIdx + 1);
            if(destIdx == std::numeric_limits<std::size_t>::max()) {
                continue;
            }

            for(std::size_t trip = 0; trip < ri->TripCount(); ++trip) {
                // Check arrival first 
                long long arrSec = ToSeconds(ri->GetStopTime(destIdx,trip));

                // Skip if arrival is too late
                if (arrSec > arriveBySec) {
                    continue;
                }

                long long depSec = ToSeconds(ri->GetStopTime(srcIdx, trip));

                // Get LATEST departure (opposite of LeaveTime)
                if(depSec > bestDepart) {
                    bestDepart = depSec;
                    bestRoute = DBusSystem->RouteByName(name);
                }
            }
            
        }
        // Return Best Route
        return bestRoute;
    }
    
    bool FindRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat, TTravelPlan &plan) const{
        if(!DBusSystem){
            return false;
        }
        long long leaveAtSec = ToSeconds(leaveat);
        long long bestArrival = std::numeric_limits<long long>::max();
        TTravelPlan bestPlan;
        //Lambda helper to easily find stop indices without relying purely on Indexer mappings
        auto findStopIdx = [](std::shared_ptr<CBusSystem::SRoute> r, TStopID stop, size_t start = 0) -> size_t {
            for(size_t i = start; i < r->StopCount(); ++i) {
                if(r->GetStopID(i) == stop) return i;
            }
            return std::string::npos;
        };
        for (size_t r1 = 0; r1 < DBusSystem->RouteCount(); ++r1) {
            auto routeA = DBusSystem->RouteByIndex(r1);
            size_t srcIdxA = findStopIdx(routeA, src);
            if (srcIdxA == std::string::npos){
                continue;
            }
            //Check 0-Transfer (Direct) Route first
            size_t destIdxA = findStopIdx(routeA, dest, srcIdxA + 1);
            if (destIdxA != std::string::npos) {
                //Check all trips on this direct route
                for (size_t tripA = 0; tripA < routeA->TripCount(); ++tripA) {
                    long long depA = ToSeconds(routeA->GetStopTime(srcIdxA, tripA));
                    if (depA < leaveAtSec){
                        continue;
                    }
                    long long arrA = ToSeconds(routeA->GetStopTime(destIdxA, tripA));  
                    if (arrA < bestArrival) {
                        bestArrival = arrA;
                        bestPlan.clear();
                        bestPlan.push_back({FromSeconds(depA), src, routeA->Name()});
                        bestPlan.push_back({FromSeconds(arrA), dest, ""});
                    }
                }
            }
            //Check 1-Transfer Route (Cross-reference with all other routes)
            for (size_t r2 = 0; r2 < DBusSystem->RouteCount(); ++r2) {
                if (r1 == r2){
                    continue; 
                } 
                auto routeB = DBusSystem->RouteByIndex(r2);
                size_t destIdxB = findStopIdx(routeB, dest);
                if (destIdxB == std::string::npos) continue;

                //Look for an intersection point (Transfer Stop) after src in Route A
                for (size_t iA = srcIdxA + 1; iA < routeA->StopCount(); ++iA) {
                    TStopID transferStop = routeA->GetStopID(iA);
                    
                    //Check if Route B also has this stop
                    size_t transIdxB = findStopIdx(routeB, transferStop);
                    
                    //Route B must pass through the transfer stop before reaching the destination
                    if (transIdxB != std::string::npos && transIdxB < destIdxB) {
                        
                        for (size_t tripA = 0; tripA < routeA->TripCount(); ++tripA) {
                            long long depA = ToSeconds(routeA->GetStopTime(srcIdxA, tripA));    
                            if (depA < leaveAtSec){
                                continue;
                            }
                            
                            long long arrA = ToSeconds(routeA->GetStopTime(iA, tripA));
                            for (size_t tripB = 0; tripB < routeB->TripCount(); ++tripB) {
                                long long depB = ToSeconds(routeB->GetStopTime(transIdxB, tripB));
                                if (depB < arrA){
                                    continue;
                                }                                
                                long long arrB = ToSeconds(routeB->GetStopTime(destIdxB, tripB));
                                if (arrB < bestArrival) {
                                    bestArrival = arrB;
                                    bestPlan.clear();
                                    
                                    //Board Route A
                                    bestPlan.push_back({FromSeconds(depA), src, routeA->Name()});
                                    //Transfer to Route B
                                    bestPlan.push_back({FromSeconds(depB), transferStop, routeB->Name()});
                                    //Arrive at Dest
                                    bestPlan.push_back({FromSeconds(arrB), dest, ""});                    
                                }
                            }
                        }
                    }
                }
            }
        }
        if (bestPlan.empty()){
            return false;
        }
        
        plan = std::move(bestPlan);
        return true;
    }
        
    bool FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TTravelPlan &plan) const{
        if(!DBusSystem){
            return false;
        }
        long long arriveBySec = ToSeconds(arriveby);
        long long bestDepart = -1LL;
        TTravelPlan bestPlan;

        //Lambda helper to easily find stop indices without relying purely on Indexer mappings
        auto findStopIdx = [](std::shared_ptr<CBusSystem::SRoute> r, TStopID stop, size_t start = 0) -> size_t {
            for(size_t i = start; i < r->StopCount(); ++i) {
                if(r->GetStopID(i) == stop) return i;
            }
            return std::string::npos;
        };

        //Iterate through every route to find the starting point
        for (size_t r1 = 0; r1 < DBusSystem->RouteCount(); ++r1) {
            auto routeA = DBusSystem->RouteByIndex(r1);
            
            //Where is src in Route A
            size_t srcIdxA = findStopIdx(routeA, src);
            if (srcIdxA == std::string::npos){
                continue;
            }

            //Check 0-Transfer (Direct) Route
            size_t destIdxA = findStopIdx(routeA, dest, srcIdxA + 1);
            if (destIdxA != std::string::npos) {
                for (size_t tripA = 0; tripA < routeA->TripCount(); ++tripA) {
                    long long arrA = ToSeconds(routeA->GetStopTime(destIdxA, tripA));
                    if (arrA > arriveBySec){
                        continue;
                    }
                    
                    long long depA = ToSeconds(routeA->GetStopTime(srcIdxA, tripA));
                    
                    if (depA > bestDepart) {
                        bestDepart = depA;
                        bestPlan.clear();
                        bestPlan.push_back({FromSeconds(depA), src, routeA->Name()});
                        bestPlan.push_back({FromSeconds(arrA), dest, ""});
                    }
                }
            }

            //Check 1-Transfer Route
            for (size_t r2 = 0; r2 < DBusSystem->RouteCount(); ++r2) {
                //Skip same route (already handled by direct check)
                if (r1 == r2) continue; 
                
                auto routeB = DBusSystem->RouteByIndex(r2);
                
                //Where is dest in Route B
                size_t destIdxB = findStopIdx(routeB, dest);
                if (destIdxB == std::string::npos) continue;

                //Look for an intersection point (Transfer Stop) AFTER src in Route A
                for (size_t iA = srcIdxA + 1; iA < routeA->StopCount(); ++iA) {
                    TStopID transferStop = routeA->GetStopID(iA);
                    
                    //Check if Route B also has this transfer stop
                    size_t transIdxB = findStopIdx(routeB, transferStop);
                    
                    //Route B must pass through the transfer stop BEFORE reaching the destination
                    if (transIdxB != std::string::npos && transIdxB < destIdxB) {
                        for (size_t tripB = 0; tripB < routeB->TripCount(); ++tripB) {
                            long long arrB = ToSeconds(routeB->GetStopTime(destIdxB, tripB));
                            if (arrB > arriveBySec){
                                continue;
                            } 
                            long long depB = ToSeconds(routeB->GetStopTime(transIdxB, tripB));
                            for (size_t tripA = 0; tripA < routeA->TripCount(); ++tripA) {
                                long long arrA = ToSeconds(routeA->GetStopTime(iA, tripA));
                                
                                if (arrA > depB){
                                    continue;
                                }
                                long long depA = ToSeconds(routeA->GetStopTime(srcIdxA, tripA));
                                //Update best plan if it leaves later
                                if (depA > bestDepart) {
                                    bestDepart = depA;
                                    bestPlan.clear();
                                    
                                    //Board Route A
                                    bestPlan.push_back({FromSeconds(depA), src, routeA->Name()});
                                    //Transfer to Route B
                                    bestPlan.push_back({FromSeconds(depB), transferStop, routeB->Name()});
                                    //Arrive at Dest
                                    bestPlan.push_back({FromSeconds(arrB), dest, ""});
                                }
                            }
                        }
                    }
                }
            }
        }
        if (bestPlan.empty()){
            return false;
        }
        plan = std::move(bestPlan);
        return true;
    }
       
};


CTripPlanner::CTripPlanner(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CTripPlanner::~CTripPlanner() = default;

std::shared_ptr<CBusSystemIndexer> CTripPlanner::BusSystemIndexer() const{
    return DImplementation->BusSystemIndexer();
}

std::shared_ptr< CTripPlanner::SRoute > CTripPlanner::FindDirectRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat) const{
    return DImplementation->FindDirectRouteLeaveTime(src,dest,leaveat);
}

std::shared_ptr< CTripPlanner::SRoute > CTripPlanner::FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const{
    return DImplementation->FindDirectRouteArrivalTime(src,dest,arriveby);
}

bool CTripPlanner::FindRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat, TTravelPlan &plan) const{
    return DImplementation->FindRouteLeaveTime(src,dest,leaveat,plan);
}

bool CTripPlanner::FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TTravelPlan &plan) const{
    return DImplementation->FindRouteArrivalTime(src,dest,arriveby,plan);
}

