#pragma once
#include <array>

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

	T* Find(const int &key);
	void Insert(const int &key, const T &data);
	void Delete(const int &key);

private:
	void Splay(Node* target);
	void Rotate(Node* target, const int &targetside);
	void DeleteTree(Node* node);
public:
	void DisplayTree(const Node* node) const;

	Node* root;
};

#include "SplayTree.ipp"