#include <functional>
#include <vector>
#include <iostream>
#include <utility>
#include "SplayTree.h"

template <typename T>
SplayTree<T>::SplayTree(const SplayTree &tree) {
    root = new Node;
    deepCopy(root, tree.root, nullptr);
}

template <typename T>
void SplayTree<T>::deepCopy(Node* thisnode, Node* thatnode, Node* parent) const {
    if (thatnode) {
        *thisnode = *thatnode;
        thisnode->parent = parent;
        for (int i = 0; i != 2; i++)
            deepCopy(thisnode->children[i], thatnode->children[i], thisnode);
    }
}

template <typename T>
SplayTree<T>& SplayTree<T>::operator=(const SplayTree &tree) {
    deleteTree(root);
    root = new Node;
    root = tree.root;
}

template <typename T>
T* SplayTree<T>::find(const int &key) {
    std::function<Node*(Node*)> fSearch = [&](Node* node) {
        if (node->key == key) {
            splay(node);
            return node;
        }
        
        unsigned side = node->key < key;
        if (node->children[side])
            return fSearch(node->children[side]);
        else {
            splay(node);
            return nullptr;
        }
    };

    Node node = fSearch(root);
    if (node) return &node->data;
    else return nullptr;
}

template <typename T>
void SplayTree<T>::insert(const int &key, const T &data) {
    if (!root) {
        root = new Node(key, data, nullptr);
        return;
    }

    std::function<Node*(Node*)> iSearch = [&](Node* node) {
        unsigned side = node->key < key;
        if (!node->children[side])
            return node->children[side] = new Node(key, data, node);
        else
            iSearch(node->children[side]);
    };

    splay(iSearch(root));
}

template <typename T>
void SplayTree<T>::remove(const int &key) {
    if (!root) return;

    Node head;
    head.children[1] = root; // Avoids dealing with special case when root is the target node
    unsigned side = 1;

    Node *target = nullptr, *predecessor = &head;

    while (predecessor->children[side]) {
        predecessor = predecessor->children[side];
        if (predecessor->key == key) target = predecessor;
        side = key > predecessor->key;
    }

    if (target) {                                   // true when key was found
        Node *parent = target->parent;

        if (target != predecessor) {                // true when inorder predecessor was found
            target->key = predecessor->key;
            target->data = predecessor->data;

            Node *predparent = predecessor->parent;
            if (predparent != target) {
                predparent->children[1] = predecessor->children[0];
            } else target->children[0] = nullptr;

            delete predecessor;
        } else {
            if (target->children[1]) {
                target->children[1]->parent = parent;
            }
            target = target->children[1];
        }

        splay(parent);
    }
    else
        splay(predecessor);
}

template <typename T>
void SplayTree<T>::splay(Node* target) {
    if (target == root) return;

    while (target->parent) {
        Node* parent = target->parent;
        Node* grandparent = parent->parent;
        int targetside = parent->children[1] == target;

        if (grandparent) {
            int parentside = grandparent->children[1] == parent;

            if (parentside == targetside) {     // Zig-Zig case
                rotate(parent, parentside);
                rotate(target, targetside);
            } else {                            // Zig-Zag case
                rotate(target, targetside);
                rotate(target, parentside);
            }
        } else
            rotate(target, targetside);         // Child of root case            
    }

    root = target;	
}

template <typename T>
void SplayTree<T>::rotate(Node* target, const int &targetside) {	
    Node *parent = target->parent;
    Node *grandparent = parent->parent;
    Node *transchild = target->children[!targetside]; // child to be transferred to target's parent

    if (grandparent) {
        int parentside = grandparent->children[1] == parent;
        grandparent->children[parentside] = target;
    }

    parent->parent = target;
    parent->children[targetside] = transchild;
    if (transchild) transchild->parent = parent;	
    target->children[!targetside] = parent;
    target->parent = grandparent;
}

template <typename T>
SplayTree<T>::~SplayTree() {
    std::cout << "rootkey: " << root->key << std::endl;
    if (root) deleteTree(root);

}

//recursively deletes the tree
template <typename T>
void SplayTree<T>::deleteTree(Node* node) {
    std::cout << node->key << std::endl;
        for (Node* child : node->children) {
            if (child) {
                deleteTree(child);
            }
        }

        delete node;
        node = nullptr;
}

template <typename T>
void SplayTree<T>::displayTree() const {
    int level = 0;

    std::vector<Node*> nodes;
    nodes.push_back(root);

    while (!nodes.empty()) {
        std::cout << "Level" << level << ": ";
        for (Node *node : nodes) {
            std::cout << node->key << "p";
            if (node->parent) std::cout << node->parent->key << " ";
        }
        std::cout << std::endl;

        int size = nodes.size();
        for (int i = 0; i != size; ++i) {
            for (Node *child : (*nodes.begin())->children)
                if (child) nodes.push_back(child);

            nodes.erase(nodes.begin());
        }

        ++level;
    }

    std::cout << std::endl;
}