#include "XMLReader.h"
#include <expat.h>
#include <queue> 

/*
Implementation for CXMLReader

This struct wraps the Expat XML parser library, handling XML parsing and entity queueing
*/
struct CXMLReader::SImplementation{
    // Data source we will read XML from (could be anything like a file, string, etc.)
    std::shared_ptr <CDataSource> DSource;
    // Expat parser object (need for actual XML parsing)
    XML_Parser DParser;
    // Queue that stores entities that Expat has found but user hasnt read yet
    std::queue <SXMLEntity> DEntityQueue;
    // Flag to track if we are at the end of an XML document
    bool DEnd; 

    // Constructor (setting up Expat)
    SImplementation(std::shared_ptr<CDataSource> src) : DSource(src), DEnd(false) {
        DParser = XML_ParserCreate(NULL); // NULL = default encoding

        // We need user data for the Handlers, "this" pointer will apply into every single object
        XML_SetUserData(DParser, this); // "this" is userData and also a pointer to this struct (SImplemntation*)
    
        // SetElementHandler and SetCharacterDataHandler are necessary as prerequisuites for their functions later
        XML_SetElementHandler(DParser, XMLStartElementCallback, XMLEndElementCallback); 
        XML_SetCharacterDataHandler(DParser, XMLCharacterDataCallback);

    }

    // Deconstructor
    ~SImplementation() {
        if(DParser) { // need to clean parser and free it
            XML_ParserFree(DParser);
        }
    }
    std::string DCharBuffer; // NEW: Buffer to hold text chunks

    // Helper to flush the text buffer into the queue
    static void FlushCharBuffer(SImplementation *impl) {
        if (!impl->DCharBuffer.empty()) {
            SXMLEntity entity;
            entity.DType = SXMLEntity::EType::CharData;
            entity.DNameData = impl->DCharBuffer;
            impl->DEntityQueue.push(entity);
            impl->DCharBuffer.clear();
        }
    }

    static void XMLStartElementCallback(void *userData, const XML_Char *name, const XML_Char **attrs) {
        auto impl = static_cast<SImplementation*>(userData);
        FlushCharBuffer(impl); // Flush any text before opening a new tag

        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;
        for (int i = 0; attrs[i] != NULL; i += 2) {
            entity.DAttributes.push_back(std::make_pair(attrs[i], attrs[i + 1]));
        }
        impl->DEntityQueue.push(entity);
    }

    static void XMLEndElementCallback(void *userData, const XML_Char *name) {
        auto impl = static_cast<SImplementation*>(userData);
        FlushCharBuffer(impl); // Flush text before closing the tag

        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;
        impl->DEntityQueue.push(entity);
    }

    static void XMLCharacterDataCallback(void *userData, const XML_Char *s, int len) {
        auto impl = static_cast<SImplementation*>(userData);
        // ONLY APPEND! Do not push to the queue here.
        impl->DCharBuffer.append(s, len); 
    }


};

/*
Constructs a new CXMLReader object

Parameter:
src: a data source containing XML data to parse
*/
CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>(src); 
    
}

/*
Destroys the CXMLReader object
*/
CXMLReader::~CXMLReader(){
 
}

/*
Check if parsing is done

Returns true when both the data sources have no more data
and when all entities have been consumed from the queue

return true if parsing is finsihed, false otherwise
*/
bool CXMLReader::End() const{
// 1. must return when source has no more data
// 2. when all entities are consumed from queue
return DImplementation->DSource->End() && DImplementation->DEntityQueue.empty();
}

/*
Read the next XML entity from the parser

Reads from the data source and feeds data to Expat until an entity 
is avaliable in the queue. Also optionally skips character data entities with skipcdata

Parameters:
entity: Reference to store the read entity
skipcdata: If true, it will skip CharData entities and return on the opening and closing tags (element tags)

returns: true if an entity was successfully read, false if no more entities or error
*/
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    while (true) {
        while (DImplementation->DEntityQueue.empty() && !DImplementation->DEnd) {
            const int bufferSize = 512;
            std::vector<char> buffer;
            if (!DImplementation->DSource->Read(buffer, bufferSize)) {
                DImplementation->DEnd = true; 
                XML_Parse(DImplementation->DParser, "", 0, XML_TRUE);
                break; 
            }
            if (XML_Parse(DImplementation->DParser, buffer.data(), buffer.size(), XML_FALSE) == XML_STATUS_ERROR) {
                DImplementation->DEnd = true;
                return false; 
            }
        }
        if (DImplementation->DEntityQueue.empty()) {
            return false;
        }

        entity = DImplementation->DEntityQueue.front();
        DImplementation->DEntityQueue.pop();

        if (skipcdata && entity.DType == SXMLEntity::EType::CharData) {
            continue; 
        }

        return true;
    }
}
