# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -g -O2 -IC:/SFML-2.6.2/include

# Source and build info
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
RES = $(SRC_DIR)/resource.o
TARGET = sudokrazy.exe

# Linker flags and libraries
LDFLAGS = -LC:/SFML-2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lm -mwindows

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ) $(RES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/resource.o: $(SRC_DIR)/resource.rc
	windres $< -o $@

clean:
	rm -f $(OBJ) $(RES) $(TARGET)