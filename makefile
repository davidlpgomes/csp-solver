CXX = g++
CXXFLAGS = -pedantic-errors -Wall -Wextra -Werror -O3

BUILD = build
OBJ_DIR = $(BUILD)/objects
EXEC_DIR = $(BUILD)/exec

INCLUDE = -I src/
SRC = $(wildcard src/*.cpp)

TARGET = cspsolver

OBJECTS = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES = $(OBJECTS:.o=.d)

all: build $(EXEC_DIR)/$(TARGET)

build:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(EXEC_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(EXEC_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(EXEC_DIR)/$(TARGET) $^

-include $(DEPENDENCIES)

.PHONY: all build clean info

clean:
	@echo "[*] Removing build directory: ${BUILD}"
	rm -rf $(BUILD)

info:
	@echo "[*] Executable directory: ${EXEC_DIR}"
	@echo "[*] Object directory: ${OBJ_DIR}"
	@echo "[*] Sources: ${SRC}"
	@echo "[*] Objects: ${OBJECTS}"
