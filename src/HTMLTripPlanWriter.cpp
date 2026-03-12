#include "HTMLTripPlanWriter.h"
#include "SVGTripPlanWriter.h"
#include "TextTripPlanWriter.h"
#include <iomanip>
#include <vector>
#include <string>
#include "StringDataSink.h"

struct CHTMLTripPlanWriter::SImplementation{
    std::shared_ptr<CStreetMap> DStreetMap;
    std::shared_ptr<CBusSystem> DBusSystem;
    std::shared_ptr<CSVGTripPlanWriter> DSVGTripPlanWriter;
    std::shared_ptr<CTextTripPlanWriter> DTextTripPlanWriter;
    SImplementation(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem){
        DStreetMap = streetmap;
        DBusSystem = bussystem;
        DSVGTripPlanWriter = std::make_shared<CSVGTripPlanWriter>(streetmap, bussystem);
        DTextTripPlanWriter = std::make_shared<CTextTripPlanWriter>(bussystem);
    }
    
    ~SImplementation(){

    }

    std::shared_ptr<SConfig> Config() const{
        return DSVGTripPlanWriter->Config();
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
        htmlOut<<"<html lang=\"en\">\n";
        htmlOut<<"<head><title>Trip Plan</title></head>\n";
        htmlOut<<"<body>\n";
        htmlOut<<svgPlan<<"\n";

        std::stringstream textSS(textPlan);
        std::string line;
        while (std::getline(textSS, line))
        {
            if(line.empty()){
                continue;
            }
            size_t delimiterPos = line.find(":");
            if(delimiterPos != std::string::npos){
                std::string timeStr = line.substr(0, delimiterPos);
                std::string instructionStr = line.substr(delimiterPos + 1);
                size_t firstNonSpace = timeStr.find_first_not_of(" ");
                if (firstNonSpace == std::string::npos) {
                    timeStr = "";
                } else {
                    size_t lastNonSpace = timeStr.find_last_not_of(" ");
                    timeStr = timeStr.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
                }

                firstNonSpace = instructionStr.find_first_not_of(" ");
                if (firstNonSpace != std::string::npos) {
                    instructionStr = instructionStr.substr(firstNonSpace);
                }

                size_t ampersandPos = 0;
                std::string ampersandHTMLCode = "&amp;";
                while ((ampersandPos = instructionStr.find("&",ampersandPos)) != std::string::npos)
                {
                    instructionStr.replace(ampersandPos, 1, ampersandHTMLCode);
                    ampersandPos += ampersandHTMLCode.size();
                }
                htmlOut<<"<div class=\"time\">"<<timeStr<<"</div>\n";
                htmlOut<<"<div>"<<instructionStr<<"</div>\n";
            }
        }
        htmlOut<<"</body>\n</html>\n";
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

