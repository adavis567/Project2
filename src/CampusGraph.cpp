#include "CampusGraph.h"
#include "FileLoader.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <limits>

using namespace std;

int CampusGraph::findBuildingIndex(const string& name) const {
    const string target = FileLoader::lower(FileLoader::trim(name));
    for (size_t i = 0; i < buildings.size(); ++i)
        if (FileLoader::lower(buildings[i]) == target) return static_cast<int>(i);
    return -1;
}

int CampusGraph::findBuilding(const string& name) const {
    return findBuildingIndex(name);
}

bool CampusGraph::addBuilding(const string& name) {
    string clean = FileLoader::trim(name);
    if (clean.empty() || findBuildingIndex(clean) >= 0) return false;
    buildings.push_back(clean);
    adjList.emplace_back();
    return true;
}

bool CampusGraph::addConnection(const string& buildingA, const string& buildingB) {
    int a = findBuildingIndex(buildingA);
    int b = findBuildingIndex(buildingB);
    if (a < 0 || b < 0 || a == b) return false;
    if (find(adjList[a].begin(), adjList[a].end(), b) != adjList[a].end()) return false;
    adjList[a].push_back(b);
    adjList[b].push_back(a);
    return true;
}

bool CampusGraph::loadBuildings(const string& filename) {
    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << ".\n";
        return false;
    }
    string line;
    int lineNumber = 0;
    size_t accepted = 0, rejected = 0;
    while (getline(input, line)) {
        ++lineNumber;
        if (FileLoader::isIgnoredLine(line)) continue;
        if (addBuilding(FileLoader::trim(line))) ++accepted;
        else {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": empty or duplicate building.\n";
            ++rejected;
        }
    }
    cout << "Buildings: " << accepted << " accepted, " << rejected << " rejected.\n";
    return accepted > 0;
}

bool CampusGraph::loadConnections(const string& filename) {
    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << ".\n";
        return false;
    }
    string line;
    int lineNumber = 0;
    size_t accepted = 0, rejected = 0;
    while (getline(input, line)) {
        ++lineNumber;
        if (FileLoader::isIgnoredLine(line)) continue;
        vector<string> fields = FileLoader::split(line, '|');
        if (fields.size() != 2 || !addConnection(fields[0], fields[1])) {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": invalid, duplicate, self, or unknown-building connection.\n";
            ++rejected;
        } else ++accepted;
    }
    cout << "Connections: " << accepted << " accepted, " << rejected << " rejected.\n";
    return true;
}

void CampusGraph::displayGraph() const {
    if (buildings.empty()) {
        cout << "No buildings loaded.\n";
        return;
    }
    for (size_t i = 0; i < buildings.size(); ++i) {
        cout << buildings[i] << ": ";
        for (size_t j = 0; j < adjList[i].size(); ++j) {
            if (j) cout << ", ";
            cout << buildings[adjList[i][j]];
        }
        cout << '\n';
    }
}

void CampusGraph::displayStatistics() const {
    size_t degreeSum = 0, isolated = 0;
    for (const auto& neighbors : adjList) {
        degreeSum += neighbors.size();
        if (neighbors.empty()) ++isolated;
    }
    cout << "\nCampus Graph Statistics\n"
         << "Buildings: " << buildings.size() << '\n'
         << "Connections: " << degreeSum / 2 << '\n'
         << "Average degree: "
         << (buildings.empty() ? 0.0 : static_cast<double>(degreeSum) / buildings.size()) << '\n'
         << "Isolated buildings: " << isolated << '\n';
}

void CampusGraph::bfsReachable(const string& startBuilding) const {
    int start = findBuildingIndex(startBuilding);
    if (start < 0) {
        cout << "Starting building not found.\n";
        return;
    }
    vector<bool> visited(buildings.size(), false);
    queue<int> q;
    visited[start] = true;
    q.push(start);
    cout << "BFS reachable from " << buildings[start] << ":\n";
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        cout << buildings[current] << '\n';
        for (int next : adjList[current]) {
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
}

void CampusGraph::shortestPath(const string& startName, const string& destinationName) const {
    int start = findBuildingIndex(startName);
    int destination = findBuildingIndex(destinationName);
    if (start < 0 || destination < 0) {
        cout << "One or both buildings were not found.\n";
        return;
    }
    vector<int> previous(buildings.size(), -1);
    vector<bool> visited(buildings.size(), false);
    queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty() && !visited[destination]) {
        int current = q.front();
        q.pop();
        for (int next : adjList[current]) {
            if (!visited[next]) {
                visited[next] = true;
                previous[next] = current;
                q.push(next);
            }
        }
    }
    if (!visited[destination]) {
        cout << "No path exists between those buildings.\n";
        return;
    }
    vector<int> path;
    for (int at = destination; at != -1; at = previous[at]) path.push_back(at);
    reverse(path.begin(), path.end());
    cout << "Shortest path (" << path.size() - 1 << " connections): ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i) cout << " -> ";
        cout << buildings[path[i]];
    }
    cout << '\n';
}

void CampusGraph::connectivityReport() const {
    cout << "\nGraph Connectivity Report\n";
    if (buildings.empty()) {
        cout << "No graph data available.\n";
        return;
    }
    vector<bool> visited(buildings.size(), false);
    vector<vector<string>> components;
    for (size_t i = 0; i < buildings.size(); ++i) {
        if (visited[i]) continue;
        vector<string> component;
        queue<int> q;
        q.push(static_cast<int>(i));
        visited[i] = true;
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            component.push_back(buildings[current]);
            for (int next : adjList[current]) {
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }
        components.push_back(component);
    }
    cout << "Connected components: " << components.size() << '\n';
    for (size_t i = 0; i < components.size(); ++i) {
        cout << "Component " << i + 1 << " (" << components[i].size() << "): ";
        for (size_t j = 0; j < components[i].size(); ++j) {
            if (j) cout << ", ";
            cout << components[i][j];
        }
        cout << '\n';
    }
    cout << "Graph is " << (components.size() == 1 ? "connected" : "not connected") << ".\n";
}

const vector<string>& CampusGraph::getBuildings() const { return buildings; }
