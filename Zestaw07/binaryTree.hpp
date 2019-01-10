#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

template <typename T>
class BinaryTree {
public:
	class Node;

	~BinaryTree();

	void insert(const T&);
	void insert(T&&);
	const T* search(const T&) const noexcept;
	const T* searchRecursive(const T&) const noexcept;
	void prettyPrint() const noexcept;
	void prettyPrint(Node*, std::string, const std::string&) const noexcept;

	void inorder(const std::function<void(const T&)>&) const noexcept;
	void preorder(const std::function<void(const T&)>&) const noexcept;
	void postorder(const std::function<void(const T&)>&) const noexcept;

	const T& minimum() const;
	const T& maximum() const;
	unsigned size() const noexcept;
	unsigned depth() const noexcept;

private:
	void insert(Node*&, Node*, unsigned = 1) noexcept;
	const T* searchRecursive(const Node*, const T&) const noexcept;
	void inorder(const Node*, const std::function<void(const T&)>&) const
		noexcept;
	void preorder(const Node*, const std::function<void(const T&)>&) const
		noexcept;
	void postorder(const Node*, const std::function<void(const T&)>&) const
		noexcept;
	static void deleter(Node*);

	Node* root{nullptr};
	unsigned m_size{0};
	unsigned m_depth{0};
};

////////////////////////////////////////

template <typename T>
class BinaryTree<T>::Node {
public:
	Node() = default;
	Node(const T _value) : value{_value}
	{
	}
	~Node() = default;
	Node(const Node&) = default;
	Node& operator=(const Node&) = default;
	Node(Node&&) = default;
	Node& operator=(Node&&) = default;

	T value{};
	Node* leftChild{nullptr};
	Node* rightChild{nullptr};
};

////////////////////////////////////////

template <typename T>
BinaryTree<T>::~BinaryTree()
{
	deleter(root);
}

template <typename T>
void BinaryTree<T>::deleter(Node* node)
{
	if (!node)
		return;
	deleter(node->leftChild);
	deleter(node->rightChild);
	delete node;
}

template <typename T>
void BinaryTree<T>::insert(const T& x)
{
	auto tmp{new Node(x)};
	insert(root, tmp);
}

template <typename T>
void BinaryTree<T>::insert(T&& x)
{
	auto tmp{new Node(std::forward<T>(x))};
	insert(root, tmp);
}

template <typename T>
const T* BinaryTree<T>::search(const T& x) const noexcept
{
	auto tmp{root};
	while (tmp != nullptr) {
		if (tmp->value == x)
			return &tmp->value;
		if (tmp->value > x)
			tmp = tmp->leftChild;
		else
			tmp = tmp->rightChild;
	}

	return nullptr;
}

template <typename T>
inline const T* BinaryTree<T>::searchRecursive(const T& x) const noexcept
{
	return searchRecursive(root, x);
}

template <typename T>
const T* BinaryTree<T>::searchRecursive(const Node* node, const T& x) const
	noexcept
{
	if (!node)
		return nullptr;
	if (node->value == x)
		return &node->value;
	if (node->value > x)
		return searchRecursive(node->leftChild, x);
	return searchRecursive(node->rightChild, x);
}

template <typename T>
void BinaryTree<T>::prettyPrint() const noexcept
{
	std::string tmp{""};
	prettyPrint(root, "", tmp);
}

template <typename T>
void BinaryTree<T>::prettyPrint(
	Node* node, std::string sp, const std::string& sn) const noexcept
{
#if 1
	static const std::string cc{"    "};
	static const std::string cr{"┌───"};
	static const std::string cp{"│   "};
	static const std::string cl{"└───"};
#else
	static const std::string cr{".-"};
	static const std::string cp{"| "};
	static const std::string cl{"'-"};
#endif

	if (!node)
		return;

	std::string s;

	s = sp;
	if (&sn == &cr) {
		s = s.substr(0, s.length() - cp.length());
		s += cc;
	}
	prettyPrint(node->rightChild, s + cp, cr);

	s = sp.substr(0, sp.length() - cp.length());
	std::cout << "\033[2;34m" << s << sn << "\033[22;39m" << node->value << std::endl;

	s = sp;
	if (sn == cl) {
		s = s.substr(0, s.length() - cp.length());
		s += cc;
	}
	prettyPrint(node->leftChild, s + cp, cl);
}

template <typename T>
inline void BinaryTree<T>::inorder(const std::function<void(const T&)>& f) const
	noexcept
{
	inorder(root, f);
}

template <typename T>
void BinaryTree<T>::inorder(
	const Node* node, const std::function<void(const T&)>& f) const noexcept
{
	if (!node)
		return;
	inorder(node->leftChild, f);
	f(node->value);
	inorder(node->rightChild, f);
}

template <typename T>
inline void
BinaryTree<T>::preorder(const std::function<void(const T&)>& f) const noexcept
{
	preorder(root, f);
}

template <typename T>
void BinaryTree<T>::preorder(
	const Node* node, const std::function<void(const T&)>& f) const noexcept
{
	if (!node)
		return;
	f(node->value);
	preorder(node->leftChild, f);
	preorder(node->rightChild, f);
}

template <typename T>
inline void
BinaryTree<T>::postorder(const std::function<void(const T&)>& f) const noexcept
{
	postorder(root, f);
}

template <typename T>
void BinaryTree<T>::postorder(
	const Node* node, const std::function<void(const T&)>& f) const noexcept
{
	if (!node)
		return;
	postorder(node->leftChild, f);
	postorder(node->rightChild, f);
	f(node->value);
}

template <typename T>
const T& BinaryTree<T>::minimum() const
{
	if (!root)
		throw std::out_of_range("Drzewo puste");

	auto tmp{root};

	while (tmp->leftChild)
		tmp = tmp->leftChild;

	return tmp->value;
}

template <typename T>
const T& BinaryTree<T>::maximum() const
{
	if (!root)
		throw std::out_of_range("Drzewo puste");

	auto tmp{root};

	while (tmp->rightChild)
		tmp = tmp->rightChild;

	return tmp->value;
}

template <typename T>
inline unsigned BinaryTree<T>::size() const noexcept
{
	return m_size;
}

template <typename T>
inline unsigned BinaryTree<T>::depth() const noexcept
{
	return m_depth;
}

template <typename T>
void BinaryTree<T>::insert(
	Node*& parent, Node* newNode, unsigned level) noexcept
{
	if (parent == nullptr) {
		++m_size;
		if (level > m_depth)
			m_depth = level;
		parent = newNode;
		return;
	}
	if (parent->value > newNode->value) {
		insert(parent->leftChild, newNode, level + 1);
		return;
	}
	insert(parent->rightChild, newNode, level + 1);
}

////////////////////////////////////////

#endif /* BINARYTREE_HPP */
