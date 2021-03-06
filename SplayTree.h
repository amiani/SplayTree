#pragma once
#include <array>

// Self-Adjusting Binary (Splay) Tree
// Amiani Johns
template <typename T>
class SplayTree
{	
    struct Node
    {
        Node() { children.fill(nullptr); }
        Node(int key, T data, Node* parent) 
            : key(key), data(data), parent(parent) 
            { children.fill(nullptr); }

        Node& operator=(Node that) {
            key = that.key;
            data = that.data;
            parent = that.parent;
            return *this;
        }

        int key;
        T data;
        std::array<Node*, 2> children;
        Node* parent;
    };

public:
    SplayTree() : root(nullptr) {}
    SplayTree(const SplayTree &tree);
    SplayTree& operator=(const SplayTree &tree);
    ~SplayTree();

    T* find(const int &key);
    void insert(const int &key, const T &data);
    void remove(const int &key);
    void displayTree() const;

private:
    void deepCopy(Node* thisnode, Node* node, Node* parent) const;
    void splay(Node* target);
    void rotate(Node* target, const int &targetside);
    void deleteTree(Node* node);

    Node* root;
    int counter;
};

#include "SplayTree.ipp"