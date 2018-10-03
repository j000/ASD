#include <ios>
#include <iostream>
#include <vector>

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

	std::vector<int> v;
	int x;

	while (std::cin >> x)
		v.push_back(x);

	my_sort(v.begin(), v.end());

	for (auto e : v)
		std::cout << e << std::endl;
}
