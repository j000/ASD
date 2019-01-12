#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>

#include "PointerIntPair.h"
#include "binaryTree.hpp"

using Node = BinaryTree<int>::Node;
using NodePtr = llvm::PointerIntPair<Node*, 1, bool>;

void add_helper(BinaryTree<int>& tree, int number, int offset)
{
	if (number == 0)
		return;

	if (offset != 7 && offset != 22)
		tree.insert(2 + offset);

	number /= 2;
	add_helper(tree, number, offset + number);
	add_helper(tree, number, offset - number);
}

void add(BinaryTree<int>& tree, int number)
{
	add_helper(tree, number / 2, number / 2);
}

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	BinaryTree<int> tree;
	add(tree, 32);
	tree.prettyPrint();

	{
		std::cout << "Stack: " << std::endl;
		// cast: &tree -> NodePtr*
		// getPointer: NodePtr* -> Node*
		Node* const root = (reinterpret_cast<NodePtr*>(&tree)->getPointer());
		std::stack<Node*, std::vector<Node*>> stack;
		for (auto current{root}; current || !stack.empty();
			 current = current->rightChild()) {
			while (current) {
				stack.push(current);
				current = current->leftChild();
			}

			current = stack.top();
			stack.pop();

			// std::cout << current->value() << std::endl;
			std::cout << current->value() << " ";
		}
		std::cout << std::endl;
	}
	{
		std::cout << "tree.inorder(): " << std::endl;
		tree.inorder([&](int t) { std::cout << t << " "; });
		std::cout << std::endl;
	}
	{
		std::cout << "Node.next(): " << std::endl;
		// cast: &tree -> NodePtr*
		// getPointer: NodePtr* -> Node*
		Node* const root = (reinterpret_cast<NodePtr*>(&tree)->getPointer());
		auto current{root};
		// go to min
		while (current->leftChild()) {
			current = current->leftChild();
		}
		// traverse
		while (current) {
			std::cout << current->value() << " ";
			current = current->next();
		}
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}
