#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <vector>
#include <string>
#include <unordered_map>

class CampusGraph {
	private:
    std::vector<std::string> buildings;
    std::vector<std::vector<int>> adjList;
  public:
    bool addBuilding(const std::string& name);
    bool addConnection(const std::string& buildingA,
                        const std::string& buildingB);

    int findBuilding(const std::string& name) const;

    void displayGraph() const;
    void displayStatistics() const;

    void bfsReachable(const std::string& startBuilding) const;

    void shortestPath(const std::string& start,
                      const std::string& destination) const;

    void connectivityReport() const;

    bool loadBuildings(const std::string& filename);
    bool loadConnections(const std::string& filename);
};

#endif
