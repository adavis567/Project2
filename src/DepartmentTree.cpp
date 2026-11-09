#include "DepartmentTree.h"
#include "FileLoader.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

DepartmentTree::Node::Node(const string& value, Node* parentNode)
    : name(value), parent(parentNode) {}

DepartmentTree::DepartmentTree() : root(nullptr) {}
DepartmentTree::~DepartmentTree() { deleteTree(root); }

void DepartmentTree::deleteTree(Node* node) {
    if (!node) return;
    for (Node* child : node->children) deleteTree(child);
    delete node;
}

DepartmentTree::Node* DepartmentTree::findNode(const string& name) const {
    if (!root) return nullptr;
    const string target = FileLoader::lower(FileLoader::trim(name));
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        if (FileLoader::lower(current->name) == target) return current;
        for (Node* child : current->children) q.push(child);
    }
    return nullptr;
}

bool DepartmentTree::addDepartment(const string& name, const string& parentName) {
    string cleanName = FileLoader::trim(name);
    string cleanParent = FileLoader::trim(parentName);
    if (cleanName.empty() || findNode(cleanName)) return false;

    if (!root) {
        if (!cleanParent.empty()) return false;
        root = new Node(cleanName, nullptr);
        return true;
    }

    if (cleanParent.empty()) return false;
    Node* parent = findNode(cleanParent);
    if (!parent) return false;
    parent->children.push_back(new Node(cleanName, parent));
    return true;
}

bool DepartmentTree::loadDepartments(const string& filename) {
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
        if (fields.size() != 2 || fields[0].empty() || fields[1].empty()) {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": expected DepartmentName|ParentName.\n";
            ++rejected;
            continue;
        }

        bool added = false;
        if (!root) {
            if (FileLoader::lower(fields[1]) == "none")
                added = addDepartment(fields[0]);
        } else if (FileLoader::lower(fields[1]) != "none") {
            added = addDepartment(fields[0], fields[1]);
        }

        if (added) ++accepted;
        else {
            cerr << "Warning: " << filename << " line " << lineNumber
                 << ": duplicate department, invalid root, or parent not yet loaded.\n";
            ++rejected;
        }
    }

    cout << "Departments: " << accepted << " accepted, "
         << rejected << " rejected.\n";
    return accepted > 0;
}

void DepartmentTree::searchDepartment(const string& name) const {
    Node* found = findNode(name);
    if (!found) {
        cout << "Department not found.\n";
        return;
    }
    cout << "Department: " << found->name << '\n'
         << "Parent: " << (found->parent ? found->parent->name : "None") << '\n'
         << "Direct children: " << found->children.size() << '\n'
         << "Depth: " << getDepth(found) << '\n';
}

int DepartmentTree::getDepth(Node* node) const {
    int depth = 0;
    while (node && node->parent) {
        ++depth;
        node = node->parent;
    }
    return depth;
}

void DepartmentTree::preorder(Node* node, int depth) const {
    if (!node) return;
    cout << string(static_cast<size_t>(depth) * 2, ' ') << node->name << '\n';
    for (Node* child : node->children) preorder(child, depth + 1);
}

void DepartmentTree::displayHierarchy() const {
    if (!root) { cout << "Department tree is empty.\n"; return; }
    cout << "Department Hierarchy:\n";
    preorder(root, 0);
}

void DepartmentTree::displayPreorder() const {
    if (!root) { cout << "Department tree is empty.\n"; return; }
    cout << "Preorder Traversal:\n";
    preorder(root, 0);
}

void DepartmentTree::postorder(Node* node) const {
    if (!node) return;
    for (Node* child : node->children) postorder(child);
    cout << node->name << '\n';
}

void DepartmentTree::displayPostorder() const {
    if (!root) { cout << "Department tree is empty.\n"; return; }
    cout << "Postorder Traversal:\n";
    postorder(root);
}

void DepartmentTree::displayLevelOrder() const {
    if (!root) { cout << "Department tree is empty.\n"; return; }
    cout << "Level-order Traversal:\n";
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        cout << current->name << '\n';
        for (Node* child : current->children) q.push(child);
    }
}

int DepartmentTree::countNodes(Node* node) const {
    if (!node) return 0;
    int count = 1;
    for (Node* child : node->children) count += countNodes(child);
    return count;
}

int DepartmentTree::height(Node* node) const {
    if (!node) return 0;
    int maximum = 0;
    for (Node* child : node->children)
        maximum = max(maximum, height(child));
    return maximum + 1;
}

void DepartmentTree::displayStatistics() const {
    if (!root) { cout << "Department tree is empty.\n"; return; }
    cout << "Department Tree Statistics\n"
         << "Total departments: " << countNodes(root) << '\n'
         << "Root department: " << root->name << '\n'
         << "Root direct children: " << root->children.size() << '\n'
         << "Tree height (levels): " << height(root) << '\n';
}
