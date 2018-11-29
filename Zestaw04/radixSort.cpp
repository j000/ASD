#include "Vector.hpp"

#include <algorithm>
#include <iostream>

template <class Iterator>
inline void my_sort(Iterator begin, Iterator end)
{
	// constexpr unsigned N{4};
	// constexpr unsigned base{1 << N}; // 2**N
	constexpr unsigned base{16};
	unsigned pow = 1;
	// unsigned iteration = 0;
	auto maximum = *std::max_element(begin, end);

	while (pow <= maximum) {
	// while ((1 << iteration) <= maximum) {
		Vector<Vector<int>> buckets(base, Vector<int>());
		for (auto i = begin; i != end; ++i) {
			auto digit = *i / pow % base;
			// auto digit = (*i >> iteration) & (base - 1);
			buckets[digit].push_back(*i);
		}
		auto i = begin;
		for (auto tmp : buckets) {
			for (auto j : tmp) {
				*i = j;
				++i;
			}
		}
		pow *= base;
		// iteration += N;
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
