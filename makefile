CC := g++
CXXSHAREDFLAGS := -Wall -fPIC -MMD -MP
CXXFLAGS := -Wall -std=c++14 -g 
CPPFLAGS := -Iinclude

LDFLAGS = -shared
LIBS := -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lGLEW -lboost_system -lboost_filesystem

INC_DIR = ./include
SRC_DIR := ./src
BUILD_DIR := ./build
TARGET_DIR := ./bin

TARGET := $(TARGET_DIR)/gl_framework.so
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(filter-out $(MAINS), $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES)))

all: main

init:
	mkdir build bin

shared: $(OBJ_FILES)
	$(CC) $(CXXSHAREDFLAGS) -o $(TARGET) $(LIBS) $^ $(LDFLAGS)

main: $(OBJ_FILES) $(BUILD_DIR)/main.o
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -o $(TARGET_DIR)/$@ $(LIBS) $^ 
	cp -r $(SRC_DIR)/shaders $(TARGET_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm $(BUILD_DIR)/*.o

-include $(OBJ_FILES:.o=.d)
