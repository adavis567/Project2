#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

class DepartmentTree {
private:
    struct Node {
        std::string name;
        Node* parent;
        std::vector<Node*> children;
        Node(const std::string& value, Node* parentNode);
    };

    Node* root;
    Node* findNode(const std::string& name) const;
    void deleteTree(Node* node);
    void preorder(Node* node, int depth) const;
    void postorder(Node* node) const;
    int countNodes(Node* node) const;
    int height(Node* node) const;
    int getDepth(Node* node) const;

public:
    DepartmentTree();
    ~DepartmentTree();
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    bool loadDepartments(const std::string& filename);
    bool addDepartment(const std::string& name, const std::string& parentName = "");
    void searchDepartment(const std::string& name) const;
    void displayHierarchy() const;
    void displayPreorder() const;
    void displayPostorder() const;
    void displayLevelOrder() const;
    void displayStatistics() const;
};

#endif
