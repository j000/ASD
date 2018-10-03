#include <ios>
#include <iostream>
#include <vector>

template <class Iterator>
inline void my_sort(Iterator begin, Iterator end)
{
	typedef decltype(*begin + 1) Base;
	std::vector<Base> histogram(*begin + 1);

	for (auto i = begin; i != end; ++i) {
		// if (histogram.capacity() < (*i + 1)) { // comparing int and unsigned
		auto old_size = histogram.capacity();
		if (old_size < static_cast<decltype(old_size)>(*i + 1)) {
			auto new_size = *i + 1;
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

	std::vector<Base> output(end - begin);
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

	std::vector<int> v;
	int x;

	while (std::cin >> x)
		v.push_back(x);

	my_sort(v.begin(), v.end());

	for (auto e : v)
		std::cout << e << std::endl;
}
