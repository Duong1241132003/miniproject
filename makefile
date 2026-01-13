CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -MMD -MP
LDFLAGS  := -lwinmm

BUILD_DIR := build
SRC_ROOT  := src
INC_ROOT  := inc

# Include Paths
INC_DIRS := $(INC_ROOT)/model $(INC_ROOT)/library $(INC_ROOT)/playback \
            $(INC_ROOT)/algorithm $(INC_ROOT)/player
INCLUDES := $(foreach dir, $(INC_DIRS), -I$(dir))

# --- Source Discovery ---

# Core (Static)
CORE_DIRS := $(SRC_ROOT)/model $(SRC_ROOT)/algorithm
CORE_SRCS := $(foreach dir,$(CORE_DIRS),$(wildcard $(dir)/*.cpp))
CORE_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(CORE_SRCS)))

# Engine (Shared)
ENG_DIRS  := $(SRC_ROOT)/player $(SRC_ROOT)/playback $(SRC_ROOT)/library
ENG_SRCS  := $(foreach dir,$(ENG_DIRS),$(wildcard $(dir)/*.cpp))
ENG_OBJS  := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(ENG_SRCS)))

# App (Exe)
APP_SRCS  := $(SRC_ROOT)/main.cpp
APP_OBJS  := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(APP_SRCS)))

# Targets
LIB_CORE   := $(BUILD_DIR)/libcore.a
LIB_ENGINE := $(BUILD_DIR)/libengine.dll
TARGET     := $(BUILD_DIR)/main.exe

# Dependencies & VPATH
ALL_OBJS := $(CORE_OBJS) $(ENG_OBJS) $(APP_OBJS)
DEPS     := $(ALL_OBJS:.o=.d)
VPATH    := $(CORE_DIRS) $(ENG_DIRS) $(SRC_ROOT)

# --- Rules ---

.PHONY: all run clean rebuild

all: $(BUILD_DIR) $(LIB_CORE) $(LIB_ENGINE) $(TARGET)

$(BUILD_DIR):
	mkdir -p $@

# Static Lib
$(LIB_CORE): $(CORE_OBJS)
	$(AR) rcs $@ $^

# Shared Lib
$(LIB_ENGINE): $(ENG_OBJS) $(LIB_CORE)
	$(CXX) -shared -o $@ $(ENG_OBJS) -L$(BUILD_DIR) -lcore \
		-Wl,--out-implib,$(BUILD_DIR)/libengine.dll.a \
		-Wl,--export-all-symbols $(LDFLAGS)

# Executable
$(TARGET): $(APP_OBJS) $(LIB_ENGINE)
	$(CXX) $(APP_OBJS) -o $@ -L$(BUILD_DIR) -lengine -lcore $(LDFLAGS)

# Compile Object
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

-include $(DEPS)