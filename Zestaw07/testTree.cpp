#include <cstdlib>
#include <iostream>

#include "binaryTree.hpp"

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	BinaryTree<int> tree;

	int n;
	std::cin >> n;
	while (n > 0) {
		int tmp;
		std::cin >> tmp;
		tree.insert(tmp);
		--n;
	}

	tree.inorder([](int v) { std::cout << v << std::endl; });

	std::cout << tree.size() << " " << tree.depth() << " " << tree.minimum()
			  << " " << tree.maximum() << std::endl;

	for (int i = 1; i < 10; ++i) {
		// if (tree.searchRecursive(i))
		if (tree.search(i))
			std::cout << "Yes";
		else
			std::cout << "No";
		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}
