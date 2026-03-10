#include "TextTripPlanWriter.h"
#include <iomanip>
#include <sstream>

struct CTextTripPlanWriter::SImplementation{

    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<SConfig> DConfig;

    SImplementation(std::shared_ptr<CBusSystem> bussystem){
        DBusSystem = DBusSystem;
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

