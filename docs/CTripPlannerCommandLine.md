# CTripPlannerCommandLine
- Implements the command line for the tripplanner program, so that the program is interactive with the command line
- Reads commands from a data source, uses CTripPlanner and writers to output results to sinks

**std::shared_ptr<CDataSource> DCommandSource**
- Source where commands are read from (e.g. standard input or a file)
- Lines read from this source are interpretted as commands

**std::shared_ptr<CDataSink> DOutSink**
- Sink to write normal output (e.g. standard output or file)
- Command results are written here

**std::shared_ptr<CDataSink> DErrorSink**
- Sink to write error messages to
- For unrecognized commands or invalid arguments

**std::shared_ptr<CDataFactory> DResultsFactory**
- Creates sinks for saving trip results when save command is done

**std::shared_ptr<CTripPlanner> DTripPlanner**
- CTripPlanner instance used to compute trip plans when leaveat or arriveby commands are done

**std::shared_ptr<CStreetMap> DStreetMap**
- This is the CStreetMap providing geographic data
- Needed when rendering SVG or HTML output of a trip plan

**std::shared_ptr<CTripPlanWriter> DOutWriter**
- The writer used to format and save the trip plan when save command is done

**CTripPlannerCommandLine(std::shared_ptr<SConfig> config)**
- Constructor
- Parameters:
    - config: shared ptr to a filled SCOnfig struct containing the data sources, sinks, trip planner, street map, and writers

**~CTripPlannerCommandLine()**
- Destructor

**bool ProcessCommands()**
- Reads and processes commands until exit command is done
- Commands:
    - help: Outputs help menu with all commands
    - exit: Terminates processing commands
    - count: Outputs total number of stops in the bus system
    - config: Outputs current writer config settings
    - toggle flag: Toggles the named flag
    - set option value: Sets option to given value
    - stop index: Outputs stopID, nodeID, latitude/longitude, and description for the stop at the index
    - leaveat time start end: Calculates and outputs best trip plan from start to end leaving at time
    - arriveby time start end: Calculates and outputs best trip plan fro mstart to end arriving by time
    - save: Saves most recent trip plan to a file using DStorageWriter and DResultsFactory

- Returns true if all commands are successful, false if error occurs during processing

**struct SImplementation**
- Internal implementation

**std::unique_ptr<SImplementation> DImplementation**
- Pointer to internal implementation