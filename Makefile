CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
TARGET = campus
SOURCES = main.cpp FileLoader.cpp HashTable.cpp Facility.cpp CampusGraph.cpp DepartmentTree.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
