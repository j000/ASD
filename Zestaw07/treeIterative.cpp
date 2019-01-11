#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>

#include "binaryTree.hpp"

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
		using Node = BinaryTree<int>::Node;

		Node* root = *reinterpret_cast<Node**>(&tree);
		std::stack<Node*, std::vector<Node*>> stack;
		auto current{root};
		while (current || !stack.empty()) {
			while (current) {
				stack.push(current);
				current = current->leftChild();
			}

			current = stack.top();
			stack.pop();

			std::cout << current->value() << std::endl;

			current = current->rightChild();
		}
	}

	return EXIT_SUCCESS;
}
