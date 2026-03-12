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

            // Not both src and dest, then return nullptr
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
        // Should be almost identitcal to leave time?? 
        std::shared_ptr< SRoute > FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const{
            // Check if Indexer is good
            if(!DIndexer) {
                return nullptr;
            }

            std::unordered_set<std::string> routeNames;

            // Check existence of src and dest
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
            return false;
        }
        
        bool FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TTravelPlan &plan) const{
            return false;
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

