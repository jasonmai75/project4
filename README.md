# project4
Student 1: Jason Mai (924632295)

Student 2: Truong Le (924636822)

## Code Referencess
- [How Node Works in OSM](https://wiki.openstreetmap.org/wiki/Node)
- [How Way Works in OSM](https://wiki.openstreetmap.org/wiki/Node)
- [The relationship between the tags](https://piazza.com/class/mk1ahplhek42hi/post/94)
- [std::sort to create sorted Nodes and Ways] (https://en.cppreference.com/w/cpp/algorithm/sort.html)
- [Data Structure commands and review, specifcally for vectors/maps] (https://www.w3schools.com/cpp/cpp_data_structures.asp)
- [How to do padding in commandline in C++](https://stackoverflow.com/questions/667183/padding-stl-strings-in-c)
- [How to use std::chronos](https://en.cppreference.com/w/cpp/chrono/duration.html)
- [Translating Latitude and Longitude to SVG Coordinates](https://jagmit.co.uk/blog/implementing-dynamic-map-longitude-latitude/)
- [Dijkstra's Shortest Path Algorithm](https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-using-priority_queue-stl/)
- [Calculate distance, bearing between Latitude/Longitude points](https://www.movable-type.co.uk/scripts/latlong.html)
- [HTML basics](https://www.w3schools.com/html/html_basic.asp)
- [How to use HTML classes](https://www.w3schools.com/html/html_classes.asp )
- [How to avoid zig zag in find path algorithm](https://stackoverflow.com/questions/26302126/efficient-path-finding-algorithm-avoiding-zigzags)
- [How to use std::numeric_limits<type> and the <limits> library] https://en.cppreference.com/w/cpp/types/numeric_limits.html

Basic git Commands
git pull --no-rebase origin main

git push origin main

And if you end up in a VIM text editor, just do esc + :wq into the text and you will exit

### What is this project overall
- We are building a bus trip planner, given a city's street map and bus schedule, a user can be like "I'm at stop A and I wanna go to stop B, I'm leaving at 3pm" and our program is supposed to figure out which bus to take

### The Pieces we need and why:
- **CStreetMap** — Raw data. Just holds every node (a point on the map like an intersection) and every way (a road connecting nodes). Think of it as the raw GPS map data.
- **CBusSystem** — Raw data. Holds every bus stop, every route, and the schedule. Think of it as the raw bus schedule data.
- **CStreetMapIndexer** — organizes the street map so it's easy to search. The raw map has no order, so this sorts everything and lets you ask questions like "which roads are in this area?" or "which roads pass through this intersection?"
- **CBusSystemIndexer** — same idea but for the bus system. Lets you ask "which routes stop at stop 42?" or "which routes go from stop A to stop B?"

### Trip Planner:
-  You have a bus system.
-  The bus system has routes.
- Each route visits stops in a fixed order.
- Each route runs multiple times a day.

- You have two stops: src and dest.
- You have a time constraint.

- Job is to find the best trip.

**The Plan:** 
Search every route and every trip that connects your two stops, skip ones that violate your time constraint, return the one that optimizes your goal.

### The Overall Flow: Chronological Progression
1. StreetMap/Bus System Data (data that goes into the program)
2. Indexers (organizes the data)
3. TripPlanner (uses indexers to find best route)
4. Writers (output the result as text/SVG/HTML)
5. CommandLine (ties it all together for the user)

# Generative AI 
## Prompts 1-8 are all by Claude AI
### All those prompts have been fed the context of:
- "I am a second year CS student working on a C++  project that involves building a bus trip planner.The system has a CBusSystem that stores routes and stops, where each route has an ordered list of stops and a schedule of trips. Each trip represents one run of the route per day. I need to implement a search function that finds the best trip between two stops given either a departure constraint or an arrival constraint. I already understand C++ fundamentals, OOP, shared_ptr, pimpl, and the structure of the bus data from building it earlier. I don't need syntax help, only help understanding the logic and algorithm design decisions. Keep responses concise and use simple diagrams where helpful."


## Prompt 1: 
Debug this Makefile I created, it's mostly complete, I just need debugging and syntax and error fixing, also give me a full list of what was wrong after

Define the tools
AR=ar
CC=gcc
CXX=g++

Define the directories
INC_DIR				= ./include
SRC_DIR				= ./src
TESTSRC_DIR			= ./testsrc
BIN_DIR				= ./bin
OBJ_DIR				= ./obj
LIB_DIR				= ./lib
TESTOBJ_DIR			= ./testobj
TESTBIN_DIR			= ./testbin
TESTCOVER_DIR		= ./htmlconv
TESTTMP_DIR			= ./testtmp

Define the flags for compilation/linking
PKGS				= expat
DEFINES				= 
INCLUDE				= -I $(INC_DIR) `pkg-config --cflags $(PKGS)`
ARFLAGS				= rcs
CFLAGS				= -Wall
CPPFLAGS			= --std=c++20
LDFLAGS				= `pkg-config --libs $(PKGS)`

TEST_CFLAGS			= $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS		= $(CPPFLAGS) -fno-inline
TEST_LDFLAGS		= $(LDFLAGS) -lgtest -lgmock -lgtest_main -lpthread

Define the object files
TEST_SVG_OBJOBJ	= $(TESTOBJ_DIR)/SVGTest.o
TEST_OBJ_FILES		= $(TEST_SVG_OBJ) $(TEST_SVG_TEST_OBJ)


TEST_STRSINK_OBJ		= $(TESTOBJ_DIR)/StringDataSink.o
TEST_STRSINK_TEST_OBJ	= $(TESTOBJ_DIR)/StringDataSinkTest.o
TEST_STRSINK_OBJ_FILES	= $(TEST_STRSINK_OBJ) $(TEST_STRSINK_TEST_OBJ)

TEST_STRSRC_OBJ			= $(TESTOBJ_DIR)/StringDataSource.o
TEST_STRSRC_TEST_OBJ 	= $(TESTOBJ_DIR)/StringDataSourceTest.o
TEST_STRSRC_OBJ_FILES	= $(TEST_STRSRC_OBJ) $(TEST_STRSRC_TEST_OBJ)

TEST_SVGWRITER_OBJ		= $(TESTOBJ_DIR)/SVGWriter.o
TEST_SVGWRITER_TEST_OBJ	= $(TESTOBJ_DIR)/SVGWriterTest.o
TEST_SVGWRITER_OBJ_FILES= $(TEST_SVGWRITER_OBJ) $(TEST_SVGWRITER_TEST_OBJ) $(TEST_STRSINK_OBJ) $(STATIC_LIB)

TEST_XMLREADER_OBJ		= $(TESTOBJ_DIR)/XMLReader.o
TEST_XMLREADER_TEST_OBJ	= $(TESTOBJ_DIR)/XMLTest.o
TEST_XMLREADER_OBJ_FILES= $(TEST_XMLREADER_OBJ) $(TEST_XMLREADER_TEST_OBJ) $(TEST_STRSRC_OBJ) $(STATIC_LIB)

TEST_XMLBS_OBJ			= $(TESTOBJ_DIR)/XMLBusSystem.o
TEST_XMLBS_TEST_OBJ		= $(TESTOBJ_DIR)/XMLBusSystemTest.o
TEST_XMLBS_OBJ_FILES	= $(TEST_STRSRC_OBJ) $(TEST_XMLREADER_OBJ) $(TEST_XMLBS_OBJ) $(TEST_XMLBS_TEST_OBJ)

TEST_OSM_OBJ			= $(TESTOBJ_DIR)/OpenStreetMap.o
TEST_OSM_TEST_OBJ		= $(TESTOBJ_DIR)/OpenStreetMapTest.o
TEST_OSM_OBJ_FILES		= $(TEST_STRSRC_OBJ) $(TEST_XMLREADER_OBJ) $(TEST_OSM_OBJ) $(TEST_OSM_TEST_OBJ)

TEST_MOCK_BS_OBJ		= $(TESTOBJ_DIR)/MockBusSystem.o
TEST_MOCK_SM_OBJ		= $(TESTOBJ_DIR)/MockStreetMap.o

TEST_BSIDX_OBJ			= $(TESTOBJ_DIR)/BusSystemIndexer.o
TEST_BSIDX_TEST_OBJ		= $(TESTOBJ_DIR)/BusSystemIndexerTest.o
TEST_BSIDXL_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
							$(TEST_MOCK_BS_OBJ) \
							$(TEST_BSIDX_OBJ) \
							$(TEST_BSIDX_TEST_OBJ)

Adding StreetMapIndexer object files and test
TEST_SMIDX_OBJ			= $(TESTOBJ_DIR)/StreetMapIndexer.o
TEST_SMIDX_TEST_OBJ		= $(TESTOBJ_DIR)/StreetMapIndexerTest.o
TEST_SMIDX_OBJ_FILES	= $(TEST_STRSRC_OBJ) $(TEST_MOCK_SM_OBJ) $(TEST_SMIDX_OBJ) $(TEST_SMIDX_TEST_OBJ)

TEST_HTMLTPW_OBJ		= $(TESTOBJ_DIR)/HTMLTripPlanWriter.o
TEST_SVGLTPW_OBJ			= $(TESTOBJ_DIR)/SVGTripPlanWriter.o
TEST_TEXTTPW_OBJ		= $(TESTOBJ_DIR)/TextTripPlanWriter.o

TEST_TP_OBJ				= $(TESTOBJ_DIR)/TripPlanner.o
TEST_TPCL_OBJ			= $(TESTOBJ_DIR)/TripPlannerCommandLine.o
TEST_TPCL_TEST_OBJ		= $(TESTOBJ_DIR)/TripPlannerCommandLineTest.o

TEST_TP_TEST_OBJ		= $(TESTOBJ_DIR)/TripPlannerTest.o
TEST_TP_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
							$(TEST_MOCK_BS_OBJ) \
							$(TEST_BSIDX_OBJ) \
							$(TEST_TP_OBJ) \
							$(TEST_TP_TEST_OBJ) \

TEST_TEXTTPW_TEST_OBJ		= $(TESTOBJ_DIR)/TextTripPlanWriterTest.o
TEST_TEXTTPW_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
								$(TEST_STRSINK_OBJ) \
								$(TEST_MOCK_BS_OBJ) \
								$(TEST_TEXTTPW_OBJ) \
								$(TEST_TEXTTPW_TEST_OBJ) \

TEST_SVGTPW_TEST_OBJ		= $(TESTOBJ_DIR)/SVGTripPlanWriterTest.o
TEST_SVGLTPW_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
								$(TEST_STRSINK_OBJ) \
								$(TEST_MOCK_BS_OBJ) \
								$(TEST_MOCK_SM_OBJ) \
								$(TEST_SVGLTPW_OBJ) \
								$(TEST_SVGLTPW_TEST_OBJ) \
								$(TEST_SVGTPW_TEST_OBJ) \
								$(STATIC_LIB)

TEST_HTMLTPW_TEST_OBJ		= $(TESTOBJ_DIR)/HTMLTripPlanWriterTest.o
TEST_HTMLTPW_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
								$(TEST_STRSINK_OBJ) \
								$(TEST_MOCK_BS_OBJ) \
								$(TEST_MOCK_SM_OBJ) \
								$(TEST_SVGLTPW_OBJ) \
								$(TEST_TEXTTPW_OBJ) \
								$(TEST_HTMLTPW_OBJ) \
								$(TEST_HTMLTPW_TEST_OBJ) \ 
								$(STATIC_LIB)

TEST_TPCL_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
							$(TEST_STRSINK_OBJ) \
							$(TEST_MOCK_BS_OBJ) \
							$(TEST_MOCK_SM_OBJ) \
							$(TEST_BSIDX_OBJ) \
							$(TEST_HTMLTPW_OBJ)	\
							$(TEST_SVGLTPW_OBJ)	\
							$(TEST_TEXTTPW_OBJ)	\
							$(TEST_TP_OBJ)	\
							$(TEST_TPCL_OBJ) \
							$(TEST_TPCL_TEST_OBJ)

TEST_FILEDATASS_OBJ		= $(TESTOBJ_DIR)/FileDataFactory.o \
						$(TESTOBJ_DIR)/FileDataSource.o \
						$(TESTOBJ_DIR)/FileDataSink.o

TEST_FILEDATASS_TEST_OBJ		= $(TESTOBJ_DIR)/FileDataSSTest.o
TEST_FILEDATASS_OBJ_FILES		= $(TEST_FILEDATASS_OBJ) $(TEST_FILEDATASS_TEST_OBJ)

TEST_GEOUTILS_OBJ				= $(TESTOBJ_DIR)/GeographicUtils.o
TEST_GEOUTILS_TEST_OBJ			= $(TESTOBJ_DIR)/GeographicUtilsTest.o
TEST_GEOUTILS_OBJ_FILES			= $(TEST_GEOUTILS_OBJ) $(TEST_GEOUTILS_TEST_OBJ)

Define the targets
TEST_TARGET			= $(TESTBIN_DIR)/testsvg

PROD_SVG_OBJ = $(OBJ_DIR)/svg.o # this is like the actual build, instead of being a test

STATIC_LIB = $(LIB_DIR)/libsvg.a # static library

MAIN_TARGET = $(BIN_DIR)/main # main

TEST_SVG_TARGET = $(TESTBIN_DIR)/testsvg

TEST_STRSINK_TARGET = $(TESTBIN_DIR)/teststrdatasink

TEST_STRSRC_TARGET = $(TESTBIN_DIR)/teststrdatasource

TEST_SVGWRITER_TARGET = $(TESTBIN_DIR)/testsvgwritertest

TEST_XMLREADER_TARGET = $(TESTBIN_DIR)/testxml

TEST_XMLBS_TARGET = $(TESTBIN_DIR)/testxmlbs

TEST_OSM_TARGET	= $(TESTBIN_DIR)/testosm

TEST_BSIDX_TARGET = $(TESTBIN_DIR)/testbussystemindexer

TEST_SMIDX_TARGET = $(TESTBIN_DIR)/teststreetmapindexer

TEST_TP_TARGET = $(TESTBIN_DIR)/testtripplanner

TEST_TEXTTPW_TARGET = $(TESTBIN_DIR)/testtexttripplanwriter

TEST_SVGTPW_TARGET = $(TESTBIN_DIR)/testsvgtripplanwriter

TEST_HTMLTPW_TARGET = $(TESTBIN_DIR)/testhtmltripplanwriter

TEST_TPCL_TARGET =  $(TESTBIN_DIR)/testtripplannercommandline

TEST_FILEDATASS_TARGET = $(TESTBIN_DIR)/testfiledatass

TEST_GEOUTILS_TARGET = $(TESTBIN_DIR)/testgeographicutils

TRIPPLANNER_TARGET	= $(BIN_DIR)/tripplanner



All these get ran
all: directories \
	make_svglib \
	runmain \
	xmldiff \
	run_svgtest \
	run_sinktest \
	run_srctest \
	run_svgwritertest \
	run_bsidxtest		\
	run_tpcltest		\
	run_xmltest \
	run_xmlbstest \
	run_osmtest \
	run_filedatasstest \
	run_geoutilstest \
	run_smidxtest \
	run_tptest \
	run_texttpwtest \
	run_svgtpwtest \
	run_htmltpwtest \
	gen_html

run_svgtest: $(TEST_SVG_TARGET)
	$(TEST_SVG_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

make_svglib: $(STATIC_LIB)

run_sinktest: $(TEST_STRSINK_TARGET)
	$(TEST_STRSINK_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_srctest: $(TEST_STRSRC_TARGET)
	$(TEST_STRSRC_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_svgwritertest: $(TEST_SVGWRITER_TARGET)
	$(TEST_SVGWRITER_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_tpcltest: $(TEST_TPCL_TARGET)
	$(TEST_TPCL_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_bsidxtest: $(TEST_BSIDX_TARGET)
	$(TEST_BSIDX_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_xmltest: $(TEST_XMLREADER_TARGET)
	$(TEST_XMLREADER_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_xmlbstest: $(TEST_XMLBS_TARGET)
	$(TEST_XMLBS_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_osmtest: $(TEST_OSM_TARGET)
	$(TEST_OSM_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

Run targets for all tests
run_filedatasstest: $(TEST_FILEDATASS_TARGET)
	$(TEST_FILEDATASS_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

run_geoutilstest: $(TEST_GEOUTILS_TARGET)
	$(TEST_GEOUTILS_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

run_smidxtest: $(TEST_SMIDX_TARGET)
	$(TEST_SMIDX_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

run_tptest: $(TEST_TP_TARGET)
	$(TEST_TP_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

run_texttpwtest: $(TEST_TEXTTPW_TARGET)
	$(TEST_TEXTTPW_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

run_svgtpwtest: $(TEST_SVGTPW_TARGET)
	$(TEST_SVGTPW_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

run_htmltpwtest: $(TEST_HTMLTPW_TARGET)
	$(TEST_HTMLTPW_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

run_tpcltest: $(TEST_TPCL_TARGET)
	$(TEST_TPCL_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@ 

gen_html:
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source
	lcov --remove $(TESTCOVER_DIR)/coverage.info '*.h' '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR)

$(PROD_SVG_OBJ): $(SRC_DIR)/svg.c # This writes svg.0 into the object file by reading svg.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE) -c $(SRC_DIR)/svg.c -o $(PROD_SVG_OBJ)

$(STATIC_LIB): $(PROD_SVG_OBJ) # static library
	$(AR) $(ARFLAGS) $(STATIC_LIB) $(PROD_SVG_OBJ)

$(MAIN_TARGET): $(SRC_DIR)/main.c $(STATIC_LIB) # compile and linking the main.c with the static library created
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE) $(SRC_DIR)/main.c $(STATIC_LIB) -o $(MAIN_TARGET)

runmain: $(MAIN_TARGET) # run command for main that will be run by Makefile by all:
	$(MAIN_TARGET)

comparing generated SVG with expected SVG
xmldiff: runmain
	xmldiff expected_checkmark.svg checkmark.svg && echo "SVG files are identical" || echo "SVG files are NOT identical"

$(TEST_SVG_TARGET): $(TEST_OBJ_FILES) 
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_SVG_TARGET)

$(TEST_STRSINK_TARGET): $(TEST_STRSINK_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STRSINK_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STRSINK_TARGET)

$(TEST_STRSRC_TARGET): $(TEST_STRSRC_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_STRSRC_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_STRSRC_TARGET)

$(TEST_SVGWRITER_TARGET): $(TEST_SVGWRITER_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_SVGWRITER_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_SVGWRITER_TARGET)

$(TEST_XMLREADER_TARGET): $(TEST_XMLREADER_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_XMLREADER_OBJ_FILES) $(TEST_LDFLAGS) -lexpat -o $(TEST_XMLREADER_TARGET)

$(TEST_SVG_OBJ): $(SRC_DIR)/svg.c
	$(CC) $(TEST_CFLAGS) $(DEFINES) $(INCLUDE) -c $(SRC_DIR)/svg.c -o $(TEST_SVG_OBJ)

$(TEST_XMLBS_TARGET): $(TEST_XMLBS_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_XMLBS_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_XMLBS_TARGET)

$(TEST_TPCL_TARGET): $(TEST_TPCL_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_TPCL_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_TPCL_TARGET)

$(TEST_OSM_TARGET): $(TEST_OSM_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OSM_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_OSM_TARGET)

$(TEST_BSIDX_TARGET): $(TEST_BSIDXL_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_BSIDXL_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_BSIDX_TARGET)

$(TEST_SVG_TEST_OBJ): $(TESTSRC_DIR)/SVGTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $(TESTSRC_DIR)/SVGTest.cpp -o $(TEST_SVG_TEST_OBJ)

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(TESTSRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(TEST_FILEDATASS_TARGET): $(TEST_FILEDATASS_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_FILEDATASS_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_FILEDATASS_TARGET)

$(TEST_GEOUTILS_TARGET): $(TEST_GEOUTILS_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_GEOUTILS_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_GEOUTILS_TARGET)

$(TEST_SMIDX_TARGET): $(TEST_SMIDX_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_SMIDX_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_SMIDX_TARGET)

$(TEST_TP_TARGET): $(TEST_TP_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_TP_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_TP_TARGET)

$(TEST_TEXTTPW_TARGET): $(TEST_TEXTTPW_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_TEXTTPW_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_TEXTTPW_TARGET)

$(TEST_SVGTPW_TARGET): $(TEST_SVGTPW_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_SVGTPW_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_SVGTPW_TARGET)

$(TEST_HTMLTPW_TARGET): $(TEST_HTMLTPW_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_HTMLTPW_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_HTMLTPW_TARGET)

$(TEST_TPCL_TARGET): $(TEST_TPCL_OBJ_FILES)
		$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_TPCL_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_TPCL_TARGET)
		
$(TRIPPLANNER_TARGET): $(TESTOBJ_DIR)/tripplannermain.o \
						$(TEST_BSIDX_OBJ) \
						$(TEST_SMIDX_OBJ) \
						$(TEST_TP_OBJ) \
						$(TEST_TEXTTPW_OBJ) \
						$(TEST_SVGTPW_OBJ) \
						$(TEST_HTMLTPW_OBJ) \
						$(TEST_TPCL_OBJ) \
						$(TEST_XMLREADER_OBJ) \
						$(TEST_XMLBS_OBJ) \
						$(TEST_OSM_OBJ) \
						$(TEST_STRSRC_OBJ) \
						$(TEST_STRSINK_OBJ) \
						$(STATIC_LIB)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $(TRIPPLANNER_TARGET)

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTCOVER_DIR)
	mkdir -p $(TESTTMP_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTCOVER_DIR)
	rm -rf $(TESTTMP_DIR)
	rm -rf run_*

## Response 1:
Here's what was fixed from your uploaded version:

TEST_SVGLTPW_OBJ → TEST_SVGTPW_OBJ (removed the extra L, everywhere it appeared)
TEST_SVGLTPW_OBJ_FILES → TEST_SVGTPW_OBJ_FILES (same)
TEST_SVGLTPW_TEST_OBJ reference removed — replaced with the correctly defined TEST_SVGTPW_TEST_OBJ
$(TEST _FILEDATASS_TEST_OBJ) → $(TEST_FILEDATASS_TEST_OBJ) (removed the space)
Trailing \ on last lines of TEST_TP_OBJ_FILES and TEST_TEXTTPW_OBJ_FILES removed
Duplicate run_tpcltest definition removed
Double-tab indentation on new build rules fixed to single tab

## Changes 1: 
### - Pasted the debugged and finished Makefile into the code, and now that Makefile code from the AI serves as our program's Makefile code

## Prompt 2:
Why am I receiving all these errors after fixing my Makefile?
In file included from /usr/include/c++/14/bits/stl_tempbuf.h:61,
                 from /usr/include/c++/14/memory:66,
                 from ./include/StreetMap.h:5,
                 from ./include/BusSystem.h:4,
                 from ./include/XMLBusSystem.h:4,
                 from src/XMLBusSystem.cpp:1:
/usr/include/c++/14/bits/stl_construct.h: In instantiation of ‘constexpr void std::_Construct(_Tp*, _Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Args = {__cxx11::basic_string<char, char_traits<char>, allocator<char> >&}]’:
/usr/include/c++/14/bits/alloc_traits.h:706:19:   required from ‘static constexpr void std::allocator_traits<std::allocator<void> >::construct(allocator_type&, _Up*, _Args&& ...) [with _Up = CXMLBusSystem::SImplementation::SRoute; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; allocator_type = std::allocator<void>]’
  706 |         { std::_Construct(__p, std::forward<_Args>(__args)...); }
      |           ~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr_base.h:607:39:   required from ‘std::_Sp_counted_ptr_inplace<_Tp, _Alloc, _Lp>::_Sp_counted_ptr_inplace(_Alloc, _Args&& ...) [with _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute; _Alloc = std::allocator<void>; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
  607 |           allocator_traits<_Alloc>::construct(__a, _M_ptr(),
      |           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
  608 |               std::forward<_Args>(__args)...); // might throw
      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
/usr/include/c++/14/bits/shared_ptr_base.h:969:16:   required from ‘std::__shared_count<_Lp>::__shared_count(_Tp*&, std::_Sp_alloc_shared_tag<_Alloc>, _Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
  969 |           auto __pi = ::new (__mem)
      |                       ^~~~~~~~~~~~~
  970 |             _Sp_cp_type(__a._M_a, std::forward<_Args>(__args)...);
      |             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr_base.h:1713:14:   required from ‘std::__shared_ptr<_Tp, _Lp>::__shared_ptr(std::_Sp_alloc_shared_tag<_Tp>, _Args&& ...) [with _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
 1713 |         : _M_ptr(), _M_refcount(_M_ptr, __tag, std::forward<_Args>(__args)...)
      |                     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr.h:463:59:   required from ‘std::shared_ptr<_Tp>::shared_ptr(std::_Sp_alloc_shared_tag<_Tp>, _Args&& ...) [with _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute]’
  463 |         : __shared_ptr<_Tp>(__tag, std::forward<_Args>(__args)...)
      |                                                                  ^
/usr/include/c++/14/bits/shared_ptr.h:1007:14:   required from ‘std::shared_ptr<std::_NonArray<_Tp> > std::make_shared(_Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Args = {__cxx11::basic_string<char, char_traits<char>, allocator<char> >&}; _NonArray<_Tp> = CXMLBusSystem::SImplementation::SRoute]’
 1007 |       return shared_ptr<_Tp>(_Sp_alloc_shared_tag<_Alloc>{__a},
      |              ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 1008 |                              std::forward<_Args>(__args)...);
      |                              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/XMLBusSystem.cpp:211:49:   required from here
  211 |         auto NewRoute = std::make_shared<SRoute>(RouteName);
      |                         ~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~
/usr/include/c++/14/bits/stl_construct.h:115:28: error: no matching function for call to ‘construct_at(CXMLBusSystem::SImplementation::SRoute*&, std::__cxx11::basic_string<char>&)’
  115 |           std::construct_at(__p, std::forward<_Args>(__args)...);
      |           ~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/stl_construct.h:94:5: note: candidate: ‘template<class _Tp, class ... _Args> constexpr decltype (::new(void*(0)) _Tp) std::construct_at(_Tp*, _Args&& ...)’
   94 |     construct_at(_Tp* __location, _Args&&... __args)
      |     ^~~~~~~~~~~~
/usr/include/c++/14/bits/stl_construct.h:94:5: note:   template argument deduction/substitution failed:
/usr/include/c++/14/bits/stl_construct.h: In substitution of ‘template<class _Tp, class ... _Args> constexpr decltype (::new(void*(0)) _Tp) std::construct_at(_Tp*, _Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}]’:
/usr/include/c++/14/bits/stl_construct.h:115:21:   required from ‘constexpr void std::_Construct(_Tp*, _Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Args = {__cxx11::basic_string<char, char_traits<char>, allocator<char> >&}]’
  115 |           std::construct_at(__p, std::forward<_Args>(__args)...);
      |           ~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/alloc_traits.h:706:19:   required from ‘static constexpr void std::allocator_traits<std::allocator<void> >::construct(allocator_type&, _Up*, _Args&& ...) [with _Up = CXMLBusSystem::SImplementation::SRoute; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; allocator_type = std::allocator<void>]’
  706 |         { std::_Construct(__p, std::forward<_Args>(__args)...); }
      |           ~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr_base.h:607:39:   required from ‘std::_Sp_counted_ptr_inplace<_Tp, _Alloc, _Lp>::_Sp_counted_ptr_inplace(_Alloc, _Args&& ...) [with _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute; _Alloc = std::allocator<void>; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
  607 |           allocator_traits<_Alloc>::construct(__a, _M_ptr(),
      |           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
  608 |               std::forward<_Args>(__args)...); // might throw
      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
/usr/include/c++/14/bits/shared_ptr_base.h:969:16:   required from ‘std::__shared_count<_Lp>::__shared_count(_Tp*&, std::_Sp_alloc_shared_tag<_Alloc>, _Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
  969 |           auto __pi = ::new (__mem)
      |                       ^~~~~~~~~~~~~
  970 |             _Sp_cp_type(__a._M_a, std::forward<_Args>(__args)...);
      |             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr_base.h:1713:14:   required from ‘std::__shared_ptr<_Tp, _Lp>::__shared_ptr(std::_Sp_alloc_shared_tag<_Tp>, _Args&& ...) [with _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
 1713 |         : _M_ptr(), _M_refcount(_M_ptr, __tag, std::forward<_Args>(__args)...)
      |                     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr.h:463:59:   required from ‘std::shared_ptr<_Tp>::shared_ptr(std::_Sp_alloc_shared_tag<_Tp>, _Args&& ...) [with _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute]’
  463 |         : __shared_ptr<_Tp>(__tag, std::forward<_Args>(__args)...)
      |                                                                  ^
/usr/include/c++/14/bits/shared_ptr.h:1007:14:   required from ‘std::shared_ptr<std::_NonArray<_Tp> > std::make_shared(_Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Args = {__cxx11::basic_string<char, char_traits<char>, allocator<char> >&}; _NonArray<_Tp> = CXMLBusSystem::SImplementation::SRoute]’
 1007 |       return shared_ptr<_Tp>(_Sp_alloc_shared_tag<_Alloc>{__a},
      |              ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 1008 |                              std::forward<_Args>(__args)...);
      |                              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/XMLBusSystem.cpp:211:49:   required from here
  211 |         auto NewRoute = std::make_shared<SRoute>(RouteName);
      |                         ~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~
/usr/include/c++/14/bits/stl_construct.h:96:17: error: invalid new-expression of abstract class type ‘CXMLBusSystem::SImplementation::SRoute’
   96 |     -> decltype(::new((void*)0) _Tp(std::declval<_Args>()...))
      |                 ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/XMLBusSystem.cpp:69:12: note:   because the following virtual functions are pure within ‘CXMLBusSystem::SImplementation::SRoute’:
   69 |     struct SRoute : public CBusSystem::SRoute {
      |            ^~~~~~
./include/BusSystem.h:26:33: note:     ‘virtual std::size_t CBusSystem::SRoute::TripCount() const’
   26 |             virtual std::size_t TripCount() const noexcept = 0;
      |                                 ^~~~~~~~~
./include/BusSystem.h:28:31: note:     ‘virtual CBusSystem::TStopTime CBusSystem::SRoute::GetStopTime(std::size_t, std::size_t) const’
   28 |             virtual TStopTime GetStopTime(std::size_t stopindex, std::size_t tripindex) const noexcept = 0;
      |                               ^~~~~~~~~~~
/usr/include/c++/14/bits/stl_construct.h: In instantiation of ‘constexpr void std::_Construct(_Tp*, _Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Args = {__cxx11::basic_string<char, char_traits<char>, allocator<char> >&}]’:
/usr/include/c++/14/bits/alloc_traits.h:706:19:   required from ‘static constexpr void std::allocator_traits<std::allocator<void> >::construct(allocator_type&, _Up*, _Args&& ...) [with _Up = CXMLBusSystem::SImplementation::SRoute; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; allocator_type = std::allocator<void>]’
  706 |         { std::_Construct(__p, std::forward<_Args>(__args)...); }
      |           ~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr_base.h:607:39:   required from ‘std::_Sp_counted_ptr_inplace<_Tp, _Alloc, _Lp>::_Sp_counted_ptr_inplace(_Alloc, _Args&& ...) [with _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute; _Alloc = std::allocator<void>; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
  607 |           allocator_traits<_Alloc>::construct(__a, _M_ptr(),
      |           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
  608 |               std::forward<_Args>(__args)...); // might throw
      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
/usr/include/c++/14/bits/shared_ptr_base.h:969:16:   required from ‘std::__shared_count<_Lp>::__shared_count(_Tp*&, std::_Sp_alloc_shared_tag<_Alloc>, _Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
  969 |           auto __pi = ::new (__mem)
      |                       ^~~~~~~~~~~~~
  970 |             _Sp_cp_type(__a._M_a, std::forward<_Args>(__args)...);
      |             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr_base.h:1713:14:   required from ‘std::__shared_ptr<_Tp, _Lp>::__shared_ptr(std::_Sp_alloc_shared_tag<_Tp>, _Args&& ...) [with _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute; __gnu_cxx::_Lock_policy _Lp = __gnu_cxx::_S_atomic]’
 1713 |         : _M_ptr(), _M_refcount(_M_ptr, __tag, std::forward<_Args>(__args)...)
      |                     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/14/bits/shared_ptr.h:463:59:   required from ‘std::shared_ptr<_Tp>::shared_ptr(std::_Sp_alloc_shared_tag<_Tp>, _Args&& ...) [with _Alloc = std::allocator<void>; _Args = {std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&}; _Tp = CXMLBusSystem::SImplementation::SRoute]’
  463 |         : __shared_ptr<_Tp>(__tag, std::forward<_Args>(__args)...)
      |                                                                  ^
/usr/include/c++/14/bits/shared_ptr.h:1007:14:   required from ‘std::shared_ptr<std::_NonArray<_Tp> > std::make_shared(_Args&& ...) [with _Tp = CXMLBusSystem::SImplementation::SRoute; _Args = {__cxx11::basic_string<char, char_traits<char>, allocator<char> >&}; _NonArray<_Tp> = CXMLBusSystem::SImplementation::SRoute]’
 1007 |       return shared_ptr<_Tp>(_Sp_alloc_shared_tag<_Alloc>{__a},
      |              ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 1008 |                              std::forward<_Args>(__args)...);
      |                              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/XMLBusSystem.cpp:211:49:   required from here
  211 |         auto NewRoute = std::make_shared<SRoute>(RouteName);
      |                         ~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~
/usr/include/c++/14/bits/stl_construct.h:119:7: error: invalid new-expression of abstract class type ‘CXMLBusSystem::SImplementation::SRoute’
  119 |       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
      |       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      
## Response 2: 
The key error is this line:
error: invalid new-expression of abstract class type 'CXMLBusSystem::SImplementation::SRoute'
Your SRoute inherits from CBusSystem::SRoute but doesn't implement two pure virtual functions that the base class requires:
'virtual std::size_t CBusSystem::SRoute::TripCount() const'
'virtual CBusSystem::TStopTime CBusSystem::SRoute::GetStopTime(std::size_t, std::size_t) const'
You need to add those two overrides to your SRoute struct:
cppstruct SRoute : public CBusSystem::SRoute {
    std::string DName;
    std::vector<TStopID> DStopIDs;

    SRoute(const std::string &name) : DName(name) {}
    ~SRoute() {}

    std::string Name() const noexcept override {
        return DName;
    }

    std::size_t StopCount() const noexcept override {
        return DStopIDs.size();
    }

    TStopID GetStopID(std::size_t index) const noexcept override {
        if(index >= DStopIDs.size()) return InvalidStopID;
        return DStopIDs[index];
    }

    // ADD THESE TWO:
    std::size_t TripCount() const noexcept override {
        return 0; // stub if not used
    }

    TStopTime GetStopTime(std::size_t stopindex, std::size_t tripindex) const noexcept override {
        return {}; // stub if not used
    }
};
Since your XML data apparently doesn't include trip/schedule data, returning 0 and a default-constructed TStopTime as stubs is fine. C++ requires every pure virtual method to be implemented before you can instantiate the class — even if you don't use those methods.

## Changes 2:
Added TripCount() and GetStopTime() into SRoute in XMLBusSystsem.cpp in order to remove errors temporarily
        std::size_t TripCount() const noexcept override {
             return 0; // stub if not used
         }

         TStopTime GetStopTime(std::size_t stopindex, std::size_t tripindex) const noexcept override {
             return {}; // stub if not used
        }   

## Prompt 3:
what is the difference between finding the earliest arrival vs latest departure in a  schedule search, and how does the initialization of your tracking variable change between the two

## Response 3:
When finding earliest arrival you want the minimum value so you initialize to the largest possible number so anything you find will be smaller and beat it. When finding latest departure you want the maximum value so you initialize to -1 so any real departure time which is always positive will beat it.
earliest arrival:
  start at MAX → any real value beats it
  update when: newValue < bestValue

latest departure:
  start at -1  → any real value beats it  
  update when: newValue > bestValue

## Changes 3:
// FindRouteLeaveTime - wants earliest arrival
long long bestArrival = std::numeric_limits<long long>::max(); // <- MAX
if (arrSec < bestArrival) // <- less than

// FindRouteArrivalTime - wants latest departure
long long bestDepart = -1LL; // <- -1
if (depSec > bestDepart) // <- greater than

## Prompt 4:
if i have multiple bus routes that all go from stop A to stop B, and each route runs multiple times a day, how do i find the single best trip across all routes and all runs at the same time

## Response 4: 
You use a nested loop. Outer loop goes through each route, inner loop goes through each trip on that route. You keep one tracking variable outside both loops that gets updated whenever you find something better. By the time both loops finish that variable holds the global best across everything.
bestArrival = MAX

for each route:
    for each trip on that route:
        if this trip is better than bestArrival:
            update bestArrival
            update bestRoute

return bestRoute

## Change 4:
long long bestArrival = std::numeric_limits<long long>::max();
std::shared_ptr<SRoute> bestRoute; // declared OUTSIDE both loops

for (const auto &name : routeNames) {        // outer loop
    for (size_t trip = 0; trip < ri->TripCount(); ++trip) { // inner loop
        if (arrSec < bestArrival) {          // updates across ALL routes
            bestArrival = arrSec;            // and ALL trips at once
            bestRoute = DBusSystem->RouteByName(name);
        }
    }
}

## Prompt 5:
if i am checking whether i am too late for something, should my boundary condition use strict less than or less than or equal to, and what real world scenario would break if i used the wrong one

## Response 5: Use strict less than for the skip condition. If you are checking whether to skip something because it happened before you arrived, you only want to skip it if it happened strictly before — not at the exact same moment. The real world example is a bus that departs at exactly the same time you arrive at the stop. With strict less than you catch it. With less than or equal you miss it even though you were there on time.
```
you arrive:    8:00 AM
bus departs:   8:00 AM

strict less than:
  8:00 < 8:00 → false → don't skip → you catch the bus ✓

less than or equal:
  8:00 <= 8:00 → true → skip → you miss the bus ✗
```
## Changes #5:
// WRONG - misses buses departing exactly at leaveat
if (depSec <= leaveAtSec) continue;

// CORRECT - only skips buses that already left
if (depSec < leaveAtSec) continue;

## Prompt 6:
if i am tracking two pieces of information that are supposed to always match each other inside a loop, what is the safest way to update them so they never get out of sync

## Response 6:
Always update both pieces of information together inside the exact same if block. Never update one in one place and the other somewhere else. The moment you separate the updates you create a window where one has changed and the other hasn't which means they describe different things. Putting them in the same block makes the update atomic — either both change or neither does.
```
UNSAFE - updates separated, can describe different things:
  if (newValue < best) best = newValue;
  ...other code...
  if (someOtherThing) rebuild relatedData;
  
SAFE - always together, always in sync:
  if (newValue < best) {
      best = newValue;        ← both updated
      rebuild relatedData;    ← in same block
  }
  ```

## Changes 6:
if (arrSec < bestArrival) {
    bestArrival = arrSec;        // tracking var updated
    bestPlan.clear();            // plan rebuilt immediately
    bestPlan.push_back(boardStep);   // always matches
    bestPlan.push_back(arriveStep);  // bestArrival exactly
}

## Prompt 7:
if a function modifies something through a reference parameter but then fails partway through, what is the safest pattern to make sure the caller never sees a partially modified result

## Response 7: 
Never touch the reference parameter until you are completely sure you have a valid result. Build everything into a local variable first. Only copy or move it into the reference parameter as the very last thing before returning success. That way if anything fails at any point during the function the caller's data is completely untouched.
```
UNSAFE:
  ref.clear();              ← caller's data already modified
  ref.push_back(item1);     ← partially built
  if (failed) return false; ← caller sees broken data

SAFE:
  local variable;           ← build here first
  local.push_back(item1);   ← caller untouched during build
  if (failed) return false; ← caller's data never touched
  ref = move(local);        ← only touch ref on success
  return true;
  ```

## Changes 7:
TTravelPlan bestPlan;            // local variable, not plan

bestPlan.push_back(boardStep);   // build locally
bestPlan.push_back(arriveStep);

if (bestPlan.empty()) return false;  // plan reference untouched

plan = std::move(bestPlan);  // only touch plan on success
return true;

## Prompt 8:
if i find item A at position N in an ordered list and then need to find a related item B that must come after A, why is starting my search at N+1 safer than starting at 0

## Response 8:
Starting at 0 could find B before A in the list which would mean B comes before A not after it. It could also find A itself if A and B happen to be the same item. Starting at N+1 guarantees that whatever you find is strictly after position N so the after relationship is always enforced. It eliminates an entire class of bugs where the order constraint gets violated silently.
```
List: [X, B, A, Y, B, Z]
         0  1  2  3  4  5

A is at index 2
we want B that comes AFTER A

search from 0: finds B at index 1
               index 1 < index 2
               B is actually BEFORE A ✗

search from 3: finds B at index 4
               index 4 > index 2
               B is correctly AFTER A ✓
```

## Changes 8
std::size_t srcIdx = ri->FindStopIndex(src);

// WRONG - could find dest before src
std::size_t destIdx = ri->FindStopIndex(dest, 0);

// CORRECT - guarantees dest is after src
std::size_t destIdx = ri->FindStopIndex(dest, srcIdx + 1);
