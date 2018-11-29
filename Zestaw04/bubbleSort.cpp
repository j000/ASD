#include "Vector.hpp"

#include <iostream>

template <class Iterator>
inline void my_sort(Iterator begin, Iterator end)
{
	for (; begin != end; ++begin)
		for (auto j = end - 1; j != begin; --j) {
			if (*(j - 1) > *j)
				std::iter_swap(j - 1, j);
		}
}

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	Vector<int> data;
	int x;

	while (std::cin >> x)
		data.push_back(x);

	my_sort(data.begin(), data.end());

	for (auto e : data)
		std::cout << e << std::endl;

	return 0;
}
