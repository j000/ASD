#include <algorithm>
#include <ios>
#include <iostream>
#include <vector>

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

	std::vector<int> v;
	int x;

	while (std::cin >> x)
		v.push_back(x);

	my_sort(v.begin(), v.end());

	for (auto e : v)
		std::cout << e << std::endl;
}
