SOURCEDIR := src
HEADERDIR := hdr
BUILDDIR  := bin

MATH_HDR := ../MathEngine/hdr
MATH_SRC := ../MathEngine/src
MATH_BIN := ../MathEngine/bin
MATH_SRCS = $(wildcard $(MATH_SRC)/*.cpp)
MATH_HDRS = $(wildcard $(MATH_HDR)/*.hpp)

REND_HDR := ../RenderEngine/hdr
REND_SRC := ../RenderEngine/src
REND_BIN := ../RenderEngine/bin
REND_SRCS = $(wildcard $(REND_SRC)/*.cpp)
REND_HDRS = $(wildcard $(REND_HDR)/*.hpp)

# Create a list of *.cpp sources in DIRS
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
HEADERS = $(wildcard $(HEADERDIR)/*.hpp)

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.cpp=.o))
OBJS += $(subst $(MATH_SRC),$(MATH_BIN),$(MATH_SRCS:.cpp=.o))
OBJS += $(subst $(REND_SRC),$(REND_BIN),$(REND_SRCS:.cpp=.o))

TARGET := main.out
VPATH = $(SOURCEDIR)

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# Name the compiler
CXX = g++

# set the flags
CXXFLAGS := -ggdb3 -Wall
INCLUDES := -I$(HEADERDIR) -I$(MATH_HDR) -I$(REND_HDR)
LDFLAGS := -Llib
LDLIBS := -lm -lglfw -lGL -lGLU -lGLEW

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo Linking $@
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET)

$(BUILDDIR)/%.o: %.cpp $(HEADERS) $(REND_HDRS) $(MATH_HDRS) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDES) -o $@

$(BUILDDIR):
	@mkdir -p $@

# Include dependencies
-include $(DEPS)

# Remove all objects, dependencies and executable files generated during the build
clean:
	rm -rf $(BUILDDIR)/*
	rm $(TARGET)
	@echo Cleaning done ! 
