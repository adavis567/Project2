CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = campus

# Keep this list synchronized with the source files in src/.
SOURCES = src/main.cpp \
          src/Facility.cpp \
          src/HashTable.cpp \
          src/CampusGraph.cpp \
          src/DepartmentTree.cpp \
          src/FileLoader.cpp

OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

