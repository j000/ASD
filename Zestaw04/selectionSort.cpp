#include "Vector.hpp"

#include <algorithm>
#include <iostream>

template <class Iterator>
inline void my_sort(Iterator begin, Iterator end)
{
	// for (; begin != end; ++begin)
	// 	std::iter_swap(begin, std::min_element(begin, end));
	// return;
	for (; begin != end; ++begin) {
		auto min = begin;
		for (auto j = begin + 1; j != end; ++j) {
			if (*j < *min)
				min = j;
		}
		std::iter_swap(begin, min);
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
