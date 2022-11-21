SOURCEDIR := src
HEADERDIR := hdr
BUILDDIR  := bin

# Create a list of *.cpp sources in DIRS
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
HEADERS = $(wildcard $(HEADERDIR)/*.hpp)

# Define objects for all sources
OBJS := $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCES:.cpp=.o))

TARGET := main.out
VPATH = $(SOURCEDIR)

# Define dependencies files for all objects
DEPS = $(OBJS:.o=.d)

# Name the compiler
CXX = g++

# set the flags
CXXFLAGS := -ggdb3 -Wall
INCLUDES := -I$(HEADERDIR)
LDFLAGS := -Llib
LDLIBS := -lm

.PHONY: all clean

all: $(OBJS)

$(TARGET): $(OBJS)
	@echo Linking $@
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET)

$(BUILDDIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDES) -o $@

# Include dependencies
-include $(DEPS)

# Remove all objects, dependencies and executable files generated during the build
clean:
	rm -rf $(BUILDDIR)/*
	rm $(TARGET)
	@echo Cleaning done ! 
