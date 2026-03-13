# SVGTripPlanWriter
- Outputs travel plan as SVG map
- Takes TTravelPlan and writes into a CDataSink as SVG image
- Street Map is rendered as background, overlays the bus route being taken, and marks source and destiantion stops
- Requires CStreetMap and CBusSystem
    - For drawing streets, nodes, getting stop data, and getting route data

**Parent Class: CTripPlanWriter**
- This writer inherits from CTripPlanWriter

## Static Configuration Keys
- These are string constants used as keys when getting or setting values on SConfig objects returned by Config()
- These are supposed to be passed as key arguments to config read/write calls

**SVGWidth = "svg-width"**
- Set pixel width

**SVGHeight = "svg-height"**
- Set pixel height

**SVGMarginPixels = "svg-margin"**
- Sets pargin between SVG border and drawn map content

**SourceColor = "source-color"**
- Sets fill color of the circle drawn at the source (the starting stop)

**DestinationColor = "destination-color"**
- Sets fill color of circle drawn at destination map

**BusStopRadius = "busstop-radius"**
- Sets radius of circles at intermediate bus stops along the route

**SourceRadius = "source-radius"**
- Sets Radius of circle drawn at src stop

**DestinationRadius = "destination-radius"**
- Sets Radius of circle drawn at dest stop

**BusColor0 = "bus-color-0"**
- Sets primary color used to draw bus route line on map

**BusColor1 = "bus-color-1"**
- Sets secondary color used to draw bus route line

**StreetColor = "street-color"**
- Sets color used to draw streets

**MotorwayStroke = "motorway-stroke"**
- Sets stroke width for motorway roads

**PrimaryStroke = "primary-stroke"**
- Sets stroke width for primary roads

**SecondaryStroke = "secondary-stroke"**
- Sets stroke width for secondary roads

**TertiaryStroke = "tertiary-stroke"**
- Sets stroke width for tertiary roads

**ResidentialStroke = "residential-stroke"**
- Sets stroke width for residential roads

**BusStroke = "bus-stroke"**
- Sets stroke width for the bus route line drawn on the map

**LabelMargin = "label-margin"**
- Sets margin between stop circle and label text

**LabelColor = "label-color"**
- Sets text color of stop labels

**LabelBackground = "label-background"**
- Sets background fill color behind stop label text

**LabelSize = "label-size"**
- Sets font size for text

**LabelPaintOrder = "label-paint-order"**
- Sets SVG paint order property

**MotorwayEnabled = "motorway-enabled"**
- Toggles whether motorways are drawn or not

**PrimaryEnabled = "primary-enabled"**
- Toggles whether primary roads are drawn**

**SecondaryEnabled = "secondary-enabled"**
- Toggles whether secondary roads are drawn

**TertiaryEnabled = "tertiary-enabled"**
- Toggles whether tertiary roads are drawn

**ResidentialEnabled = "residential-enabled"**
- Toggles whether residential roads are drawn

**CSVGTripPlanWriter(std::shared_ptr<CStreetMap> streetmap, std::shared_ptr<CBusSystem> bussystem)**
- Constructor
- Parameters:
    - streetmap: shared ptr to the CStreetMap to draw streets and locate nodes geographically
    - bussystem: shared ptr to the CBusSystem to look up stop position and route paths

**~CSVGTripPlanWriter()**
- Destructor

**std::shared_ptr<SConfig> Config() const override**
- Return current config object for this writer
- Use static keys to read and modify options
- Returns shared ptr to the SConfig

**bool WritePlan(std::shared_ptr<CDataSink> sink, const TTravelPlan &plan) override**
- Writes travel plan as an SVG image
- Parameters:
    - sink: shared ptr to the CDataSink to write the SVGoutput to a something like a file or string sink
    - plan: TTravelPlan containing ordered sequences of STravelStep objects
- Returns true if plan is successful, false if failed

**struct SImplementation**
- Internal implementation

**std::unique_ptr<SImplementation> DImplementation**
- Pointer to internal implementation
