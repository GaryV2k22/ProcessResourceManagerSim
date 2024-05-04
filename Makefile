CXX = g++

CXXFLAGS = -Wall -Iheaders

LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj

# Use wildcard to list all cpp files in the SRC_DIR
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Transform .cpp files in SOURCES to .o files in OBJ_DIR
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TARGET = program

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
