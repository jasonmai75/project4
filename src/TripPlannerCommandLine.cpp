#include "TripPlannerCommandLine.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <map>
#include <iostream>

struct CTripPlannerCommandLine::SImplementation{
    std::shared_ptr<CDataSource> DCommandSource;
    std::shared_ptr<CDataSink> DOutSink;
    std::shared_ptr<CDataSink> DErrorSink;
    std::shared_ptr<CDataFactory> DResultsFactory;
    std::shared_ptr<CTripPlanner> DTripPlanner;
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CTripPlanWriter> DOutWriter;
    std::shared_ptr<CTripPlanWriter> DStorageWriter;
    inline static constexpr std::string_view DExitCommand = "exit";
    inline static constexpr std::string_view DHelpCommand = "help";
    inline static constexpr std::string_view DCountCommand = "count";
    inline static constexpr std::string_view DConfigCommand = "config";
    inline static constexpr std::string_view DToggleCommand = "toggle";
    inline static constexpr std::string_view DSetCommand = "set";
    inline static constexpr std::string_view DStopCommand = "stop";
    inline static constexpr std::string_view DLeaveAtCommand = "leaveat";
    inline static constexpr std::string_view DArriveByCommand = "arriveby";
    inline static constexpr std::string_view DSaveCommand = "save";

    SImplementation(std::shared_ptr<SConfig> config){
        DCommandSource = config->DCommandSource;
        DOutSink = config->DOutSink;
        DErrorSink = config->DErrorSink;
        DResultsFactory = config->DResultsFactory;
        DTripPlanner = config->DTripPlanner;
        DStreetMap = config->DStreetMap;
        DOutWriter = config->DOutWriter;
        DStorageWriter = config->DStorageWriter;
    }

    ~SImplementation(){

    }

    void OutputString(const std::string &str){
        DOutSink->Write(std::vector<char>{str.begin(),str.end()});
    }

    void OutputError(const std::string &str){
        DErrorSink->Write(std::vector<char>{str.begin(),str.end()});
    }

    void OutputPrompt(){
        OutputString("> ");
    }

    std::string InputCommand(){
        std::string Command;
        char TempCh;
        while(!DCommandSource->End()){
            if(DCommandSource->Get(TempCh)){
                if(TempCh == '\n'){
                    return Command;
                }
                Command += std::string(1,TempCh);
            }
        }
        return Command;
    }

    void ParseCommand(const std::string &cmd, std::vector<std::string> &args){
        args.clear();
        size_t Index = 0;
        while(Index < cmd.length()){
            while((Index < cmd.length()) && std::isspace(cmd[Index])){
                Index++;
            }
            std::string Argument;
            while((Index < cmd.length()) && !std::isspace(cmd[Index])){
                Argument += std::string(1,cmd[Index]);
                Index++;
            }
            if(!Argument.empty()){
                args.push_back(Argument);
            }
        }
    }
    std::string FormatLatLong(double val, bool isLat){
        char dir = isLat ? (val >= 0 ? 'N' : 'S') : (val >= 0 ? 'E' : 'W');
        val = std::abs(val);
        int d = std::floor(val);
        val = (val - d) * 60.0;
        int m = std::floor(val);
        double s = (val - m) * 60.0;
        
        std::ostringstream oss;
        oss<< d << "d " << m << "' " << std::fixed << std::setprecision(2) << s << "\" " << dir;
        return oss.str();
    }
    void ExtractConfig(std::shared_ptr<CTripPlanWriter::SConfig> c, std::map<std::string, std::string>& flagsMap, std::map<std::string, std::string>& optionsMap){
        if(!c){
            return;
        }
        for (const auto& f : c->ValidFlags())
        {
            flagsMap[f] = c->FlagEnabled(f) ? "true" : "false";
        }
        for (const auto& o : c->ValidOptions())
        {
            auto type = c->GetOptionType(o);
            auto val = c->GetOption(o);
            if(type == CTripPlanWriter::SConfig::EOptionType::String){
                optionsMap[o] = std::any_cast<std::string>(val);
            }else if(type == CTripPlanWriter::SConfig::EOptionType::Int){
                optionsMap[o] = std::to_string(std::any_cast<int>(val));
            }else if(type == CTripPlanWriter::SConfig::EOptionType::Double){
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(6) << std::any_cast<double>(val);
                optionsMap[o] = ss.str();
            }
        }
    }

    void ToggleConfigFlag(std::shared_ptr<CTripPlanWriter::SConfig> c, const std::string& flag, bool& found, bool& state){
        if(!c){
            return;
        }
        if(c->ValidFlags().count(flag)){
            if(c->FlagEnabled(flag)){
                c->DisableFlag(flag);
            }
            else{
                c->EnableFlag(flag);
            }
            state = c->FlagEnabled(flag);
            found = true;
        }
    }

    void TrySetOption(std::shared_ptr<CTripPlanWriter::SConfig> c, const std::string& option, const std::string& valueStr, bool& found, std::string& printedVal){
        if(!c || c->ValidOptions().count(option) == 0){
            return;
        }
        auto type = c->GetOptionType(option);
        if(type == CTripPlanWriter::SConfig::EOptionType::String){
            c->SetOption(option, valueStr);
            printedVal = valueStr;
            found = true;
        }else if(type == CTripPlanWriter::SConfig::EOptionType::Int){
            int v = std::stoi(valueStr);
            c->SetOption(option, v);
            printedVal = std::to_string(v);
            found = true;
        }else if(type == CTripPlanWriter::SConfig::EOptionType::Double){
            double v = std::stod(valueStr);
            c->SetOption(option, v);
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(6) << v;
            printedVal = ss.str();
            found = true;
        }
    }
    CTripPlanner::TTravelPlan DRecentPlan;
    bool DLastWasLeaveAt = false;
    int DLastTimeQuery = 0;
    CBusSystem::TStopID DLastStartStop = 0;
    CBusSystem::TStopID DLastEndStop = 0;
    bool ProcessCommands(){
        while(!DCommandSource->End()){
            OutputPrompt();
            auto Command = InputCommand();
            std::vector<std::string> Arguments;
            ParseCommand(Command,Arguments);
            if(!Arguments.empty()){
                if(Arguments[0] == DExitCommand){
                    return true;
                }
                else if(Arguments[0] == DHelpCommand){
                    OutputString("--------------------------------------------------------------------------\n"
                                    "help     Display this help menu\n"
                                    "exit     Exit the program\n"
                                    "count    Output the number of stops in the system\n"
                                    "config   Output the current configuration\n"
                                    "toggle   Syntax \"toggle flag\"\n"
                                    "         Will toggle the flag specified.\n"
                                    "set      Syntax \"set option value\"\n"
                                    "         Will set the option specified with the value.\n"
                                    "stop     Syntax \"stop [0, count)\"\n"
                                    "         Will output stop ID, node ID, and Lat/Lon for and description.\n"
                                    "leaveat  Syntax \"leaveat time start end\" \n"
                                    "         Calculates the best trip plan from start to end leaving at time.\n"
                                    "arriveby Syntax \"arriveby time start end\" \n"
                                    "         Calculates the best trip plan from start to end arriving by time.\n"
                                    "save     Saves the last calculated trip to file\n");
                }
                else if(Arguments[0] == DCountCommand){
                    auto stopCount = DTripPlanner->BusSystemIndexer()->StopCount();
                    auto routeCount = DTripPlanner->BusSystemIndexer()->RouteCount();
                    std::ostringstream oss;
                    oss <<stopCount<<" stops\n";
                    std::cout <<routeCount<<" routes\n";
                    OutputString(oss.str());
                }
                else if(Arguments[0] == DConfigCommand){
                    std::map<std::string, std::string> flagsMap;
                    std::map<std::string, std::string> optionsMap;

                    ExtractConfig(DOutWriter ? DOutWriter->Config() : nullptr, flagsMap, optionsMap);
                    ExtractConfig(DStorageWriter ? DStorageWriter->Config() : nullptr, flagsMap, optionsMap);

                    for (const auto& [k, v] : flagsMap)
                    {
                        std::ostringstream oss;
                        oss << std::left << std::setw(19) << k << ": " << v << "\n";
                        OutputString(oss.str());
                    }
                    for (const auto& [k, v] : optionsMap)
                    {
                        std::ostringstream oss;
                        oss << std::left << std::setw(19) << k << ": " << v << "\n";
                        OutputString(oss.str());
                    }
                }
                else if(Arguments[0] == DToggleCommand){
                    if(Arguments.size() < 2){
                        OutputError("Invalid toggle command, see help.\n");
                    }else{
                        std::string flag = Arguments[1];
                        bool found = false;
                        bool state = false;

                        ToggleConfigFlag(DOutWriter ? DOutWriter->Config() : nullptr, flag, found, state);
                        ToggleConfigFlag(DStorageWriter ? DStorageWriter->Config() : nullptr, flag, found, state);

                        if(found){
                            OutputString("Flag " + flag + " is now " + (state ? "true" : "false") + "\n");
                        }else{
                            OutputError("Invalid toggle parameter, see help.\n");
                        }
                    }
                }
                else if(Arguments[0] == DSetCommand){
                    if(Arguments.size() < 3){
                        OutputError("Invalid set command, see help.\n");
                    }else{
                        std::string option = Arguments[1];
                        std::string valueStr = Arguments[2];
                        for (size_t i = 3; i < Arguments.size(); i++)
                        {
                            valueStr += " " + Arguments[i];
                        }
                
                        bool found = false;
                        std::string printedVal;

                        try
                        {
                            TrySetOption(DOutWriter ? DOutWriter->Config() : nullptr, option, valueStr, found, printedVal);
                            TrySetOption(DStorageWriter ? DStorageWriter->Config() : nullptr, option, valueStr, found, printedVal);
                            
                            if(found){
                                OutputString("Option " + option + " is now " + printedVal + "\n");
                            }else{
                                OutputError("Invalid set parameter, see help.\n");
                            }
                        }
                        catch (const std::invalid_argument& e)
                        {
                            OutputError("Invalid set parameter, see help.\n");
                        }
                        catch (const std::out_of_range& e)
                        {
                            OutputError("Invalid set parameter, see help.\n");
                        }
                    }
                }
                else if(Arguments[0] == DStopCommand){
                    if(Arguments.size() < 2){
                        OutputError("Invalid stop command, see help.\n");
                    }else{
                        try
                        {
                            size_t idx = std::stoul(Arguments[1]);
                            auto bsIndexer = DTripPlanner->BusSystemIndexer();
                            if(idx < bsIndexer->StopCount()){
                                auto stop = bsIndexer->SortedStopByIndex(idx);
                                auto node = DStreetMap->NodeByID(stop->NodeID());

                                std::ostringstream oss;
                                oss << "Stop " << idx << ":\n"
                                    << "    ID          : " << stop->ID() << "\n"
                                    << "    Node ID     : " << stop->NodeID() << "\n"
                                    << "    Location    : " << FormatLatLong(node->Location().DLatitude, true) << ", " << FormatLatLong(node->Location().DLongitude, false) << "\n"
                                    << "    Description : " << stop->Description() << "\n";
                                OutputString(oss.str());
                            }else{
                                throw std::out_of_range("Invalid stop parameter, see help.\n");
                            }
                        }
                        catch(const std::invalid_argument& e)
                        {
                            OutputError("Invalid stop parameter, see help.\n");
                        }
                        catch(const std::out_of_range& e)
                        {
                            OutputError(e.what());
                        }
                    }
                }
                else if(Arguments[0] == DLeaveAtCommand || Arguments[0] == DArriveByCommand){
                    if(Arguments.size() < 4){
                        OutputError("Invalid " + Arguments[0] + " command, see help.\n");
                    }else{
                        try
                        {
                            std::string timeStr = Arguments[1];
                            std::string periodStr = "";
                            size_t argOffset = 2; 
                            
                            if(Arguments.size() > 4 && (Arguments[2] == "AM" || Arguments[2] == "PM")){
                                periodStr = Arguments[2];
                                argOffset = 3;
                            }
                            
                            if(Arguments.size() <= argOffset + 1){
                                OutputError("Invalid " + Arguments[0] + " command, see help.\n");
                            }else{
                                CBusSystem::TStopID startStop = std::stoul(Arguments[argOffset]);
                                CBusSystem::TStopID endStop = std::stoul(Arguments[argOffset + 1]);
                                
                                int hr = 0, min = 0, sec = 0;
                                char delimiter1 = 0, delimiter2 = 0;
                                std::stringstream ss(timeStr);
                                
                                ss >> hr >> delimiter1 >> min;
                                if(ss.fail() || delimiter1 != ':'){
                                    throw std::invalid_argument("Invalid time format");
                                }
                                if(ss.peek() == ':'){
                                    ss >> delimiter2 >> sec;
                                    if(ss.fail() || delimiter2 != ':'){
                                        throw std::invalid_argument("Invalid time format");
                                    }
                                }
                                
                                if(hr < 0 || hr > 23 || min < 0 || min > 59 || sec < 0 || sec > 59){
                                    throw std::invalid_argument("Invalid time bounds");
                                }
                                if(!periodStr.empty() && (hr < 1 || hr > 12)){
                                    throw std::invalid_argument("Invalid 12-hour time bounds");
                                }
                                
                                if(periodStr == "PM" && hr != 12){
                                    hr += 12;
                                }
                                if(periodStr == "AM" && hr == 12){
                                    hr = 0;
                                }
                                
                                auto bsIndexer = DTripPlanner->BusSystemIndexer();
                                bool startValid = false, endValid = false;
                                for (size_t i = 0; i < bsIndexer->StopCount(); ++i){
                                    auto stop = bsIndexer->SortedStopByIndex(i);
                                    if(stop->ID() == startStop){
                                        startValid = true;
                                    }
                                    if(stop->ID() == endStop){
                                        endValid = true;
                                    }
                                }
                                if(!startValid || !endValid){
                                    throw std::invalid_argument("Stop ID does not exist");
                                }
                                
                                CBusSystem::TStopTime timeQuery(std::chrono::hours(hr) + std::chrono::minutes(min) + std::chrono::seconds(sec));
                                bool success = false;
                                DRecentPlan.clear(); 
                                
                                if(Arguments[0] == DLeaveAtCommand){
                                    success = DTripPlanner->FindRouteLeaveTime(startStop, endStop, timeQuery, DRecentPlan);
                                }else{
                                    success = DTripPlanner->FindRouteArrivalTime(startStop, endStop, timeQuery, DRecentPlan);
                                }

                                if(success && !DRecentPlan.empty()){
                                    DLastWasLeaveAt = (Arguments[0] == DLeaveAtCommand);
                                    DLastTimeQuery = hr * 60 + min; 
                                    DLastStartStop = startStop;
                                    DLastEndStop = endStop;
                                    
                                    DOutWriter->WritePlan(DOutSink, DRecentPlan);
                                }else{
                                    OutputError("Unable to find route from " + std::to_string(startStop) + " to " + std::to_string(endStop) + ".\n");
                                }
                            }
                        }
                        catch(const std::invalid_argument& e)
                        {
                            OutputError("Invalid " + Arguments[0] + " parameter, see help.\n");
                        }
                        catch(const std::out_of_range& e)
                        {
                            OutputError("Invalid " + Arguments[0] + " parameter, see help.\n");
                        }
                    }
                }
                else if(Arguments[0] == DSaveCommand){
                    if(DRecentPlan.empty()){
                        OutputError("No valid trip to save, see help.\n");
                    }else{
                        std::string filename;
                        if(Arguments.size() >= 2){
                            filename = Arguments[1];
                        }else{
                            std::string prefix = DLastWasLeaveAt ? "la_" : "ab_";
                            filename = prefix + std::to_string(DLastTimeQuery) + "_" + 
                                    std::to_string(DLastStartStop) + "_" + 
                                    std::to_string(DLastEndStop) + ".html";
                        }
                        
                        auto storageSink = DResultsFactory->CreateSink(filename);
                        if(storageSink){
                            DStorageWriter->WritePlan(storageSink, DRecentPlan);
                        }
                        OutputString("Trip saved to <results>/" + filename + "\n");
                    }
                }
                else{
                    OutputError("Unknown command \"" + Arguments[0] + "\" type help for help.\n");
                }
            }
        }

        return true;
    }
};

CTripPlannerCommandLine::CTripPlannerCommandLine(std::shared_ptr<SConfig> config){
    DImplementation = std::make_unique<SImplementation>(config);
}

CTripPlannerCommandLine::~CTripPlannerCommandLine(){

}

bool CTripPlannerCommandLine::ProcessCommands(){
    return DImplementation->ProcessCommands();
}
