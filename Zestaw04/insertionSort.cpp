#include "Vector.hpp"

#include <iostream>

template <class Iterator>
inline void my_sort(Iterator begin, Iterator end)
{
	for (auto i = begin + 1; i != end; ++i) {
		auto value = *i;
		auto j = i - 1;
		while (value < *j) {
			*(j + 1) = *j;
			--j;
		}
		*(j + 1) = value;
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
