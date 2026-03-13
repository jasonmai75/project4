#include "HTMLTripPlanWriter.h"
#include "SVGTripPlanWriter.h"
#include "TextTripPlanWriter.h"
#include <iomanip>
#include <vector>
#include <string>
#include "StringDataSink.h"

struct CHTMLTripPlanWriter::SImplementation{
    struct SConfigSync : public CTripPlanWriter::SConfig {
        std::shared_ptr<CTripPlanWriter::SConfig> DSVGConfig;
        std::shared_ptr<CTripPlanWriter::SConfig> DTextConfig;

        SConfigSync(std::shared_ptr<CTripPlanWriter::SConfig> svg, 
                           std::shared_ptr<CTripPlanWriter::SConfig> text) 
            : DSVGConfig(svg), DTextConfig(text) {}

        void EnableFlag(std::string_view flag) override {
            if (DSVGConfig->ValidFlags().count(std::string(flag))) DSVGConfig->EnableFlag(flag);
            if (DTextConfig->ValidFlags().count(std::string(flag))) DTextConfig->EnableFlag(flag);
        }

        void DisableFlag(std::string_view flag) override {
            if (DSVGConfig->ValidFlags().count(std::string(flag))) DSVGConfig->DisableFlag(flag);
            if (DTextConfig->ValidFlags().count(std::string(flag))) DTextConfig->DisableFlag(flag);
        }

        bool FlagEnabled(std::string_view flag) const override {
            if (DSVGConfig->ValidFlags().count(std::string(flag))) return DSVGConfig->FlagEnabled(flag);
            if (DTextConfig->ValidFlags().count(std::string(flag))) return DTextConfig->FlagEnabled(flag);
            return false;
        }

        std::any GetOption(std::string_view option) const override {
            // Priority given to SVG options
            if (DSVGConfig->ValidOptions().count(std::string(option))) return DSVGConfig->GetOption(option);
            return DTextConfig->GetOption(option);
        }

        std::unordered_set<std::string> ValidFlags() const override {
            auto flags = DSVGConfig->ValidFlags();
            auto textFlags = DTextConfig->ValidFlags();
            flags.insert(textFlags.begin(), textFlags.end());
            return flags;
        }

        EOptionType GetOptionType(std::string_view option) const override {
            if (DSVGConfig->ValidOptions().count(std::string(option))) return DSVGConfig->GetOptionType(option);
            return DTextConfig->GetOptionType(option);
        }

        void SetOption(std::string_view option, int value) override {
            if (DSVGConfig->ValidOptions().count(std::string(option))) DSVGConfig->SetOption(option, value);
            if (DTextConfig->ValidOptions().count(std::string(option))) DTextConfig->SetOption(option, value);
        }

        void SetOption(std::string_view option, double value) override {
            if (DSVGConfig->ValidOptions().count(std::string(option))) DSVGConfig->SetOption(option, value);
            if (DTextConfig->ValidOptions().count(std::string(option))) DTextConfig->SetOption(option, value);
        }

        void SetOption(std::string_view option, const std::string &value) override {
            if (DSVGConfig->ValidOptions().count(std::string(option))) DSVGConfig->SetOption(option, value);
            if (DTextConfig->ValidOptions().count(std::string(option))) DTextConfig->SetOption(option, value);
        }

        void ClearOption(std::string_view option) override {
            DSVGConfig->ClearOption(option);
            DTextConfig->ClearOption(option);
        }

        std::unordered_set<std::string> ValidOptions() const override {
            auto opts = DSVGConfig->ValidOptions();
            auto textOpts = DTextConfig->ValidOptions();
            opts.insert(textOpts.begin(), textOpts.end());
            return opts;
        }
    };
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<CSVGTripPlanWriter> DSVGTripPlanWriter;
    std::shared_ptr<CTextTripPlanWriter> DTextTripPlanWriter;
    std::shared_ptr<SConfigSync> DSConfigSync;
    SImplementation(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
        DSVGTripPlanWriter = std::make_shared<CSVGTripPlanWriter>(streetmap, bussystem);
        DTextTripPlanWriter = std::make_shared<CTextTripPlanWriter>(bussystem);
        DSConfigSync = std::shared_ptr<SConfigSync>(new SConfigSync(DSVGTripPlanWriter->Config(), DTextTripPlanWriter->Config()));
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DSConfigSync;
    }

    bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
        if(!sink || plan.empty()) {
            return false;
        }

        auto svgSink = std::make_shared<CStringDataSink>();
        if(!DSVGTripPlanWriter->WritePlan(svgSink, plan)){
            return false;
        }
        std::string svgPlan = svgSink->String();

        auto textSink = std::make_shared<CStringDataSink>();
        if(!DTextTripPlanWriter->WritePlan(textSink, plan)){
            return false;
        }
        std::string textPlan = textSink->String();

        std::stringstream htmlOut;
        htmlOut<<"<!DOCTYPE html>\n";
        htmlOut<<"<html>\n";
        htmlOut<<"    <head>\n";
        htmlOut<<"        <meta charset=\"UTF-8\">\n";
        htmlOut<<"        <title>Trip Plan</title>\n";
        htmlOut<<"        <style>\n";
        htmlOut<<"            .container {\n";
        htmlOut<<"                display: flex;\n";
        htmlOut<<"                flex-direction: column;\n";
        htmlOut<<"                align-items: center;\n";
        htmlOut<<"                gap: 2rem;\n";
        htmlOut<<"            }\n\n";
        htmlOut<<"            .text {\n";
        htmlOut<<"                width: 100%;\n";
        htmlOut<<"            }\n\n";
        htmlOut<<"            .graphic {\n";
        htmlOut<<"                width: 100%;\n";
        htmlOut<<"            }\n\n";
        htmlOut<<"            svg {\n";
        htmlOut<<"                max-width: 100%;\n";
        htmlOut<<"                height: auto;\n";
        htmlOut<<"            }\n";
        htmlOut<<"            .stop circle {\n";
        htmlOut<<"              pointer-events: all;\n";
        htmlOut<<"            }\n";
        htmlOut<<"            \n";
        htmlOut<<"            .stop text {\n";
        htmlOut<<"              opacity: 0;\n";
        htmlOut<<"              transition: opacity 0.2s ease;\n";
        htmlOut<<"            }\n";
        htmlOut<<"            \n";
        htmlOut<<"            .stop:hover text {\n";
        htmlOut<<"              opacity: 1;\n";
        htmlOut<<"            }\n";
        htmlOut<<"            .schedule {\n";
        htmlOut<<"                display: grid;\n";
        htmlOut<<"                grid-template-columns: 80px 10px auto;\n";
        htmlOut<<"                row-gap: 0.4rem;\n";
        htmlOut<<"                font-family: monospace;\n";
        htmlOut<<"            }\n\n";
        htmlOut<<"            .time {\n";
        htmlOut<<"                text-align: right;\n";
        htmlOut<<"                padding-right: 5px;\n";
        htmlOut<<"            }\n";
        htmlOut<<"        </style>\n";
        htmlOut<<"    </head>\n";
        htmlOut<<"    <body>\n";
        htmlOut<<"        <div class=\"container\">\n";
        htmlOut<<"            <div class=\"graphic\">\n";
        htmlOut<<"                "<<svgPlan<<"\n";
        htmlOut<<"            </div>\n";
        htmlOut<<"            <div class=\"text\">\n";
        htmlOut<<"                <h2>Trip Steps</h2>\n";
        htmlOut<<"                <div class=\"schedule\">\n";

        std::stringstream textSS(textPlan);
        std::string line;
        while (std::getline(textSS, line))
        {
            if(line.empty()){
                continue;
            }
            size_t delimiterPos = line.find(": ");
            if(delimiterPos != std::string::npos){
                std::string timeStr = line.substr(0, delimiterPos);
                std::string instructionStr = line.substr(delimiterPos + 2);
                
                size_t firstNonSpace = timeStr.find_first_not_of(" ");
                if(firstNonSpace == std::string::npos){
                    timeStr = ""; 
                }else{
                    size_t lastNonSpace = timeStr.find_last_not_of(" ");
                    timeStr = timeStr.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
                }

                firstNonSpace = instructionStr.find_first_not_of(" ");
                if(firstNonSpace != std::string::npos){
                    instructionStr = instructionStr.substr(firstNonSpace);
                }

                size_t ampersandPos = 0;
                std::string ampersandHTMLCode = "&amp;";
                while ((ampersandPos = instructionStr.find("&",ampersandPos)) != std::string::npos)
                {
                    instructionStr.replace(ampersandPos, 1, ampersandHTMLCode);
                    ampersandPos += ampersandHTMLCode.size();
                }
                htmlOut<<"                    <div class=\"time\">"<<timeStr<<"</div><div>:</div><div>"<<instructionStr<<"</div>\n";
            }
        }
        htmlOut<<"                </div>\n";
        htmlOut<<"            </div>\n";
        htmlOut<<"        </div>\n";
        htmlOut<<"    </body>\n";
        htmlOut<<"</html>\n";
        std::string htmlStr = htmlOut.str();
        std::vector<char> outData(htmlStr.begin(),htmlStr.end());
        return sink->Write(outData);
    }
};



CHTMLTripPlanWriter::CHTMLTripPlanWriter(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
    DImplementation = std::make_unique<SImplementation>(streetmap,bussystem);
}

CHTMLTripPlanWriter::~CHTMLTripPlanWriter(){

}

std::shared_ptr<CTripPlanWriter::SConfig> CHTMLTripPlanWriter::Config() const{
    return DImplementation->Config();
}

bool CHTMLTripPlanWriter::WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan){
    return DImplementation->WritePlan(sink,plan);
}

