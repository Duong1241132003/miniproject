CC = g++

INC_DIR := ./inc/model ./inc/library ./inc/playback 
SRC_DIR := ./src/model ./src/library ./src/playback ./src

DEPS = $(wildcard $(foreach dir, $(INC_DIR), $(dir)/*.h))

CFLAGS = -std=c++17 -Wall -Wextra -Wpedantic $(foreach inc, $(INC_DIR),-I$(inc))
VPATH = $(foreach source, $(SRC_DIR),$(source))

OBJECT = build/main.o build/MusicLibrary.o build/Song.o build/PlaybackQueue.o

LDFLAGS  = -lwinmm

all: $(OBJECT)
	$(CC) $^ -o main.exe $(LDFLAGS)
	./main.exe

build/%.o: %.cpp $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS) 

.PHONY: clean
clean:
	rm -f build/*.o main.exe 




# # ==============================
# # Compiler configuration
# # ==============================
# CXX := g++
# CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic \
#             -Iinc \
#             -Iinc/model \
#             -Iinc/library \
#             -Iinc/playback 
# #             -Iinc/player \
# #             -Iinc/algorithm

# # ==============================
# # Project configuration
# # ==============================
# TARGET := main

# # ==============================
# # Source files
# # ==============================
# SRCS := \
#     src/main.cpp \
#     src/model/Song.cpp \
#     src/library/MusicLibrary.cpp \
#     src/playback/PlaybackQueue.cpp 
# #     src/playback/PlaybackHistory.cpp \
# #     src/playback/ShuffleManager.cpp \
# #     src/player/MusicPlayer.cpp \
# #     src/algorithm/SmartPlaylist.cpp

# # ==============================
# # Object files
# # ==============================
# OBJS := $(SRCS:.cpp=.o)

# # ==============================
# # Build rules
# # ==============================

# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CXX) $(OBJS) -o $(TARGET)

# # Compile each .cpp into .o
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # ==============================
# # Utility targets
# # ==============================

# run: $(TARGET)
# 	./$(TARGET)

# clean:
# 	rm -f $(OBJS) $(TARGET)

# rebuild: clean all

# .PHONY: all run clean rebuild
