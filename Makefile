# Define the tools
AR=ar
CC=gcc
CXX=g++

# Define the directories
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

# Define the flags for compilation/linking
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

# Define the object files
TEST_SVG_OBJ		= $(TESTOBJ_DIR)/svg.o
TEST_SVG_TEST_OBJ	= $(TESTOBJ_DIR)/SVGTest.o
TEST_OBJ_FILES		= $(TEST_SVG_OBJ) $(TEST_SVG_TEST_OBJ)

TEST_STRSINK_OBJ		= $(TESTOBJ_DIR)/StringDataSink.o
TEST_STRSINK_TEST_OBJ	= $(TESTOBJ_DIR)/StringDataSinkTest.o
TEST_STRSINK_OBJ_FILES	= $(TEST_STRSINK_OBJ) $(TEST_STRSINK_TEST_OBJ)
TEST_STDSINK_OBJ        = $(TESTOBJ_DIR)/StandardDataSink.o

TEST_STRSRC_OBJ			= $(TESTOBJ_DIR)/StringDataSource.o
TEST_STRSRC_TEST_OBJ	= $(TESTOBJ_DIR)/StringDataSourceTest.o
TEST_STRSRC_OBJ_FILES	= $(TEST_STRSRC_OBJ) $(TEST_STRSRC_TEST_OBJ)
TEST_STDSRC_OBJ         = $(TESTOBJ_DIR)/StandardDataSource.o
TEST_STDERR_OBJ         = $(TESTOBJ_DIR)/StandardErrorDataSink.o

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
TEST_BSIDXL_OBJ_FILES	= $(TEST_STRSRC_OBJ) \
						$(TEST_MOCK_BS_OBJ) \
						$(TEST_BSIDX_OBJ) \
						$(TEST_BSIDX_TEST_OBJ)

TEST_SMIDX_OBJ			= $(TESTOBJ_DIR)/StreetMapIndexer.o
TEST_SMIDX_TEST_OBJ		= $(TESTOBJ_DIR)/StreetMapIndexerTest.o
TEST_SMIDX_OBJ_FILES	= $(TEST_STRSRC_OBJ) $(TEST_MOCK_SM_OBJ) $(TEST_SMIDX_OBJ) $(TEST_SMIDX_TEST_OBJ)

TEST_HTMLTPW_OBJ		= $(TESTOBJ_DIR)/HTMLTripPlanWriter.o
TEST_SVGTPW_OBJ			= $(TESTOBJ_DIR)/SVGTripPlanWriter.o
TEST_TEXTTPW_OBJ		= $(TESTOBJ_DIR)/TextTripPlanWriter.o

TEST_TP_OBJ				= $(TESTOBJ_DIR)/TripPlanner.o
TEST_TPCL_OBJ			= $(TESTOBJ_DIR)/TripPlannerCommandLine.o
TEST_TPCL_TEST_OBJ		= $(TESTOBJ_DIR)/TripPlannerCommandLineTest.o

TEST_TP_TEST_OBJ		= $(TESTOBJ_DIR)/TripPlannerTest.o
TEST_TP_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
						$(TEST_MOCK_BS_OBJ) \
						$(TEST_BSIDX_OBJ) \
						$(TEST_TP_OBJ) \
						$(TEST_TP_TEST_OBJ)

TEST_TEXTTPW_TEST_OBJ	= $(TESTOBJ_DIR)/TextTripPlanWriterTest.o
TEST_TEXTTPW_OBJ_FILES	= $(TEST_STRSRC_OBJ) \
						$(TEST_STRSINK_OBJ) \
						$(TEST_MOCK_BS_OBJ) \
						$(TEST_TEXTTPW_OBJ) \
						$(TEST_TEXTTPW_TEST_OBJ)

TEST_SVGTPW_TEST_OBJ	= $(TESTOBJ_DIR)/SVGTripPlanWriterTest.o
TEST_SVGTPW_OBJ_FILES	= $(TEST_STRSRC_OBJ) \
						$(TEST_STRSINK_OBJ) \
						$(TEST_MOCK_BS_OBJ) \
						$(TEST_MOCK_SM_OBJ) \
						$(TEST_SMIDX_OBJ) \
                        $(TEST_SVGWRITER_OBJ) \
                        $(TEST_GEOUTILS_OBJ) \
                        $(TEST_XMLREADER_OBJ) \
						$(TEST_SVGTPW_OBJ) \
						$(TEST_SVGTPW_TEST_OBJ) \
						$(STATIC_LIB)

TEST_HTMLTPW_TEST_OBJ	= $(TESTOBJ_DIR)/HTMLTripPlanWriterTest.o
TEST_HTMLTPW_OBJ_FILES	= $(TEST_STRSRC_OBJ) \
						$(TEST_STRSINK_OBJ) \
						$(TEST_MOCK_BS_OBJ) \
						$(TEST_MOCK_SM_OBJ) \
						$(TEST_SMIDX_OBJ) \
                        $(TEST_SVGWRITER_OBJ) \
                        $(TEST_GEOUTILS_OBJ) \
                        $(TEST_XMLREADER_OBJ) \
						$(TEST_SVGTPW_OBJ) \
						$(TEST_TEXTTPW_OBJ) \
						$(TEST_HTMLTPW_OBJ) \
						$(TEST_HTMLTPW_TEST_OBJ) \
						$(STATIC_LIB)

TEST_TPCL_OBJ_FILES		= $(TEST_STRSRC_OBJ) \
						$(TEST_STRSINK_OBJ) \
						$(TEST_MOCK_BS_OBJ) \
						$(TEST_MOCK_SM_OBJ) \
						$(TEST_BSIDX_OBJ) \
						$(TEST_SMIDX_OBJ) \
<<<<<<< HEAD
						$(TEST_SVGWRITER_OBJ) \
						$(TEST_GEOUTILS_OBJ) \
						$(TEST_XMLREADER_OBJ) \
=======
                        $(TEST_SVGWRITER_OBJ) \
                        $(TEST_GEOUTILS_OBJ) \
                        $(TEST_XMLREADER_OBJ) \
>>>>>>> b9b76866b5d47552bc20cf9bb5ea597e52289ef6
						$(TEST_HTMLTPW_OBJ) \
						$(TEST_SVGTPW_OBJ) \
						$(TEST_TEXTTPW_OBJ) \
						$(TEST_TP_OBJ) \
						$(TEST_TPCL_OBJ) \
						$(TEST_TPCL_TEST_OBJ) \
						$(STATIC_LIB)

TEST_FILEDATASS_OBJ		= $(TESTOBJ_DIR)/FileDataFactory.o \
						$(TESTOBJ_DIR)/FileDataSource.o \
						$(TESTOBJ_DIR)/FileDataSink.o
TEST_FILEDATASS_TEST_OBJ	= $(TESTOBJ_DIR)/FileDataSSTest.o
TEST_FILEDATASS_OBJ_FILES	= $(TEST_FILEDATASS_OBJ) $(TEST_FILEDATASS_TEST_OBJ)

TEST_GEOUTILS_OBJ		= $(TESTOBJ_DIR)/GeographicUtils.o
TEST_GEOUTILS_TEST_OBJ	= $(TESTOBJ_DIR)/GeographicUtilsTest.o
TEST_GEOUTILS_OBJ_FILES	= $(TEST_GEOUTILS_OBJ) $(TEST_GEOUTILS_TEST_OBJ)

# Define the targets
PROD_SVG_OBJ		= $(OBJ_DIR)/svg.o
STATIC_LIB			= $(LIB_DIR)/libsvg.a
MAIN_TARGET			= $(BIN_DIR)/main

TEST_SVG_TARGET			= $(TESTBIN_DIR)/testsvg
TEST_STRSINK_TARGET		= $(TESTBIN_DIR)/teststrdatasink
TEST_STRSRC_TARGET		= $(TESTBIN_DIR)/teststrdatasource
TEST_SVGWRITER_TARGET	= $(TESTBIN_DIR)/testsvgwritertest
TEST_XMLREADER_TARGET	= $(TESTBIN_DIR)/testxml
TEST_XMLBS_TARGET		= $(TESTBIN_DIR)/testxmlbs
TEST_OSM_TARGET			= $(TESTBIN_DIR)/testosm
TEST_BSIDX_TARGET		= $(TESTBIN_DIR)/testbussystemindexer
TEST_SMIDX_TARGET		= $(TESTBIN_DIR)/teststreetmapindexer
TEST_TP_TARGET			= $(TESTBIN_DIR)/testtripplanner
TEST_TEXTTPW_TARGET		= $(TESTBIN_DIR)/testtexttripplanwriter
TEST_SVGTPW_TARGET		= $(TESTBIN_DIR)/testsvgtripplanwriter
TEST_HTMLTPW_TARGET		= $(TESTBIN_DIR)/testhtmltripplanwriter
TEST_TPCL_TARGET		= $(TESTBIN_DIR)/testtripplannercommandline
TEST_FILEDATASS_TARGET	= $(TESTBIN_DIR)/testfiledatass
TEST_GEOUTILS_TARGET	= $(TESTBIN_DIR)/testgeographicutils
TRIPPLANNER_TARGET		= $(BIN_DIR)/tripplanner

all: directories \
	make_svglib \
	runmain \
	xmldiff \
	run_svgtest \
	run_sinktest \
	run_srctest \
	run_svgwritertest \
	run_xmltest \
	run_xmlbstest \
	run_osmtest \
	run_filedatasstest \
	run_bsidxtest \
	run_geoutilstest \
	run_smidxtest \
	run_tptest \
	run_texttpwtest \
	run_svgtpwtest \
	run_htmltpwtest \
	run_tpcltest \
	run_tripmain \
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

run_xmltest: $(TEST_XMLREADER_TARGET)
	$(TEST_XMLREADER_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_xmlbstest: $(TEST_XMLBS_TARGET)
	$(TEST_XMLBS_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_osmtest: $(TEST_OSM_TARGET)
	$(TEST_OSM_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

run_bsidxtest: $(TEST_BSIDX_TARGET)
	$(TEST_BSIDX_TARGET) --gtest_output=xml:$(TESTTMP_DIR)/$@
	mv $(TESTTMP_DIR)/$@ $@

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

$(PROD_SVG_OBJ): $(SRC_DIR)/svg.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE) -c $(SRC_DIR)/svg.c -o $(PROD_SVG_OBJ)

$(STATIC_LIB): $(PROD_SVG_OBJ)
	$(AR) $(ARFLAGS) $(STATIC_LIB) $(PROD_SVG_OBJ)

$(MAIN_TARGET): $(SRC_DIR)/main.c $(STATIC_LIB)
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE) $(SRC_DIR)/main.c $(STATIC_LIB) -o $(MAIN_TARGET)

runmain: $(MAIN_TARGET)
	$(MAIN_TARGET)

run_tripmain: $(TRIPPLANNER_TARGET)
	$(TRIPPLANNER_TARGET)

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

$(TEST_OSM_TARGET): $(TEST_OSM_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OSM_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_OSM_TARGET)

$(TEST_BSIDX_TARGET): $(TEST_BSIDXL_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_BSIDXL_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_BSIDX_TARGET)

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
                    $(TEST_GEOUTILS_OBJ) \
                    $(TEST_SVGWRITER_OBJ) \
                    $(TEST_FILEDATASS_OBJ) \
                    $(TEST_STDSRC_OBJ) \
                    $(TEST_STDSINK_OBJ) \
                    $(TEST_STDERR_OBJ) \
                    $(STATIC_LIB)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $(TRIPPLANNER_TARGET)

$(TEST_SVG_TEST_OBJ): $(TESTSRC_DIR)/SVGTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $(TESTSRC_DIR)/SVGTest.cpp -o $(TEST_SVG_TEST_OBJ)

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(TESTSRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

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