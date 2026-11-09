#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "Facility.h"
#include "FileLoader.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

static string readLine(const string& prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return FileLoader::trim(value);
}

static void showMenu() {
    cout << "\n========== SMART CAMPUS NAVIGATION ==========\n"
         << "1. Display all facilities\n"
         << "2. Search facility by ID\n"
         << "3. Search facility by complete name\n"
         << "4. Search facilities by category\n"
         << "5. Facility category and hash report\n"
         << "6. Display campus graph\n"
         << "7. Display graph statistics\n"
         << "8. BFS reachable buildings\n"
         << "9. Find shortest path between buildings\n"
         << "10. Graph connectivity report\n"
         << "11. Display department hierarchy\n"
         << "12. Search department\n"
         << "13. Add department\n"
         << "14. Preorder traversal\n"
         << "15. Postorder traversal\n"
         << "16. Level-order traversal\n"
         << "17. Department statistics\n"
         << "0. Exit\n"
         << "Select an option: ";
}

int main(int argc, char* argv[]) {
    const string dataDirectory = (argc > 1 ? argv[1] : ".");
    const string separator = (dataDirectory.empty() || dataDirectory.back() == '/') ? "" : "/";

    CampusGraph graph;
    DepartmentTree departments;
    FacilityDatabase facilities;

    cout << "Loading campus data from " << dataDirectory << "...\n";
    graph.loadBuildings(dataDirectory + separator + "buildings.txt");
    graph.loadConnections(dataDirectory + separator + "connections.txt");
    facilities.loadFacilities(dataDirectory + separator + "facilities.txt", graph.getBuildings());
    departments.loadDepartments(dataDirectory + separator + "departments.txt");

    while (true) {
        showMenu();
        string choiceText;
        if (!getline(cin, choiceText)) break;
        int choice = -1;
        try {
            size_t used = 0;
            choice = stoi(FileLoader::trim(choiceText), &used);
            if (used != FileLoader::trim(choiceText).size()) choice = -1;
        } catch (...) {
            choice = -1;
        }

        switch (choice) {
            case 1:
                facilities.displayAll();
                break;
            case 2:
                facilities.searchByID(readLine("Enter facility ID: "));
                break;
            case 3:
                facilities.searchByName(readLine("Enter complete facility name: "));
                break;
            case 4:
                facilities.searchByCategory(readLine("Enter category: "));
                break;
            case 5:
                facilities.categoryReport();
                break;
            case 6:
                graph.displayGraph();
                break;
            case 7:
                graph.displayStatistics();
                break;
            case 8:
                graph.bfsReachable(readLine("Starting building: "));
                break;
            case 9: {
                string start = readLine("Starting building: ");
                string end = readLine("Destination building: ");
                graph.shortestPath(start, end);
                break;
            }
            case 10:
                graph.connectivityReport();
                break;
            case 11:
                departments.displayHierarchy();
                break;
            case 12:
                departments.searchDepartment(readLine("Department name: "));
                break;
            case 13: {
                string name = readLine("New department name: ");
                string parent = readLine("Parent department name: ");
                if (departments.addDepartment(name, parent))
                    cout << "Department added successfully.\n";
                else
                    cout << "Department was not added. Check for duplicate names and a valid parent.\n";
                break;
            }
            case 14:
                departments.displayPreorder();
                break;
            case 15:
                departments.displayPostorder();
                break;
            case 16:
                departments.displayLevelOrder();
                break;
            case 17:
                departments.displayStatistics();
                break;
            case 0:
                cout << "Exiting Smart Campus Navigation System.\n";
                return 0;
            default:
                cout << "Invalid selection. Enter a number from 0 through 17.\n";
        }
    }

    return 0;
}

