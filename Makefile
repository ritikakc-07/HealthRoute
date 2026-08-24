CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
SRC = src/BSTAdmin.cpp src/MergeSort.cpp src/Graph.cpp src/Ambulance.cpp src/FileHandler.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = healthroute

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
