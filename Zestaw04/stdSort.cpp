#include "Vector.hpp"

#include <algorithm>
#include <iostream>

template <class Iterator>
inline void my_sort(Iterator begin, Iterator end)
{
	std::sort(begin, end);
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
