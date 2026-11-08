#include "DepartmentTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <cctype>

using namespace std;

// Node constructor
DepartmentTree::Node::Node(const string& departmentName, Node* parentNode) {
    name = departmentName;
    parent = parentNode;
}

// Tree constructor
DepartmentTree::DepartmentTree() {
    root = nullptr;
}

// Tree destructor
DepartmentTree::~DepartmentTree() {
    deleteTree(root);
}

// Delete all nodes to prevent memory leaks
void DepartmentTree::deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    for (Node* child : node->children) {
        deleteTree(child);
    }

    delete node;
}

// Remove whitespace from beginning and end
string DepartmentTree::trim(const string& text) const {
    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");

    return text.substr(first, last - first + 1);
}

// Convert text to lowercase for case-insensitive comparisons
string DepartmentTree::toLower(const string& text) const {
    string result = text;

    transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });

    return result;
}

// Find a department anywhere in the tree
DepartmentTree::Node* DepartmentTree::findNode(
    const string& name) const {

    if (root == nullptr) {
        return nullptr;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (toLower(current->name) == toLower(name)) {
            return current;
        }

        for (Node* child : current->children) {
            q.push(child);
        }
    }

    return nullptr;
}

// Add a department
bool DepartmentTree::addDepartment(
    const string& name,
    const string& parentName) {

    string cleanName = trim(name);
    string cleanParent = trim(parentName);

    if (cleanName.empty()) {
        cout << "Error: Department name cannot be empty.\n";
        return false;
    }

    if (findNode(cleanName) != nullptr) {
        cout << "Error: Department already exists.\n";
        return false;
    }

    // Create root if tree is empty
    if (root == nullptr) {
        if (!cleanParent.empty()) {
            cout << "Error: Root department cannot have a parent.\n";
            return false;
        }

        root = new Node(cleanName, nullptr);

        cout << "Root department added successfully.\n";
        return true;
    }

    // A parent must be provided
    if (cleanParent.empty()) {
        cout << "Error: Please provide an existing parent department.\n";
        return false;
    }

    Node* parentNode = findNode(cleanParent);

    if (parentNode == nullptr) {
        cout << "Error: Parent department not found.\n";
        return false;
    }

    Node* newNode = new Node(cleanName, parentNode);
    parentNode->children.push_back(newNode);

    cout << "Department added successfully.\n";
    return true;
}

// Load departments from a file
bool DepartmentTree::loadDepartments(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return false;
    }

    string line;
    int lineNumber = 0;
    int loaded = 0;

    while (getline(file, line)) {
        lineNumber++;

        string cleanLine = trim(line);

        // Ignore blank lines and comments
        if (cleanLine.empty() || cleanLine[0] == '#') {
            continue;
        }

        size_t separator = cleanLine.find('|');

        if (separator == string::npos) {
            cerr << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Missing | separator.\n";
            continue;
        }

        string departmentName = trim(cleanLine.substr(0, separator));
        string parentName = trim(cleanLine.substr(separator + 1));

        if (departmentName.empty() || parentName.empty()) {
            cerr << "Warning: " << filename
                 << " line " << lineNumber
                 << ": Missing department or parent name.\n";
            continue;
        }

        // First accepted department must be the root
        if (root == nullptr) {
            if (toLower(parentName) != "none") {
                cerr << "Warning: " << filename
                     << " line " << lineNumber
                     << ": First department must have NONE as parent.\n";
                continue;
            }

            if (addDepartment(departmentName)) {
                loaded++;
            }
        }
        else {
            if (toLower(parentName) == "none") {
                cerr << "Warning: " << filename
                     << " line " << lineNumber
                     << ": Only the root can have NONE as parent.\n";
                continue;
            }

            if (addDepartment(departmentName, parentName)) {
                loaded++;
            }
        }
    }

    file.close();

    cout << loaded << " departments loaded successfully.\n";

    return loaded > 0;
}

// Search for a department
void DepartmentTree::searchDepartment(const string& name) const {
    Node* found = findNode(trim(name));

    if (found == nullptr) {
        cout << "Department not found.\n";
        return;
    }

    cout << "\nDepartment: " << found->name << endl;

    if (found->parent == nullptr) {
        cout << "Parent: None\n";
    }
    else {
        cout << "Parent: " << found->parent->name << endl;
    }

    cout << "Direct children: "
         << found->children.size() << endl;

    cout << "Depth: " << getDepth(found) << endl;
}

// Get the depth of a department
int DepartmentTree::getDepth(Node* node) const {
    int depth = 0;

    while (node != nullptr && node->parent != nullptr) {
        depth++;
        node = node->parent;
    }

    return depth;
}

// Display the full hierarchy
void DepartmentTree::displayPreorder(Node* node, int depth) const {
    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        cout << "    ";
    }

    cout << node->name << endl;

    for (Node* child : node->children) {
        displayPreorder(child, depth + 1);
    }
}

void DepartmentTree::displayHierarchy() const {
    if (root == nullptr) {
        cout << "Department tree is empty.\n";
        return;
    }

    cout << "\nDepartment Hierarchy:\n";
    displayPreorder(root, 0);
}

// Display preorder traversal
void DepartmentTree::displayPreorder() const {
    if (root == nullptr) {
        cout << "Department tree is empty.\n";
        return;
    }

    cout << "Preorder Traversal: ";

    queue<Node*> q;
    q.push(root);

    // Recursive preorder traversal
    cout << endl;
    displayPreorder(root, 0);
}

// Postorder helper
void DepartmentTree::displayPostorder(Node* node) const {
    if (node == nullptr) {
        return;
    }

    for (Node* child : node->children) {
        displayPostorder(child);
    }

    cout << node->name << " ";
}

// Postorder traversal
void DepartmentTree::displayPostorder() const {
    if (root == nullptr) {
        cout << "Department tree is empty.\n";
        return;
    }

    cout << "Postorder Traversal: ";
    displayPostorder(root);
    cout << endl;
}

// Level-order traversal
void DepartmentTree::displayLevelOrder() const {
    if (root == nullptr) {
        cout << "Department tree is empty.\n";
        return;
    }

    cout << "Level-order Traversal: ";

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->name << " ";

        for (Node* child : current->children) {
            q.push(child);
        }
    }

    cout << endl;
}

// Count all departments
int DepartmentTree::countNodes(Node* node) const {
    if (node == nullptr) {
        return 0;
    }

    int count = 1;

    for (Node* child : node->children) {
        count += countNodes(child);
    }

    return count;
}

// Display department statistics
void DepartmentTree::displayStatistics() const {
    if (root == nullptr) {
        cout << "Department tree is empty.\n";
        return;
    }

    int total = countNodes(root);

    cout << "\nDepartment Statistics\n";
    cout << "---------------------\n";
    cout << "Total departments: " << total << endl;
    cout << "Root department: " << root->name << endl;

    cout << "Root direct children: "
         << root->children.size() << endl;
}
