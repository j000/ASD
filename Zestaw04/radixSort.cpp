#include "Vector.hpp"
#include "sorting.hpp"

#include <iostream>

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	Vector<unsigned> data;
	unsigned x;

	while (std::cin >> x)
		data.push_back(x);

	radix_sort(data.begin(), data.end());

	for (auto e : data)
		std::cout << e << std::endl;

	return 0;
}
