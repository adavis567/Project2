SMART CAMPUS NAVIGATION AND FACILITY ANALYTICS SYSTEM

Project 2

TEAM MEMBERS

* Anthony Davis — Facility management and custom ID hash table.
* Caleb Harper — Campus graph, BFS, shortest path, and menu integration.
* Eyob Tesfay — Department general tree and traversals.

PURPOSE

This C++ program loads campus buildings, walkway connections, facilities, and academic departments from text files.

The program allows users to:

* View and search campus facilities.
* Search facilities by ID, name, and category.
* Explore campus buildings and walkway connections.
* Find reachable buildings using Breadth-First Search (BFS).
* Find shortest paths between campus buildings.
* Add and search academic departments.
* Display department hierarchies and tree traversals.
* View department statistics and facility and graph reports.

DIRECTORY LAYOUT

Project2/
|-- include/
|   |-- Facility.h
|   |-- HashTable.h
|   |-- CampusGraph.h
|   |-- DepartmentTree.h
|   `-- FileLoader.h
|-- src/
|   |-- main.cpp
|   |-- Facility.cpp
|   |-- HashTable.cpp
|   |-- CampusGraph.cpp
|   |-- DepartmentTree.cpp
|   `-- FileLoader.cpp
|-- data/
|   |-- buildings.txt
|   |-- connections.txt
|   |-- facilities.txt
|   `-- departments.txt
|-- testdata/
|-- README.txt
|-- CONTRIBUTIONS.txt
|-- design_document.txt
|-- test_results.txt
|-- Makefile
`-- .gitignore

BUILDING THE PROGRAM

From the Project2 root directory, run:

make

The Makefile uses g++ with C++17 and compiler warning flags.

To remove the generated executable and object files, run:

make clean

RUNNING THE PROGRAM

To run using the default data directory:

./campus

To run using a different data directory:

./campus testdata/collision_test

The optional directory argument allows the program to load different datasets without modifying the source code. Directory paths are relative to the current working directory.

INPUT FILES

The program loads the following four files in order:

1. buildings.txt
2. connections.txt
3. facilities.txt
4. departments.txt

The instructor-provided files are stored in the data/ directory and remain unchanged.

Additional testing datasets are stored in separate subdirectories of testdata/.

IMPLEMENTATION OVERVIEW

The project is organized into separate modules:

Facility and HashTable

* Stores and manages facility records.
* Implements a custom string hash function.
* Uses separate chaining for collision handling.
* Supports facility ID, name, and category searches.
* Generates the facility category report.

CampusGraph

* Stores buildings and undirected walkway connections.
* Implements graph statistics and degree calculations.
* Performs BFS reachability and shortest-path searches.
* Generates the graph connectivity report.

DepartmentTree

* Implements the general department tree.
* Supports department insertion and searching.
* Displays the department hierarchy.
* Implements preorder, postorder, and level-order traversals.
* Calculates department statistics.

FileLoader and Main

* Loads and validates the four input files.
* Reports accepted and rejected records.
* Coordinates the program’s menu and user interaction.

The facility ID lookup must use the custom hash table. Standard library hash tables and linear scans cannot replace this implementation.

TEAM WORKFLOW

Each team member develops and maintains their assigned modules.

Completed work is submitted through pull requests into the develop branch. The integrated project is tested on develop before being merged into main.

All team members contribute to testing, debugging, code reviews, documentation, and final integration.

CELL MACHINE VERIFICATION

The project must compile and run successfully on the UNT CSE CELL machines before submission.

The exact successful build and run commands, along with testing outcomes, are documented in test_results.txt.