# Define the directory containing this Makefile
MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Define the working directory you want to switch to
WORKING_DIR := $(MAKEFILE_DIR)/../../..

# Detect the build type
BUILD_TYPE ?= debug
FINAL_BUILD_DIR := $(WORKING_DIR)/build/linux-x86_64-$(BUILD_TYPE)

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall $(shell sdl2-config --cflags) -I$(WORKING_DIR)/vendor/tinyxml2
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lSDL2_ttf
ifeq ($(BUILD_TYPE), release)
    CXXFLAGS += -O2
else
    CXXFLAGS += -O0 -g
endif

# Source files
COMMON_SOURCES := $(shell find $(WORKING_DIR)/src -name "*.cpp" -not -path "$(WORKING_DIR)/src/Platform/*")
PLATFORM_SPECIFIC_SOURCES := $(shell find $(WORKING_DIR)/src/Platform/Linux -name "*.cpp") $(WORKING_DIR)/src/Platform/Common/SaveDataHelper.cpp
VENDOR_SOURCE := $(WORKING_DIR)/vendor/tinyxml2/tinyxml2.cpp
SOURCES := $(COMMON_SOURCES) $(PLATFORM_SPECIFIC_SOURCES) $(VENDOR_SOURCE)

# Object files (replace .cpp with .o in the FINAL_BUILD_DIR)
OBJECTS := $(patsubst $(WORKING_DIR)/%.cpp, $(FINAL_BUILD_DIR)/%.o, $(SOURCES))

# Target executable
TARGET := $(FINAL_BUILD_DIR)/DeadlyStagesRemix

# Default target
all: $(TARGET)

# Link objects into the final executable
$(TARGET): $(OBJECTS)
	mkdir -p $(FINAL_BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@if [ "$(BUILD_TYPE)" = "release" ]; then \
		rm -rf $(FINAL_BUILD_DIR)/src $(FINAL_BUILD_DIR)/vendor; \
	fi

# Compile each .cpp file to .o, creating directories as needed
$(FINAL_BUILD_DIR)/%.o: $(WORKING_DIR)/%.cpp $(WORKING_DIR)/src/Core/Config.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(FINAL_BUILD_DIR)

.PHONY: all clean
