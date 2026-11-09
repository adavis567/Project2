#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

class CampusGraph {
private:
    std::vector<std::string> buildings;
    std::vector<std::vector<int>> adjList;
    int findBuildingIndex(const std::string& name) const;

public:
    bool addBuilding(const std::string& name);
    bool addConnection(const std::string& buildingA, const std::string& buildingB);
    int findBuilding(const std::string& name) const;
    void displayGraph() const;
    void displayStatistics() const;
    void bfsReachable(const std::string& startBuilding) const;
    void shortestPath(const std::string& start, const std::string& destination) const;
    void connectivityReport() const;
    bool loadBuildings(const std::string& filename);
    bool loadConnections(const std::string& filename);
    const std::vector<std::string>& getBuildings() const;
};

#endif
