# TextTripPlanWriter
- Outputs travel plan as plain text
- Takes TTravelPlan and writes to a CDataSInk as human-readable text
- Requires CBusSystem
- CTextTripPlanWriter inherits from CTripPlanWriter

**inline static constexpr std::string_view Verbose = "verbose";**
- Flag name used to toggle verbose output mode
- Enabling this will cause the writer to output all intermediate stops and the scheduled times along the route instead of just the minimal get-on and get-off summary

**CTextTripPlanWriter(std::shared_ptr<CBusSystem> bussystem)**
- Constructor
- Parameters:
    - bussystem: A shared ptr to the CBusSystem, used for stop descriptions and route names

**~CTextTripPlanWriter()**
- Destructor

**std::shared_ptr<SConfig> Config() const override**
- Returns current config object for this writer
- Returns shared ptr to the SConfig associated to this writer

**bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan) override**
- Writes travel plan to sink as a plain text string
- Outputs a minimal summary by default
    - Which route to board
    - Which stop to get off
    - When verbose enabled, it outputs each stop and time along the route
- Parameters
    - sink: shared ptr to CDataSink to write text output to
    - plan: TTravelPlan containing ordered sequence of STravelStep objects
- Returns true of plan is successfully written, false if failed

**struct SImplementation**
- Internal implementation

**std::unique_ptr<SImplementation> DImplementation**
- Pointer to internal implementation