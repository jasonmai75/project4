# SVGTripPlanWriter
- Outputs travel plan as SVG map
- Takes TTravelPlan and writes into a CDataSink as SVG image
- Street Map is rendered as background, overlays the bus route being taken, and marks source and destiantion stops
- Requires CStreetMap and CBusSystem
    - For drawing streets, nodes, getting stop data, and getting route data
