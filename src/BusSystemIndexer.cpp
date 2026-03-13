#include "BusSystemIndexer.h"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <limits>
#include <utility>
#include <unordered_set>
#include <iostream>

struct CBusSystemIndexer::SImplementation{
struct SRouteIndexer : public CBusSystemIndexer::SRouteIndexer{
        std::shared_ptr<CBusSystem::SRoute> DRoute;

        SRouteIndexer(std::shared_ptr<CBusSystem::SRoute> route){
            DRoute = route;
        }
        ~SRouteIndexer(){};

        std::string Name() const noexcept override{
            return DRoute->Name();
        }

        std::size_t StopCount() const noexcept override{
            return DRoute->StopCount();
        }
        
        std::size_t TripCount() const noexcept override{
            return DRoute->TripCount();
        }
        
        
        TStopID GetStopID(std::size_t index) const noexcept override{
            return DRoute->GetStopID(index);
        }
        
        CBusSystem::TStopTime GetStopTime(std::size_t stopindex, std::size_t tripindex) const noexcept override{
            return DRoute->GetStopTime(stopindex,tripindex);
        }
        
        // Searches for a stop ID starting from a given index
        size_t FindStopIndex(TStopID stopid, size_t start) const override{
        for(size_t i = start; i < StopCount(); i++) {
            if(GetStopID(i) == stopid) {
                return i;
            }
        }
        return std::numeric_limits<size_t>::max(); // return max when not found
    }

        // Returns all stop IDs between src and dest on this route
        std::vector< TStopID > StopIDsSourceDestination(TStopID src, TStopID dest) const override{
            std::vector<TStopID> result;

            // Figure out where src appears in this route
            size_t startIndex = FindStopIndex(src, 0);
            if(startIndex >= StopCount()) {
                return result; // src not on this route
            }

            // Collect all stops from src up to and including dest
            for(size_t i = startIndex; i < StopCount(); i++) {
                result.push_back(GetStopID(i));
                if(GetStopID(i) == dest) {
                    return result; // found dest
                }
            }
        return {}; // If dest not found after src, return empty 
      }
    };

    std::shared_ptr<CBusSystem> DBusSystem;
    std::vector<std::shared_ptr<CBusSystem::SStop>> DSortedStopsByIndex;
    std::vector<std::shared_ptr<SRouteIndexer>> DSortedRoutesByIndex;
    std::unordered_map<std::string, std::shared_ptr<SRouteIndexer>> DRoutesByName;
    std::unordered_map<TStopID, std::unordered_set<std::string>> DRoutesByStopID;
    struct SStopIDsHash{
        size_t operator()(const std::pair<TStopID,TStopID> &pair) const{
            return pair.first ^ (pair.second<<1);
        }
    };
    std::unordered_map<std::pair<TStopID, TStopID>,std::unordered_set<std::string>, SStopIDsHash> DRoutesByStopIDs;

    SImplementation(std::shared_ptr<CBusSystem> bussystem){
        DBusSystem = bussystem;
        for(size_t Index = 0; Index < bussystem->StopCount(); Index++){
            auto stop = bussystem->StopByIndex(Index);
          
            DSortedStopsByIndex.push_back(stop);
        }
        std::sort(DSortedStopsByIndex.begin(), DSortedStopsByIndex.end(),[](std::shared_ptr<SStop> &l, std::shared_ptr<SStop> &r) -> bool{
            return l->ID() < r->ID();
        });
        for(size_t Index = 0; Index < bussystem->RouteCount(); Index++){
            auto Route = std::make_shared<SRouteIndexer>(bussystem->RouteByIndex(Index));
            DSortedRoutesByIndex.push_back(Route);
            DRoutesByName[Route->Name()] = Route;
            for(size_t StopIndex = 0; StopIndex < Route->StopCount(); StopIndex++){
                auto StopID = Route->GetStopID(StopIndex);
                auto Search = DRoutesByStopID.find(StopID);
                if(Search == DRoutesByStopID.end()){
                    DRoutesByStopID[StopID] = {Route->Name()};
                }
                else{
                    Search->second.insert(Route->Name());
                }
                for(size_t DestIndex = StopIndex+1; DestIndex < Route->StopCount(); DestIndex++){
                    auto DestID = Route->GetStopID(DestIndex);
                    auto Pair = std::make_pair(StopID,DestID);
                    auto PairSearch = DRoutesByStopIDs.find(Pair);
                    if(PairSearch == DRoutesByStopIDs.end()){
                        DRoutesByStopIDs[Pair] = {Route->Name()};
                    }
                    else{
                        PairSearch->second.insert(Route->Name());
                    }
                }
            }
        }
        std::sort(DSortedRoutesByIndex.begin(), DSortedRoutesByIndex.end(),[](std::shared_ptr<SRouteIndexer> &l, std::shared_ptr<SRouteIndexer> &r) -> bool{
            return l->Name() < r->Name();
        });


    }

    ~SImplementation(){

    }


    std::size_t StopCount() const noexcept{
        return DSortedStopsByIndex.size();
    }

    std::size_t RouteCount() const noexcept{
        return DSortedRoutesByIndex.size();
    }

    std::shared_ptr<SStop> SortedStopByIndex(std::size_t index) const noexcept{
        if(index < StopCount()){
            return DSortedStopsByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<SRouteIndexer> SortedRouteByIndex(std::size_t index) const noexcept{
        if(index < RouteCount()){
            return DSortedRoutesByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<SRouteIndexer> RouteByName(const std::string &name) const noexcept{
        auto Search = DRoutesByName.find(name);
        if(Search != DRoutesByName.end()){
            return Search->second;
        }
        return nullptr;
    }

    bool RoutesByStopID(TStopID stopid, std::unordered_set< std::string > &routes) const noexcept{
        auto Search = DRoutesByStopID.find(stopid);
        if(Search != DRoutesByStopID.end()){
            routes = Search->second;
            return true;
        }
        return false;
    }

    bool RoutesByStopIDs(TStopID src, TStopID dest, std::unordered_set< std::string > &routes) const noexcept{
        auto Search = DRoutesByStopIDs.find(std::make_pair(src,dest));
        if(Search != DRoutesByStopIDs.end()){
            routes = Search->second;
            return true;
        }
        return false;
    }

    bool StopIDsByRoutes(const std::string &route1, const std::string &route2, std::unordered_set< TStopID > &stops) const noexcept{
        auto search1 = DRoutesByName.find(route1);
        auto search2 = DRoutesByName.find(route2);

        if(search1 == DRoutesByName.end() || search2 == DRoutesByName.end()) { 
            return false;
        }

        stops.clear();

        for(size_t i = 0; i < search1->second->StopCount(); i++) {
            auto id = search1->second->GetStopID(i);
            for(size_t j = 0; j < search2->second->StopCount(); j++) {
                if(search2->second->GetStopID(j) == id) {
                    stops.insert(id);
                }
            }
        } 

        return !stops.empty();

    }

};


CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CBusSystemIndexer::~CBusSystemIndexer(){

}


std::size_t CBusSystemIndexer::StopCount() const noexcept{
    return DImplementation->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept{
    return DImplementation->SortedStopByIndex(index);
}

std::shared_ptr<CBusSystemIndexer::SRouteIndexer> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept{
    return DImplementation->SortedRouteByIndex(index);
}

std::shared_ptr<CBusSystemIndexer::SRouteIndexer> CBusSystemIndexer::RouteByName(const std::string &name) const noexcept{
    return DImplementation->RouteByName(name);
}

bool CBusSystemIndexer::RoutesByStopID(TStopID stopid, std::unordered_set< std::string > &routes) const noexcept{
    return DImplementation->RoutesByStopID(stopid,routes);
}

bool CBusSystemIndexer::RoutesByStopIDs(TStopID src, TStopID dest, std::unordered_set< std::string > &routes) const noexcept{
    return DImplementation->RoutesByStopIDs(src,dest,routes);
}

bool CBusSystemIndexer::StopIDsByRoutes(const std::string &route1, const std::string &route2, std::unordered_set< TStopID > &stops) const noexcept{
    return DImplementation->StopIDsByRoutes(route1,route2,stops);
}

