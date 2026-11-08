#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <string>
#include <vector>

class DepartmentTree {
private:
    struct Node {
        std::string name;
        Node* parent;
        std::vector<Node*> children;

        Node(const std::string& departmentName, Node* parentNode);
    };

    Node* root;

    Node* findNode(const std::string& name) const;
    void deleteTree(Node* node);
    void displayPreorder(Node* node, int depth) const;
    void displayPostorder(Node* node) const;
    void displayLevelOrder() const;
    int countNodes(Node* node) const;
    int getDepth(Node* node) const;
    std::string trim(const std::string& text) const;
    std::string toLower(const std::string& text) const;

public:
    DepartmentTree();
    ~DepartmentTree();

    bool loadDepartments(const std::string& filename);
    bool addDepartment(const std::string& name,
                       const std::string& parentName = "");

    void searchDepartment(const std::string& name) const;
    void displayHierarchy() const;
    void displayPreorder() const;
    void displayPostorder() const;
    void displayLevelOrder() const;
    void displayStatistics() const;
};

#endif
