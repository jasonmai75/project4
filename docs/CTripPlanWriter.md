# TripPlanWriter
- Base class for all trip plan writers
- Defines interface for all trip plan writer implementations
- The three implementations are CTextTripPlanWriter, CSVGTripPlanWriter, and CHTMLTripPlanWriter 

**using TTravelPlan = CTripPlanner::TTravelPlan**
- Brings CTriPlanner's TTravelPlan into scope

**SConfig Struct**
- Allows reading and modification of flags and options that control how plans are written

**enum class EOptionType { None, Int, Double, String }**
- Describes value type of configuration option
- None: option has no value set or isn't recognized
- Int: option holds an integer value
- Double: option holds a double value
- String: option holds a string value

**virtual bool FlagEnabled(std::string_view flag) const = 0**
- Returns true if named flag is enabled, false otherwise
- Parameters:
    - flag: name of flag to check

**virtual void EnableFlag(std::string_view flag) = 0**
- Enables named flag
- Parameters:
    - flag: name of flag to enable

**virtual void DisableFlag(std::string_view flag) = 0**
- Disables named flag
- Parameters:
    - flag: name of flag to disable

**virtual std::any GetOption(std::string_view option) const = 0**
- Returns current value of named option as std::any
- Parameters:
    - option: name of option to get
- Returns std::any holding the value or an empty std::any if option is not set

**virtual std::unordered_setstd::string ValidFlags() const = 0**
- Returns set of all flag names recognized by the writer

**virtual EOptionType GetOptionType(std::string_view option) const = 0**
- Returns type of named option, so caller can know how to interpret the value from GetOption()
- Parameters:
    - option: name of option
- Returns either EOptionType or EOptionType::None if not recognized

**virtual void SetOption(std::string_view option, int value) = 0**
- Sets option to an integer value
- Parameters:
    - option: name of option
    - value: integer value to set

**virtual void SetOption(std::string_view option, double value) = 0**
- Sets option to double value
- Parameters:
    - option: name of option
    - double value to set

**virtual void SetOption(std::string_view option, const std::string &value) = 0**
- Sets option to string
- Parameters:
    - option: name of option
    - value: string value to assign

**virtual void ClearOption(std::string_view option) = 0**
- Removes any set value for the option, resetting to unset
- Parameters:
    - option: name of option

**virtual std::unordered_setstd::string ValidOptions() const = 0**
- Returns set of all option names recognized by writer

**virtual ~CTripPlanWriter()**
- Destructor

**virtual std::shared_ptr<SConfig> Config() const = 0**
- Returns configuration object for this writer
- Returns shared ptr to the SConfig for this writer

**virtual bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan) = 0**
- Writes travel plan to the sink in whatever format that the writer implements
- Parameters:
    - sink: shared ptr to the CDataSink to write output to
    - plan: TTravelPlan containing ordered sequence of STravelStep objects
- Return true if plan was written, false if writing failed

