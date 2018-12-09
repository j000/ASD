#include "Heap.hpp"
#include "Vector.hpp"
#include "sorting.hpp"
#include "sorting2.hpp"

#include <iostream>

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	Vector<int> data;
	int x;

	while (std::cin >> x)
		data.push_back(x);

	heap_sort_rec(data.begin(), data.end());

	for (auto e : data)
		std::cout << e << std::endl;

	return 0;
}
