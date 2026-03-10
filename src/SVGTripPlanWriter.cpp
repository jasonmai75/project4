#include "SVGTripPlanWriter.h"
#include <unordered_map>
#include <unordered_set>

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
            SetOption(CSVGTripPlanWriter::SVGMarginPixels, 30.0);
            
            SetOption(CSVGTripPlanWriter::SourceColor, std::string("green"));
            SetOption(CSVGTripPlanWriter::DestinationColor, std::string("red"));
            SetOption(CSVGTripPlanWriter::StreetColor, std::string("black"));
            SetOption(CSVGTripPlanWriter::BusColor0, std::string("blue"));
            SetOption(CSVGTripPlanWriter::BusColor1, std::string("purple"));
            SetOption(CSVGTripPlanWriter::LabelColor, std::string("black"));
            SetOption(CSVGTripPlanWriter::LabelBackground, std::string("white"));
            SetOption(CSVGTripPlanWriter::LabelPaintOrder, std::string("stroke fill"));
            SetOption(CSVGTripPlanWriter::BusStopRadius, 4.0);
            SetOption(CSVGTripPlanWriter::SourceRadius, 6.0);
            SetOption(CSVGTripPlanWriter::DestinationRadius, 6.0);
            SetOption(CSVGTripPlanWriter::LabelMargin, 4.0);
            SetOption(CSVGTripPlanWriter::LabelSize, 12);
            SetOption(CSVGTripPlanWriter::MotorwayStroke, 5.0);
            SetOption(CSVGTripPlanWriter::PrimaryStroke, 4.0);
            SetOption(CSVGTripPlanWriter::SecondaryStroke, 3.0);
            SetOption(CSVGTripPlanWriter::TertiaryStroke, 2.0);
            SetOption(CSVGTripPlanWriter::ResidentialStroke, 1.0);
            SetOption(CSVGTripPlanWriter::BusStroke, 3.0);
            
        };

        bool FlagEnabled(std::string_view flag) const{
            return DEnabledFlags.find(std::string(flag)) != DEnabledFlags.end();
        }
        void EnableFlag(std::string_view flag){
            if(DValidFlags.find(std::string(flag)) != DEnabledFlags.end()){
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

    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<SConfig> DConfig;
    SImplementation(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
        DConfig = std::make_shared<SConfig>();
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DConfig;
    }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        return false;
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

