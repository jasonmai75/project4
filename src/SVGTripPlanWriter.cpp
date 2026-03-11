#include "SVGTripPlanWriter.h"
#include <unordered_map>
#include <unordered_set>
#include "GeographicUtils.h"
#include "SVGWriter.h"
#include "StreetMapIndexer.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

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
        void SetOption(std::string_view option, int value){
            if(DValidOptions.find(std::string(option)) != DValidOptions.end()){
                DOptions[std::string(option)] = value;
                DOptionTypes[std::string(option)] = EOptionType::Int;
            }
        }
        void SetOption(std::string_view option, double value){
            if(DValidOptions.find(std::string(option)) != DValidOptions.end()){
                DOptions[std::string(option)] = value;
                DOptionTypes[std::string(option)] = EOptionType::Double;
            }
        }
        void SetOption(std::string_view option, const std::string &value){
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
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DConfig;
    }

    double MapLonToSVGX(double lon, double minLon, double maxLon, double width, double margin){
        if(maxLon == minLon){
            return width/2.0;
        }
        return margin + ((lon-minLon) / (maxLon-minLon) ) * (width - 2.0 * margin);
    }

    double MapLatToSVGY(double lat, double minLat, double maxLat, int height, double margin){
        if(maxLat == minLat){
            return height/2.0;
        }
        return margin + (maxLat - lat) * (height - 2.0 * margin) / (maxLat - minLat);
    }

    void DrawStreets(CSVGWriter &writer, CStreetMap::SLocation minLoc, CStreetMap::SLocation maxLoc,
        int width, int height, int margin){
        std::string streetColor = std::any_cast<std::string>(DConfig->GetOption(StreetColor));
        std::vector<CStreetMap::SLocation> allNodeLocs;
        for(size_t i = 0; i < DStreetMap->NodeCount(); i++){
            auto node = DStreetMap->NodeByIndex(i);
            if(node) allNodeLocs.push_back(node->Location());
        }
        CStreetMap::SLocation fullMin, fullMax;
        if(!allNodeLocs.empty()){
            SGeographicUtils::CalculateExtents(allNodeLocs, fullMin, fullMax);
        } else {
            fullMin = minLoc;
            fullMax = maxLoc;
        }
        auto waysInRange = DStreetMapIndexer->WaysInRange(fullMin, fullMax);

        for (auto &way : waysInRange)
        {
            std::cout<<way->ID()<<std::endl;
            std::string highwayType = way->GetAttribute(DTagHighWay);
            if(highwayType == DTypeMotorWay && !DConfig->FlagEnabled(MotorwayEnabled)){
                continue;
            } else if(highwayType == DTypePrimary && !DConfig->FlagEnabled(PrimaryEnabled)){
                continue;
            } else if(highwayType == DTypeSecondary && !DConfig->FlagEnabled(SecondaryEnabled)){
                continue;
            } else if(highwayType == DTypeTertiary && !DConfig->FlagEnabled(TertiaryEnabled)){
                continue;
            } else if(highwayType == DTypeResidential && !DConfig->FlagEnabled(ResidentialEnabled)){
                continue;
            }
            int strokeWidth = std::any_cast<int>(DConfig->GetOption(ResidentialStroke));
            if(highwayType == DTypeMotorWay){
                strokeWidth = std::any_cast<int>(DConfig->GetOption(MotorwayStroke));
            }else if(highwayType == DTypePrimary ){
                strokeWidth = std::any_cast<int>(DConfig->GetOption(PrimaryStroke));
            }else if(highwayType == DTypeSecondary){
                strokeWidth = std::any_cast<int>(DConfig->GetOption(SecondaryStroke));
            }else if(highwayType == DTypeTertiary){
                strokeWidth = std::any_cast<int>(DConfig->GetOption(TertiaryStroke));
            }

            std::vector<SSVGPoint> points;
            for (size_t i = 0; i < way->NodeCount(); i++)
            {
                auto node = DStreetMap->NodeByID(way->GetNodeID(i));
                if(node){
                    double x = MapLonToSVGX(node->Location().DLongitude, minLoc.DLongitude, maxLoc.DLongitude, width, margin);
                    double y = MapLatToSVGY (node->Location().DLatitude, minLoc.DLatitude, maxLoc.DLatitude, height, margin);
                    points.push_back({x, y});
                }
            }
            TAttributes style = {
                {"stroke", streetColor},
                {"stroke-width", std::to_string(strokeWidth)},
                {"fill", "none"}
            };
            writer.SimplePath(points, style);
        }       
    }

    void DrawRoutes(CSVGWriter &writer, TTravelPlan plan,  CStreetMap::SLocation minLoc, CStreetMap::SLocation maxLoc,
        int width, int height, int margin){
        int busStroke = std::any_cast<int>(DConfig->GetOption(BusStroke));
        std::string color0 = std::any_cast<std::string>(DConfig->GetOption(BusColor0));
        std::string color1 = std::any_cast<std::string>(DConfig->GetOption(BusColor1));
        int colorIndex = 0;
        for (size_t stepID = 0; stepID < plan.size() - 1; stepID++) {
            auto route = DBusSystem->RouteByName(plan[stepID].DRouteName);
            std::cout<< route->Name() << std::endl;
            if (!route){
                continue;
            }

            std::vector<SSVGPoint> points;
            bool inCurrentRoute = false;

            for (size_t j = 0; j < route->StopCount(); j++) {
                auto currentStopID = route->GetStopID(j);
                
                if (currentStopID == plan[stepID].DStopID){
                    inCurrentRoute = true;
                }

                if (inCurrentRoute) {
                    auto stopNode = DBusSystem->StopByID(currentStopID);
                    auto mapNode = DStreetMap->NodeByID(stopNode->NodeID());
                    
                    double x = MapLonToSVGX(mapNode->Location().DLongitude, minLoc.DLongitude, maxLoc.DLongitude, width, margin);
                    double y = MapLatToSVGY(mapNode->Location().DLatitude, minLoc.DLatitude, maxLoc.DLatitude, height, margin);
                    points.push_back({x, y});
                }
                if (currentStopID == plan[stepID+1].DStopID){
                    break;
                }
            }
            std::string busRouteColor = (colorIndex % 2 == 0) ? color0 : color1;
            writer.SimplePath(points, {
                {"stroke", busRouteColor},
                {"stroke-width", std::to_string(busStroke)},
                {"fill", "none"}
            });
            colorIndex++;
        }
        
    }

    void DrawStopsAndLabels(CSVGWriter &writer, TTravelPlan plan,  CStreetMap::SLocation minLoc, CStreetMap::SLocation maxLoc,
        int width, int height, int margin){
        double busStopRadius = std::any_cast<double>(DConfig->GetOption(CSVGTripPlanWriter::BusStopRadius));
        double sourceRadius = std::any_cast<double>(DConfig->GetOption(CSVGTripPlanWriter::SourceRadius));
        double destRadius = std::any_cast<double>(DConfig->GetOption(CSVGTripPlanWriter::DestinationRadius));
        std::string sourceColor = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::SourceColor));
        std::string destColor = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::DestinationColor));
        std::string stopColor = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::BusColor0)); 
        std::string labelColor = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::LabelColor));
        std::string labelBg = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::LabelBackground));
        int labelSize = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::LabelSize));
        std::string paintOrder = std::any_cast<std::string>(DConfig->GetOption(CSVGTripPlanWriter::LabelPaintOrder));
        int labelMargin = std::any_cast<int>(DConfig->GetOption(CSVGTripPlanWriter::LabelMargin));

        for (size_t i = 0; i < plan.size(); i++)
        {
            auto stopNode = DBusSystem->StopByID(plan[i].DStopID);
            if(!stopNode){
                continue;
            }
            auto mapNode = DStreetMap->NodeByID(stopNode->NodeID());

            if(mapNode){
                double cx = MapLonToSVGX(mapNode->Location().DLongitude, minLoc.DLongitude, maxLoc.DLongitude, width, margin);
                double cy = MapLatToSVGY(mapNode->Location().DLatitude, minLoc.DLatitude, maxLoc.DLatitude, height, margin);
                double radius = busStopRadius;
                std::string color = stopColor;
                if(i == 0){
                    radius = sourceRadius;
                    color = sourceColor;
                }else if(i == plan.size() -1 ){
                    radius =destRadius;
                    color = destColor;
                }
                writer.GroupBegin({{"class", "stop"}});
                writer.Circle({cx, cy}, radius, {{"fill", color}});
                
                CStreetMap::SLocation midLoc = mapNode->Location();
                CStreetMap::SLocation srcLoc = midLoc;
                CStreetMap::SLocation descLoc = midLoc;

                if(i>0){
                    auto prevStop = DBusSystem->StopByID(plan[i-1].DStopID);
                    srcLoc = DStreetMap->NodeByID(prevStop->NodeID())->Location();
                }
                if(i < plan.size() -1){
                    auto nextStop = DBusSystem->StopByID(plan[i+1].DStopID);
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
                    description.replace(pos, match.size(),"&amp;");
                }

                writer.Text({tx, ty}, description, textAttributes);
                writer.GroupEnd();
            }
        }
        
    }
    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if(!sink || plan.empty()){
            return false;
        }
        
        std::vector<CStreetMap::SLocation> TravelStepLocations;
        for (auto &step : plan)
        {
            auto stop = DBusSystem->StopByID(step.DStopID);
            if(stop){                
                auto node = DStreetMap->NodeByID(stop->NodeID());
                if(node){
                    TravelStepLocations.push_back(node->Location());
                }
            }
        }
        
        CStreetMap::SLocation MinLocation, MaxLocation;
        SGeographicUtils::CalculateExtents(TravelStepLocations, MinLocation, MaxLocation);
        int width = std::any_cast<int>(DConfig->GetOption(SVGWidth));
        int height = std::any_cast<int>(DConfig->GetOption(SVGHeight));
        int margin = std::any_cast<int>(DConfig->GetOption(SVGMarginPixels));
        CSVGWriter writer(sink, width, height);

        DrawStreets(writer, MinLocation, MaxLocation, width, height, margin);
        DrawRoutes(writer, plan, MinLocation, MaxLocation, width, height, margin);
        DrawStopsAndLabels(writer, plan, MinLocation, MaxLocation, width, height, margin);
        
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

