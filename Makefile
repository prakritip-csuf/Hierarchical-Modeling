#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# Linux:
#   apt-get install libglfw3-dev
#

#CXX = g++
#CXX = clang++

CC = g++
CFLAGS = -Iinclude -std=c++11

EXE = editor

IMGUI_HEADERS = include/imgui
IMGUI_DIR = src/imgui
TINYDIALOG_HEADERS = include/tinydialog
TINYDIALOG_DIR = src/tinydialog
SRC_HEADER = include
SRC_DIR = src
OBJ_DIR = obj
GLAD_HEADER = include/glad
GLAD_DIR = src/glad

SOURCES = main.cpp
SOURCES += $(GLAD_DIR)/glad.c
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(TINYDIALOG_DIR)/tinyfiledialogs.c
SOURCES += $(SRC_DIR)/Shape.cpp $(SRC_DIR)/Cube.cpp $(SRC_DIR)/Sphere.cpp $(SRC_DIR)/Pyramid.cpp $(SRC_DIR)/Teapot.cpp $(SRC_DIR)/ImportShape.cpp $(SRC_DIR)/ImportCurve.cpp $(SRC_DIR)/ImportCharacter.cpp $(SRC_DIR)/Custom.cpp $(SRC_DIR)/Icosahedron.cpp $(SRC_DIR)/Curve.cpp $(SRC_DIR)/Surface.cpp $(SRC_DIR)/Joint.cpp $(SRC_DIR)/MatrixStack.cpp $(SRC_DIR)/SkeletalModel.cpp $(SRC_DIR)/ColorPresets.cpp $(SRC_DIR)/FileImporter.cpp $(SRC_DIR)/Renderer.cpp $(SRC_DIR)/ShapeManager.cpp $(SRC_DIR)/Application.cpp $(SRC_DIR)/Globals.cpp
SOURCES += $(SRC_DIR)/ErrorHandling.cpp $(SRC_DIR)/ShaderLoader.cpp 

# Object files (in obj directory)
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(IMGUI_HEADERS) -I$(TINYDIALOG_HEADERS) -I$(SRC_HEADER) -I$(GLAD_HEADER)
CXXFLAGS += -g -Wall -Wformat


##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -lglfw -ldl
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -lglfw3
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(OBJ_DIR)/%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(TINYDIALOG_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o:$(GLAD_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
