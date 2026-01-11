# # CC = g++

# # INC_DIR := ./inc/model ./inc/library ./inc/playback ./inc/algorithm ./inc/player
# # SRC_DIR := ./src/model ./src/library ./src/playback ./src/algorithm ./src/player ./src

# # DEPS = $(wildcard $(foreach dir, $(INC_DIR), $(dir)/*.h))

# # CFLAGS = -std=c++17 -Wall -Wextra -Wpedantic $(foreach inc, $(INC_DIR),-I$(inc))
# # VPATH = $(foreach source, $(SRC_DIR),$(source))

# # OBJECT = build/main.o build/MusicLibrary.o build/Song.o build/PlaybackQueue.o build/SmartPlaylist.o build/PlayNextQueue.o build/ShuffleManager.o build/PlaybackHistory.o build/MusicPlayer.o

# # LDFLAGS  = -lwinmm

# # all: $(OBJECT)
# # 	$(CC) $^ -o main.exe $(LDFLAGS)

# # run:
# # 	./main.exe

# # build/%.o: %.cpp $(DEPS)
# # 	$(CC) -c $< -o $@ $(CFLAGS) 

# # .PHONY: clean
# # clean:
# # 	rm -f build/*.o main.exe 


# ==========================================
# Compiler & Flags Configuration
# ==========================================
CXX      := g++
AR       := ar
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
LDFLAGS  := -lwinmm

# Dependency tracking flags
DEPFLAGS := -MMD -MP

# Directories
BUILD_DIR := build
SRC_ROOT  := src
INC_ROOT  := inc

# ==========================================
# Module Separation
# ==========================================

# --- CORE (Static Library) ---
# Contains basic data structures and algorithms.
# These function are self-contained.
STATIC_LIB_NAME := libcore.a
STATIC_DIRS     := $(SRC_ROOT)/model \
                   $(SRC_ROOT)/algorithm

# --- ENGINE (Shared/Dynamic Library) ---
# Contains business logic and player control.
# IMPORTANT: This layer DEPENDS on the CORE layer.
SHARED_LIB_NAME := libengine.dll
SHARED_DIRS     := $(SRC_ROOT)/player \
                   $(SRC_ROOT)/playback \
                   $(SRC_ROOT)/library

# --- APP (Executable) ---
# Entry point
APP_DIRS        := $(SRC_ROOT)
APP_SRCS        := $(SRC_ROOT)/main.cpp

# Include paths
INC_DIRS := $(INC_ROOT)/model \
            $(INC_ROOT)/library \
            $(INC_ROOT)/playback \
            $(INC_ROOT)/algorithm \
            $(INC_ROOT)/player

INCLUDES := $(foreach dir, $(INC_DIRS), -I$(dir))

# ==========================================
# Object Discovery
# ==========================================

# Helper: Find sources and map to objects
get_srcs = $(foreach dir,$(1),$(wildcard $(dir)/*.cpp))
get_objs = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir $(1)))

# Core Objects
STATIC_SRCS := $(call get_srcs, $(STATIC_DIRS))
STATIC_OBJS := $(call get_objs, $(STATIC_SRCS))

# Engine Objects
SHARED_SRCS := $(call get_srcs, $(SHARED_DIRS))
SHARED_OBJS := $(call get_objs, $(SHARED_SRCS))

# App Objects
APP_OBJS    := $(call get_objs, $(APP_SRCS))

# All Objects & Dependencies
ALL_OBJS := $(STATIC_OBJS) $(SHARED_OBJS) $(APP_OBJS)
DEPS     := $(ALL_OBJS:.o=.d)

# VPATH for make to find source files
VPATH := $(STATIC_DIRS) $(SHARED_DIRS) $(APP_DIRS)

# ==========================================
# Build Rules
# ==========================================

.PHONY: all run clean rebuild

# Order is important here implies logic flow, though Make handles parallel execution.
all: $(BUILD_DIR) \
     $(BUILD_DIR)/$(STATIC_LIB_NAME) \
     $(BUILD_DIR)/$(SHARED_LIB_NAME) \
     $(BUILD_DIR)/main.exe

# --- Rule 1: Build Static Library (Core) ---
$(BUILD_DIR)/$(STATIC_LIB_NAME): $(STATIC_OBJS)
	@echo "[BUILD] Creating Static Library: $@"
	$(AR) rcs $@ $^

# --- Rule 2: Build Shared Library (Engine) ---
$(BUILD_DIR)/$(SHARED_LIB_NAME): $(SHARED_OBJS) $(BUILD_DIR)/$(STATIC_LIB_NAME)
	@echo "[BUILD] Creating Dynamic Library: $@"
	$(CXX) -shared -o $@ $(SHARED_OBJS) \
		-L$(BUILD_DIR) -lcore \
		-Wl,--out-implib,$(BUILD_DIR)/libengine.dll.a \
		-Wl,--export-all-symbols \
		$(LDFLAGS)

# --- Rule 3: Build Executable (Main) ---
# Links against both libraries.
$(BUILD_DIR)/main.exe: $(APP_OBJS) $(BUILD_DIR)/$(SHARED_LIB_NAME)
	@echo "[BUILD] Linking Executable: $@"
	$(CXX) $(APP_OBJS) -o $@ \
		-L$(BUILD_DIR) -lengine -lcore \
		$(LDFLAGS)

# --- Compiler Rule ---
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "[COMPILE] $<"
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# --- Helpers ---
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	@echo "[RUN] Starting application..."
	./$(BUILD_DIR)/main.exe

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

-include $(DEPS)