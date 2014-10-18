#include <functional>
#include <vector>
#include <iostream>
#include <utility>
#include "SplayTree.h"

template <typename T>
typename SplayTree<T>::Node& SplayTree<T>::Node::operator=(Node _node) // Not working :(
{
	key = _node.key;
	data = _node.data;

	std::swap(*children[0], *_node.children[0]);
	std::swap(*children[1], *_node.children[1]);

	return *this;
}

template <typename T>
SplayTree<T>::SplayTree(const SplayTree &_tree)
{
	root = new Node;
	*root = *_tree.root;
}

template <typename T>
SplayTree<T>& SplayTree<T>::operator=(const SplayTree &_tree)
{
	DeleteTree(root);
	root = new Node;
	*root = *_tree.root;
}

template <typename T>
T* SplayTree<T>::Find(const int &key)
{
	std::function<Node*(Node*)> FSearch = [&](Node* node) {
		if (node->key == key) {
			Splay(node);
			return node;
		}
		
		unsigned side = node->key < key;
		if (node->children[side]) return FSearch(node->children[side]);
		else {
			Splay(node);
			return nullptr;
		}
	};

	Node node = FSearch(root);
	if (node) return &node->data;
	else return nullptr;
}

template <typename T>
void SplayTree<T>::Insert(const int &key, const T &data)
{
	if (!root) {
		root = new Node(key, data, nullptr);
		return;
	}

	std::function<Node*(Node*)> ISearch = [&](Node* node) {
		unsigned side = node->key < key;
		if (!node->children[side]) {
			return node->children[side] = new Node(key, data, node);
		}
		else ISearch(node->children[side]);
	};

	Splay(ISearch(root));
}

template <typename T>
void SplayTree<T>::Delete(const int &key)
{
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

		Splay(parent);
	}
	else Splay(predecessor);
}

template <typename T>
void SplayTree<T>::Splay(Node* target)
{
	if (target == root) return;

	while (target->parent) {
		Node* parent = target->parent;
		Node* grandparent = parent->parent;

		int targetside = parent->children[1] == target;

		if (grandparent) {
			int parentside = grandparent->children[1] == parent;

			if (parentside == targetside) {     // Zig-Zig case
				Rotate(parent, parentside);
				Rotate(target, targetside);
			} else {                              // Zig-Zag case
				Rotate(target, targetside);
				Rotate(target, parentside);
			}
		} else Rotate(target, targetside);                             // Child of root case
			
	}

	root = target;	
}

template <typename T>
void SplayTree<T>::Rotate(Node* target, const int &targetside)
{
	
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
SplayTree<T>::~SplayTree()
{
	if (root) DeleteTree(root);
}

template <typename T>
void SplayTree<T>::DeleteTree(Node* node)
{
	for (Node* child : node->children)
		if (child) DeleteTree(child);

	delete node;
	node = nullptr;
}

template <typename T>
void SplayTree<T>::DisplayTree(const Node* startnode) const
{
	int level = 0;

	std::vector<Node*> nodes;
	nodes.push_back(const_cast<Node*>(startnode));

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