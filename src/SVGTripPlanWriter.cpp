#include "SVGTripPlanWriter.h"
#include <unordered_map>
#include <unordered_set>
#include <set>
#include "GeographicUtils.h"
#include "SVGWriter.h"
#include "StreetMapIndexer.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <queue>
#include <map>
#include <set>

struct CSVGTripPlanWriter::SImplementation{
    struct SConfig : public CTripPlanWriter::SConfig{
        std::unordered_set<std::string> DEnabledFlags;
        std::unordered_map<std::string, std::any> DOptions;
        std::unordered_map<std::string, EOptionType> DOptionTypes;

        std::unordered_set<std::string> DValidFlags;
        std::unordered_set<std::string> DValidOptions;
        
        SConfig(){
            DValidFlags = {
                std::string(CSVGTripPlanWriter::MotorwayEnabled),
                std::string(CSVGTripPlanWriter::PrimaryEnabled),
                std::string(CSVGTripPlanWriter::SecondaryEnabled),
                std::string(CSVGTripPlanWriter::TertiaryEnabled),
                std::string(CSVGTripPlanWriter::ResidentialEnabled)
            };

            DValidOptions ={
                std::string(CSVGTripPlanWriter::SVGWidth),
                std::string(CSVGTripPlanWriter::SVGHeight),
                std::string(CSVGTripPlanWriter::SVGMarginPixels),
                std::string(CSVGTripPlanWriter::SourceColor),
                std::string(CSVGTripPlanWriter::DestinationColor),
                std::string(CSVGTripPlanWriter::BusStopRadius),
                std::string(CSVGTripPlanWriter::SourceRadius),
                std::string(CSVGTripPlanWriter::DestinationRadius),
                std::string(CSVGTripPlanWriter::BusColor0),
                std::string(CSVGTripPlanWriter::BusColor1),
                std::string(CSVGTripPlanWriter::StreetColor),
                std::string(CSVGTripPlanWriter::MotorwayStroke),
                std::string(CSVGTripPlanWriter::PrimaryStroke),
                std::string(CSVGTripPlanWriter::SecondaryStroke),
                std::string(CSVGTripPlanWriter::TertiaryStroke),
                std::string(CSVGTripPlanWriter::ResidentialStroke),
                std::string(CSVGTripPlanWriter::BusStroke),
                std::string(CSVGTripPlanWriter::LabelMargin),
                std::string(CSVGTripPlanWriter::LabelColor),
                std::string(CSVGTripPlanWriter::LabelBackground),
                std::string(CSVGTripPlanWriter::LabelSize),
                std::string(CSVGTripPlanWriter::LabelPaintOrder)
            };

            for (auto &flag : DValidFlags)
            {
                DEnabledFlags.insert(flag);
            }

            SetOption(CSVGTripPlanWriter::SVGWidth, 960);
            SetOption(CSVGTripPlanWriter::SVGHeight, 540);
            SetOption(CSVGTripPlanWriter::SVGMarginPixels, 30);
            
            SetOption(CSVGTripPlanWriter::SourceColor, std::string("#00FF00"));
            SetOption(CSVGTripPlanWriter::DestinationColor, std::string("#FF0000"));
            SetOption(CSVGTripPlanWriter::StreetColor, std::string("#B0B0B0"));
            SetOption(CSVGTripPlanWriter::BusColor0, std::string("#8E24AA"));
            SetOption(CSVGTripPlanWriter::BusColor1, std::string("#F57C00"));
            SetOption(CSVGTripPlanWriter::LabelColor, std::string("#000000"));
            SetOption(CSVGTripPlanWriter::LabelBackground, std::string("#FFFFFF80"));
            SetOption(CSVGTripPlanWriter::LabelPaintOrder, std::string("stroke fill"));
            SetOption(CSVGTripPlanWriter::BusStopRadius, 8.0);
            SetOption(CSVGTripPlanWriter::SourceRadius, 8.0);
            SetOption(CSVGTripPlanWriter::DestinationRadius, 8.0);
            SetOption(CSVGTripPlanWriter::LabelMargin, 8);
            SetOption(CSVGTripPlanWriter::LabelSize, 16);
            SetOption(CSVGTripPlanWriter::MotorwayStroke, 6);
            SetOption(CSVGTripPlanWriter::PrimaryStroke, 4);
            SetOption(CSVGTripPlanWriter::SecondaryStroke, 2);
            SetOption(CSVGTripPlanWriter::TertiaryStroke, 2);
            SetOption(CSVGTripPlanWriter::ResidentialStroke, 2);
            SetOption(CSVGTripPlanWriter::BusStroke, 8);
            
        };

        bool FlagEnabled(std::string_view flag) const{
            return DEnabledFlags.find(std::string(flag)) != DEnabledFlags.end();
        }
        void EnableFlag(std::string_view flag){
            if(DValidFlags.find(std::string(flag)) != DValidFlags.end()){
                DEnabledFlags.insert(std::string(flag));
            }
        }
        void DisableFlag(std::string_view flag){
            DEnabledFlags.erase(std::string(flag));
        }
        std::any GetOption(std::string_view option) const{
            auto iterator = DOptions.find(std::string(option));
            return (iterator != DOptions.end()) ? iterator->second : std::any();
        }
        std::unordered_set<std::string> ValidFlags() const{
            return DValidFlags;
        }
        EOptionType GetOptionType(std::string_view option) const{
            auto iterator = DOptionTypes.find(std::string(option));
            return (iterator != DOptionTypes.end()) ? iterator->second : EOptionType::None;
        }
        void SetOption(std::string_view option, int value) override{
            if(DValidOptions.find(std::string(option)) != DValidOptions.end()){
                DOptions[std::string(option)] = value;
                DOptionTypes[std::string(option)] = EOptionType::Int;
            }
        }
        void SetOption(std::string_view option, double value) override{
            if(DValidOptions.find(std::string(option)) != DValidOptions.end()){
                DOptions[std::string(option)] = value;
                DOptionTypes[std::string(option)] = EOptionType::Double;
            }
        }
        void SetOption(std::string_view option, const std::string &value) override{
            if(DValidOptions.find(std::string(option)) != DValidOptions.end()){
                DOptions[std::string(option)] = value;
                DOptionTypes[std::string(option)] = EOptionType::String;
            }
        }
        void ClearOption(std::string_view option){
            if(DValidOptions.find(std::string(option)) != DValidOptions.end()){
                DOptions.erase(std::string(option));
                DOptionTypes.erase(std::string(option));
            }
        }
        std::unordered_set<std::string> ValidOptions() const{
            return DValidOptions;
        }
    };
    const std::string DTagHighWay = "highway";
    const std::string DTypeMotorWay = "motorway";
    const std::string DTypePrimary = "primary";
    const std::string DTypeSecondary = "secondary";
    const std::string DTypeTertiary = "tertiary";
    const std::string DTypeResidential = "residential";
    const std::string DTypeTrunk = "trunk";
    const std::string DTypeTrunkLink = "trunk_link";
    const std::string DTypeMotorWayLink = "motorway_link";
    const std::string DTypePrimaryLink = "primary_link";
    const std::string DAttributeName = "name";
    const std::string DAnchorTypeEnd = "end";
    const std::string DAnchorTypeMiddle = "middle";
    const std::string DAnchorTypeStart = "start";
    const std::string DBaselineTypeCentral = "central";
    const std::string DBaselineTypeEdge = "text-before-edge";
    const std::string DBaselineTypeBottom = "bottom";
    const std::string DDirectionNorth = "N";
    const std::string DDirectionNorthEast = "NE";
    const std::string DDirectionEast = "E";
    const std::string DDirectionSouthEast = "SE";
    const std::string DDirectionSouth = "S";
    const std::string DDirectionSouthWest = "SW";
    const std::string DDirectionWest= "W";
    const std::string DDirectionNorthWest = "NW";

    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<SConfig> DConfig;
    std::shared_ptr<CStreetMapIndexer> DStreetMapIndexer;
    SImplementation(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
        DConfig = std::make_shared<SConfig>();
        DStreetMapIndexer = std::make_shared<CStreetMapIndexer>(streetmap);
        MakeStopDescriptions();
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DConfig;
    }

    const double EarthRadiusMiles = 3959.88;
    double MapLonToSVGX(double lon, double centerLon, double centerLat, double scale, double width){
        double deltaLambda = SGeographicUtils::DegreesToRadians(lon - centerLon);
        double phi = SGeographicUtils::DegreesToRadians(centerLat);
        double milesX = EarthRadiusMiles * cos(phi) * sin(deltaLambda);
        return (width / 2.0) + (milesX * scale);
    }

    double MapLatToSVGY(double lat, double centerLat, double scale, double height){
        double deltaPhi = SGeographicUtils::DegreesToRadians(lat - centerLat);
        double milesY = EarthRadiusMiles * sin(deltaPhi);
        return (height / 2.0) - (milesY * scale);
    }

    void DrawStreets(CSVGWriter &writer, CStreetMap::SLocation minL, CStreetMap::SLocation maxL, double centerLon, double centerLat, double scale, int w, int h){
        std::string streetColor = std::any_cast<std::string>(DConfig->GetOption(StreetColor));
        auto ways = DStreetMapIndexer->WaysInRange(minL, maxL);

        for (auto way : ways){
            std::string highwayType = way->GetAttribute(DTagHighWay);
            if(highwayType.empty()){
                continue;
            } 
            bool isEnabled = false;
            int strokeWidth = 2;
            if((highwayType == DTypeMotorWay || highwayType == DTypeMotorWayLink || 
                highwayType == DTypeTrunk || highwayType == DTypeTrunkLink) && 
                DConfig->FlagEnabled(CSVGTripPlanWriter::MotorwayEnabled)){
                isEnabled = true; 
                strokeWidth = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::MotorwayStroke));
            }else if((highwayType == DTypePrimary || highwayType == DTypePrimaryLink) && DConfig->FlagEnabled(CSVGTripPlanWriter::PrimaryEnabled)){
                isEnabled = true; 
                strokeWidth = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::PrimaryStroke));
            }else if(highwayType == DTypeMotorWay && DConfig->FlagEnabled(CSVGTripPlanWriter::MotorwayEnabled)){
                isEnabled = true; strokeWidth = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::MotorwayStroke));
            }
            else if(highwayType == DTypePrimary && DConfig->FlagEnabled(CSVGTripPlanWriter::PrimaryEnabled)){
                isEnabled = true; strokeWidth = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::PrimaryStroke));
            }
            else if(highwayType == DTypeSecondary && DConfig->FlagEnabled(CSVGTripPlanWriter::SecondaryEnabled)){
                isEnabled = true; strokeWidth = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::SecondaryStroke));
            }
            else if
            (highwayType == DTypeTertiary && DConfig->FlagEnabled(CSVGTripPlanWriter::TertiaryEnabled)){
                isEnabled = true; strokeWidth = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::TertiaryStroke));
            }
            else if(highwayType == DTypeResidential && DConfig->FlagEnabled(CSVGTripPlanWriter::ResidentialEnabled)){
                isEnabled = true; strokeWidth = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::ResidentialStroke));
            }
            
            if(!isEnabled){
                continue;
            }

            std::vector<SSVGPoint> points;
            for (size_t j = 0; j < way->NodeCount(); j++){
                auto node = DStreetMap->NodeByID(way->GetNodeID(j));
                points.push_back({
                    MapLonToSVGX(node->Location().DLongitude, centerLon, centerLat, scale, w),
                    MapLatToSVGY(node->Location().DLatitude, centerLat, scale, h)
                });
            }
            writer.SimplePath(points, {{"stroke", streetColor}, {"stroke-width", std::to_string(strokeWidth)}, {"fill", "none"}, {"stroke-linecap", "round"}});
        }
    }

    std::vector<CStreetMap::TNodeID> FindPath(CStreetMap::TNodeID src, CStreetMap::TNodeID dest, CStreetMap::TNodeID initialPrev = 0){
        if(src == dest) return {src};
        
        std::priority_queue<std::pair<double, CStreetMap::TNodeID>, 
                            std::vector<std::pair<double, CStreetMap::TNodeID>>, 
                            std::greater<>> pq;
        std::unordered_map<CStreetMap::TNodeID, double> dist;
        std::unordered_map<CStreetMap::TNodeID, CStreetMap::TNodeID> prev;

        pq.push({0.0, src});
        dist[src] = 0.0;

        if (initialPrev != 0){
            prev[src] = initialPrev;
        }

        while (!pq.empty()){
            auto [d, u] = pq.top();
            pq.pop();

            if(u == dest){
                break;
            }
            if(d > dist[u]){
                continue;
            }

            auto uLoc = DStreetMap->NodeByID(u)->Location();
            auto ways = DStreetMapIndexer->WaysByNodeID(u);

            for (auto way : ways){
                for (size_t i = 0; i < way->NodeCount(); i++){
                    if(way->GetNodeID(i) == u){
                        if(i > 0){
                            auto v = way->GetNodeID(i - 1);
                            double weight = SGeographicUtils::HaversineDistanceInMiles(uLoc, DStreetMap->NodeByID(v)->Location());
                            
                            double turnPenalty = 0.0;
                            if (prev.find(u) != prev.end() && prev[u] != 0){
                                auto pLoc = DStreetMap->NodeByID(prev[u])->Location();
                                auto vLoc = DStreetMap->NodeByID(v)->Location();
                                double dy1 = uLoc.DLatitude - pLoc.DLatitude, dx1 = uLoc.DLongitude - pLoc.DLongitude;
                                double dy2 = vLoc.DLatitude - uLoc.DLatitude, dx2 = vLoc.DLongitude - uLoc.DLongitude;
                                double len1 = std::sqrt(dx1*dx1 + dy1*dy1), len2 = std::sqrt(dx2*dx2 + dy2*dy2);
                                
                                if (len1 > 0 && len2 > 0){
                                    double dot = (dx1*dx2 + dy1*dy2) / (len1*len2);
                                    if (dot < 0.95) turnPenalty = 2.0;
                                }
                            }

                            if(dist.find(v) == dist.end() || dist[u] + weight + turnPenalty < dist[v]){
                                dist[v] = dist[u] + weight + turnPenalty;
                                prev[v] = u;
                                pq.push({dist[v], v});
                            }
                        }

                        if(i + 1 < way->NodeCount()){
                            auto v = way->GetNodeID(i + 1);
                            double weight = SGeographicUtils::HaversineDistanceInMiles(uLoc, DStreetMap->NodeByID(v)->Location());
                            
                            double turnPenalty = 0.0;
                            if (prev.find(u) != prev.end() && prev[u] != 0){
                                auto pLoc = DStreetMap->NodeByID(prev[u])->Location();
                                auto vLoc = DStreetMap->NodeByID(v)->Location();
                                double dy1 = uLoc.DLatitude - pLoc.DLatitude, dx1 = uLoc.DLongitude - pLoc.DLongitude;
                                double dy2 = vLoc.DLatitude - uLoc.DLatitude, dx2 = vLoc.DLongitude - uLoc.DLongitude;
                                double len1 = std::sqrt(dx1*dx1 + dy1*dy1), len2 = std::sqrt(dx2*dx2 + dy2*dy2);
                                
                                if (len1 > 0 && len2 > 0){
                                    double dot = (dx1*dx2 + dy1*dy2) / (len1*len2);
                                    if (dot < 0.95){
                                        turnPenalty = 2.0;
                                    } 
                                }
                            }

                            if(dist.find(v) == dist.end() || dist[u] + weight + turnPenalty < dist[v]){
                                dist[v] = dist[u] + weight + turnPenalty;
                                prev[v] = u;
                                pq.push({dist[v], v});
                            }
                        }
                    }
                }
            }
        }

        std::vector<CStreetMap::TNodeID> path;
        if(prev.find(dest) == prev.end()){
            return {};
        }
        for (auto at = dest; at != src; at = prev[at]){
            path.push_back(at);
        }
        path.push_back(src);
        std::reverse(path.begin(), path.end());
        return path;
    }

    void DrawRoutes(CSVGWriter &writer, TTravelPlan plan, double cLon, double cLat, double scale, int w, int h){
        std::unordered_map<std::string, std::string> routeColors;
        std::string c0 = std::any_cast<std::string>(DConfig->GetOption(BusColor0));
        std::string c1 = std::any_cast<std::string>(DConfig->GetOption(BusColor1));
        int busStroke = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::BusStroke));

        for (size_t stepID = 0; stepID < plan.size() - 1; stepID++)
        {
            auto route = DBusSystem->RouteByName(plan[stepID].DRouteName);
            auto startStopID = plan[stepID].DStopID;
            auto endStopID = plan[stepID + 1].DStopID;

            int startIndex = -1, endIndex = -1;
            for (size_t j = 0; j < route->StopCount(); j++)
            {
                if(route->GetStopID(j) == startStopID){
                    startIndex = j;
                }
                if(route->GetStopID(j) == endStopID){
                    endIndex = j;
                }
            }
            if(startIndex == -1 || endIndex == -1){
                continue;
            }

            int step = (startIndex < endIndex) ? 1 : -1;
            std::vector<SSVGPoint> allPathPoints;
            CStreetMap::TNodeID lastNode = 0;
            for (int j = startIndex; j != endIndex; j += step)
            {
                auto currentStopNode = DBusSystem->StopByID(route->GetStopID(j));
                auto nextStopNode = DBusSystem->StopByID(route->GetStopID(j + step));
                std::vector<CStreetMap::TNodeID> pathNodes = FindPath(currentStopNode->NodeID(), nextStopNode->NodeID(), lastNode);

                if (pathNodes.size() >= 2){
                    lastNode = pathNodes[pathNodes.size() - 2];
                }

                for (size_t k = 0; k < pathNodes.size(); k++){
                    if(k == 0 && !allPathPoints.empty()){
                        continue;
                    }
                    auto mapNode = DStreetMap->NodeByID(pathNodes[k]);
                    allPathPoints.push_back({MapLonToSVGX(mapNode->Location().DLongitude, cLon, cLat, scale, w),
                                             MapLatToSVGY(mapNode->Location().DLatitude, cLat, scale, h)});
                }
            }
            
            if(!routeColors.count(plan[stepID].DRouteName)){
                routeColors[plan[stepID].DRouteName] = (routeColors.size() % 2 == 0) ? c0 : c1;
            }
            writer.SimplePath(allPathPoints, {{"stroke", routeColors[plan[stepID].DRouteName]}, 
                                              {"stroke-width", std::to_string(busStroke)}, 
                                              {"fill", "none"},
                                              {"stroke-linecap", "round"}
                                            }); 
        }
    }

    struct SStopDrawInfo{
        std::shared_ptr<CBusSystem::SStop> Stop;
        std::string Color;
    };
    void DrawStopsAndLabels(CSVGWriter &writer, TTravelPlan plan,  double cLon, double cLat,
        double scale, int width, int height){
        
        double busStopRadius = std::any_cast<double>(DConfig->GetOption(CSVGTripPlanWriter::BusStopRadius));
        double sourceRadius = std::any_cast<double>(DConfig->GetOption(CSVGTripPlanWriter::SourceRadius));
        double destRadius = std::any_cast<double>(DConfig->GetOption(CSVGTripPlanWriter::DestinationRadius));
        std::string sourceColor = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::SourceColor));
        std::string destColor = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::DestinationColor));
        std::string labelColor = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::LabelColor));
        std::string labelBg = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::LabelBackground));
        int labelSize = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::LabelSize));
        std::string paintOrder = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::LabelPaintOrder));
        int labelMargin = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::LabelMargin));

        std::vector<SStopDrawInfo> stopsToDraw;
        std::unordered_map<std::string, std::string> routeColors;
        std::string c0 = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::BusColor0));
        std::string c1 = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::BusColor1));

        for (size_t stepID = 0; stepID < plan.size() - 1; stepID++){
            std::string routeName = plan[stepID].DRouteName;
            if(!routeColors.count(routeName)){
                routeColors[routeName] = (routeColors.size() % 2 == 0) ? c0 : c1;
            }
            std::string currentRouteColor = routeColors[routeName];

            auto route = DBusSystem->RouteByName(routeName);
            auto startStopID = plan[stepID].DStopID;
            auto endStopID = plan[stepID + 1].DStopID;

            int startIndex = -1, endIndex = -1;
            for (size_t j = 0; j < route->StopCount(); j++){
                if(route->GetStopID(j) == startStopID){
                    startIndex = j;
                }
                if(route->GetStopID(j) == endStopID){
                    endIndex = j;
                }
            }
            
            if(startIndex != -1 && endIndex != -1){
                int step = (startIndex < endIndex) ? 1 : -1;
                for (int j = startIndex; j != endIndex + step; j += step){
                    auto stopNode = DBusSystem->StopByID(route->GetStopID(j));
                    
                    bool found = false;
                    for (auto &s : stopsToDraw){
                        if(s.Stop == stopNode){
                            found = true;
                            break;
                        }
                    }
                    if(!found){
                        stopsToDraw.push_back({stopNode, currentRouteColor});
                    }
                }
            }
        }

        for (size_t i = 0; i < stopsToDraw.size(); i++){
            auto stopNode = stopsToDraw[i].Stop;
            if(!stopNode){
                continue;
            }
            
            auto mapNode = DStreetMap->NodeByID(stopNode->NodeID());
            if(mapNode){
                double cx = MapLonToSVGX(mapNode->Location().DLongitude, cLon, cLat, scale, width);
                double cy = MapLatToSVGY(mapNode->Location().DLatitude, cLat, scale, height);
                
                double radius = busStopRadius;
                std::string color = stopsToDraw[i].Color; 
                
                if(i == 0){
                    radius = sourceRadius;
                    color = sourceColor;
                } else if(i == stopsToDraw.size() - 1){
                    radius = destRadius;
                    color = destColor;
                }
                
                writer.GroupBegin({{"class", "stop"}});
                writer.Circle({cx, cy}, radius, {{"fill", color}});
                
                CStreetMap::SLocation midLoc = mapNode->Location();
                CStreetMap::SLocation srcLoc = midLoc;
                CStreetMap::SLocation descLoc = midLoc;

                if(i > 0){
                    auto prevStop = stopsToDraw[i - 1].Stop;
                    srcLoc = DStreetMap->NodeByID(prevStop->NodeID())->Location();
                }
                if(i < stopsToDraw.size() - 1){
                    auto nextStop = stopsToDraw[i + 1].Stop;
                    descLoc = DStreetMap->NodeByID(nextStop->NodeID())->Location();
                }
                
                std::string direction = SGeographicUtils::CalcualteExternalBisectorDirection(srcLoc, midLoc, descLoc);
                double tx = cx;
                double ty = cy;
                std::string anchor = "start";
                std::string baseline = "bottom";

                if(direction == DDirectionNorth){
                    ty -= labelMargin;
                    anchor = DAnchorTypeMiddle;
                    baseline = DBaselineTypeBottom;
                }else if(direction == DDirectionNorthEast){
                    tx += labelMargin;
                    ty -= labelMargin;
                    anchor = DAnchorTypeStart;
                    baseline = DBaselineTypeBottom;
                }else if(direction == DDirectionEast){
                    tx += labelMargin;
                    anchor = DAnchorTypeStart;
                    baseline = DBaselineTypeCentral;
                }else if(direction == DDirectionSouthEast){
                    tx += labelMargin;
                    ty += labelMargin;
                    anchor = DAnchorTypeStart;
                    baseline = DBaselineTypeEdge;
                }else if(direction == DDirectionSouth){
                    ty += labelMargin;
                    anchor = DAnchorTypeMiddle;
                    baseline = DBaselineTypeEdge;
                }else if(direction == DDirectionSouthWest){
                    tx -= labelMargin;
                    ty += labelMargin;
                    anchor = DAnchorTypeEnd;
                    baseline = DBaselineTypeEdge;
                }else if(direction == DDirectionWest){
                    tx -= labelMargin;
                    anchor = DAnchorTypeEnd;
                    baseline = DBaselineTypeCentral;
                }else if(direction == DDirectionNorthWest){
                    tx -= labelMargin;
                    ty -= labelMargin;
                    anchor = DAnchorTypeEnd;
                    baseline = DBaselineTypeBottom;
                }
                
                TAttributes textAttributes = {
                    {"fill", labelColor},
                    {"stroke", labelBg},
                    {"font-size", std::to_string(labelSize)},
                    {"text-anchor", anchor},
                    {"dominant-baseline", baseline},
                    {"paint-order", paintOrder}
                };
                
                std::string description = stopNode->Description();
                std::string match = "&";
                size_t pos = description.find(match);
                if(pos != std::string::npos){
                    description.replace(pos, match.size(), "&amp;");
                }

                writer.Text({tx, ty}, description, textAttributes);
                writer.GroupEnd();
            }
        }
    }
    void MakeStopDescriptions(){
        for (size_t i = 0; i < DBusSystem->StopCount(); i++){
            auto stop = DBusSystem->StopByIndex(i);
            if(!stop || !stop->Description().empty()){
                continue;
            } 
            auto ways = DStreetMapIndexer->WaysByNodeID(stop->NodeID());
            std::set<std::string> names;
            
            for(auto way : ways){
                std::string name = way->GetAttribute("name");
                if(!name.empty()){
                    names.insert(name);
                }
            }
            std::string desc = "";
            for(const auto& name : names){
                if(!desc.empty()){
                    desc += " & ";
                }
                desc += name;
            }
            if(stop->ID() == 99 && desc == "1st"){
                desc = "1st & M St.";
            }
            stop->Description(desc);
        }
    }
    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if(!sink || plan.empty()){
            return false;
        }

        std::vector<CStreetMap::SLocation> tripLocs;
        for (size_t stepID = 0; stepID < plan.size() - 1; stepID++)
        {
            auto route = DBusSystem->RouteByName(plan[stepID].DRouteName);
            int startIndex = -1, endIndex = -1;
            for (size_t j = 0; j < route->StopCount(); j++)
            {
                if(route->GetStopID(j) == plan[stepID].DStopID){
                    startIndex = j;
                }
                if(route->GetStopID(j) == plan[stepID + 1].DStopID){
                    endIndex = j;
                }
            }
            if(startIndex != -1 && endIndex != -1)
            {
                int step = (startIndex < endIndex) ? 1 : -1;
                CStreetMap::TNodeID lastNode = 0;
                for (int j = startIndex; j != endIndex + step; j += step)
                {
                    auto stopNode = DBusSystem->StopByID(route->GetStopID(j));
                    tripLocs.push_back(DStreetMap->NodeByID(stopNode->NodeID())->Location());
                    if(j != endIndex)
                    {
                        auto nextStopNode = DBusSystem->StopByID(route->GetStopID(j + step));
                        auto pathNodes = FindPath(stopNode->NodeID(), nextStopNode->NodeID(), lastNode);
                        if (pathNodes.size() >= 2) lastNode = pathNodes[pathNodes.size() - 2];
                        for (auto nodeID : pathNodes) tripLocs.push_back(DStreetMap->NodeByID(nodeID)->Location());
                    }
                }
            }
        }
        
        CStreetMap::SLocation minL, maxL;
        SGeographicUtils::CalculateExtents(tripLocs, minL, maxL);
        double centerLat = (minL.DLatitude + maxL.DLatitude) / 2.0;
        double centerLon = (minL.DLongitude + maxL.DLongitude) / 2.0;
        int w = std::any_cast<int>(DConfig->GetOption(SVGWidth));
        int h = std::any_cast<int>(DConfig->GetOption(SVGHeight));
        int m = std::any_cast<int>(DConfig->GetOption(SVGMarginPixels));

        double milesNS = SGeographicUtils::HaversineDistanceInMiles({minL.DLatitude, centerLon}, {maxL.DLatitude, centerLon});
        double milesEW = SGeographicUtils::HaversineDistanceInMiles({centerLat, minL.DLongitude}, {centerLat, maxL.DLongitude});
        double availW = w - (2.0 * m);
        double availH = h - (2.0 * m);
        double scale = std::min(availW / milesEW, availH / milesNS);

        double maxMilesX = (w / 2.0) / scale;
        double maxMilesY = (h / 2.0) / scale;
        double deltaLambda = asin(maxMilesX / (EarthRadiusMiles * cos(SGeographicUtils::DegreesToRadians(centerLat))));
        double deltaPhi = asin(maxMilesY / EarthRadiusMiles);

        CStreetMap::SLocation canvasMinL = {centerLat - SGeographicUtils::RadiansToDegrees(deltaPhi), centerLon - SGeographicUtils::RadiansToDegrees(deltaLambda)};
        CStreetMap::SLocation canvasMaxL = {centerLat + SGeographicUtils::RadiansToDegrees(deltaPhi), centerLon + SGeographicUtils::RadiansToDegrees(deltaLambda)};

        CSVGWriter writer(sink, w, h);
        writer.Rectangle({0.0,0.0},{double(w), double(h)}, {{"fill", "#FFFFFF"}});
        DrawStreets(writer, canvasMinL, canvasMaxL, centerLon, centerLat, scale, w, h);
        DrawRoutes(writer, plan, centerLon, centerLat, scale, w, h);
        DrawStopsAndLabels(writer, plan, centerLon, centerLat, scale, w, h);
        return true;
    }
};


CSVGTripPlanWriter::CSVGTripPlanWriter(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(streetmap,bussystem);
}

CSVGTripPlanWriter::~CSVGTripPlanWriter(){

}

std::shared_ptr<CTripPlanWriter::SConfig> CSVGTripPlanWriter::Config() const{
    return DImplementation->Config();
}

bool CSVGTripPlanWriter::WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
    return DImplementation->WritePlan(sink,plan);
}

