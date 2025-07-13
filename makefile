# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -g -O2 -I/mingw64/include/SFML

# Source and build info
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = sudoku_game.exe

# Linker flags and libraries
LDFLAGS = -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -lm

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)