#include "Vector.hpp"

#include <iostream>

template <class Iterator>
inline void my_sort(Iterator begin, Iterator end)
{
	using Base = decltype(*begin + 1);
	Vector<Base> histogram(*begin + 1);
	for (auto i = begin; i != end; ++i) {
		auto old_size{histogram.size()};
		auto new_size{*i + 1};
		if (old_size < new_size) {
			histogram.reserve(new_size);
			histogram.insert(histogram.end(), new_size - old_size, 0);
		}
		histogram[*i] += 1;
	}
	auto total = 0;
	for (auto& e : histogram) {
		auto old_count = e;
		e = total;
		total += old_count;
	}
	Vector<Base> output(end - begin);
	for (auto i = begin; i != end; ++i) {
		output[histogram[*i]] = *i;
		histogram[*i] += 1;
	}
	for (auto i = begin, j = output.begin(); i != end; ++i, ++j)
		*i = *j;
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
