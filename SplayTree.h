#pragma once
#include <array>

// Self-Adjusting Binary Search (Splay) Tree
// Amiani Johns
template <typename T>
class SplayTree
{	
    struct Node
    {
        Node() { children.fill(nullptr); }
        Node(int _key, T _data, Node* _parent) 
            : key(_key), data(_data), parent(_parent) 
            { children.fill(nullptr); }
        Node& operator=(Node _node);
    
        int key;
        T data;
        std::array<Node*, 2> children;
        Node* parent;
    };

public:
    SplayTree() : root(nullptr) {}
    SplayTree(const SplayTree &_tree);
    SplayTree& operator=(const SplayTree &_tree);
    ~SplayTree();

    T* find(const int &key);
    void insert(const int &key, const T &data);
    void remove(const int &key);
    void displayTree() const;

private:
    void splay(Node* target);
    void rotate(Node* target, const int &targetside);
    void deleteTree(Node* node);

    Node* root;
};

#include "SplayTree.ipp"