#include "TextTripPlanWriter.h"
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <chrono>

struct CTextTripPlanWriter::SImplementation{

    struct SConfig : public CTripPlanWriter::SConfig{
        std::unordered_set<std::string> DEnabledFlags;
        std::unordered_map<std::string, std::any> DOptions;
        std::unordered_map<std::string, EOptionType> DOptionTypes;

        std::unordered_set<std::string> DValidFlags;
        std::unordered_set<std::string> DValidOptions;
        
        SConfig(){
            DValidFlags = {std::string(Verbose)};
        }

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

    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<SConfig> DConfig;

    SImplementation(std::shared_ptr<CBusSystem> bussystem){
        DBusSystem = bussystem;
        DConfig = std::make_shared<SConfig>();
    }

    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DConfig;
    }

    std::string FormatTime(CBusSystem::TStopTime stopTime) {
        auto hours = stopTime.hours().count();
        auto minutes = stopTime.minutes().count();
        std::string period = (hours >= 12) ? "PM" : "AM";
        int displayHour = hours % 12;
        if(displayHour == 0){
            displayHour = 12;
        }


        std::ostringstream oss;
        oss << std::setw(2) << displayHour << ":" 
            << std::setfill('0') << std::setw(2) << minutes 
            << " " << period;
    
        return oss.str();
    }

    void WriteVerboseStops(std::ostringstream &oss, const CTripPlanner::STravelStep &start, const CTripPlanner::STravelStep &end){
        auto route = DBusSystem->RouteByName(start.DRouteName);
        if(!route){
            return;
        }

        size_t currenTrip =0;
        size_t startStopIndex =0;
        bool foundTrip = false;

        for (size_t i = 0; i < route->TripCount(); i++)
        {
            for (size_t j = 0; j < route->StopCount(); j++)
            {   
                auto routeTime = route->GetStopTime(j, i);
                auto startTime = start.DTime;
                if(route->GetStopID(j) == start.DStopID && routeTime.to_duration() == startTime.to_duration()){
                    currenTrip = i;
                    startStopIndex = j;
                    foundTrip = true;
                    break;
                }
            }
            if(foundTrip){
                break;
            }
        }
        if(!foundTrip){
            return;
        }
        
        for (size_t i = startStopIndex + 1; i < route->StopCount(); i++)
        {
            auto currentStopID = route->GetStopID(i);
            if(currentStopID == end.DStopID){
                break;
            }
            auto stopNode = DBusSystem->StopByID(currentStopID);
            std::string stopDescription = (stopNode != nullptr) ? stopNode->Description() : "Unknown Stop";
            auto stopTime = route->GetStopTime(i, currenTrip);
            oss << FormatTime(stopTime) << ": Stay on the " << start.DRouteName
            << " bus at " << stopDescription << " (stop "<< currentStopID << ").\n";
        }
        
    }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if(!sink | plan.empty()){
            return false;
        }
        std::ostringstream oss;
        bool isVerbose = DConfig->FlagEnabled(Verbose);
        for (size_t i = 0; i < plan.size(); i++)
        {
            const auto &step = plan[i];
            auto stopObj = DBusSystem->StopByID(step.DStopID);
            auto stopDesc = stopObj ? stopObj->Description() : "Unknown Stop";
            std::string timeStr = FormatTime(step.DTime);

            if(i == 0){
                oss << timeStr << ": Take the " << step.DRouteName << " bus from " 
                << stopDesc << " (stop " << step.DStopID << ").\n";
                if(isVerbose && (i + 1 < plan.size())){
                    WriteVerboseStops(oss, plan[i], plan[i+1]);
                }
            }
            else if(i == plan.size() - 1){
                std::string arrivalRoute = plan[i-1].DRouteName;
                oss << timeStr << ": Get off the " << arrivalRoute << " bus at " 
                    << stopDesc << " (stop " << step.DStopID << ").\n";
            }
            else{
                std::string prevRoute = plan[i-1].DRouteName;
                oss << "        : Get off the " << prevRoute << " bus at " << stopDesc 
                << " (stop " << step.DStopID << ") and wait for the " 
                << step.DRouteName << " bus.\n";
                
                oss << timeStr << ": Take the " << step.DRouteName << " bus from " 
                << stopDesc << " (stop " << step.DStopID << ").\n";
                
                if(isVerbose && (i + 1 < plan.size())){
                    WriteVerboseStops(oss, plan[i], plan[i+1]);
                }
            }

            

        }
        std::string finalOutput = oss.str();
        std::vector<char> outData(finalOutput.begin(), finalOutput.end());
        sink->Write(outData);
        return true;
    }
};



CTextTripPlanWriter::CTextTripPlanWriter(std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(bussystem);
}

CTextTripPlanWriter::~CTextTripPlanWriter(){

}

std::shared_ptr<CTripPlanWriter::SConfig> CTextTripPlanWriter::Config() const{
    return DImplementation->Config();
}

bool CTextTripPlanWriter::WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
    return DImplementation->WritePlan(sink,plan);
}

