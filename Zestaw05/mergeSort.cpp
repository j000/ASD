#include "Vector.hpp"

#include <algorithm>
#include <iostream>

void merge(unsigned array[], unsigned start1, unsigned start2, unsigned end)
{
	auto diff = start2 - start1;
	Vector<unsigned> tmp(start2 - start1);
	// kopiujemy
	for (auto i = 0u; i < diff; ++i) {
		tmp[i] = array[start1 + i];
	}
	// łączenie
	auto out = start1;
	auto i = 0u;
	auto j = start2;
	while (i < diff && j < end) {
		if (tmp[i] < array[j]) {
			array[out] = tmp[i];
			++i;
		} else {
			array[out] = array[j];
			++j;
		}
		++out;
	}
	while (i < diff) {
		array[out] = tmp[i];
		++i;
		++out;
	}
}

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	Vector<unsigned> data;
	unsigned x;

	while (std::cin >> x)
		data.push_back(x);

	////////////////////

	for (auto krok = 1u; krok < data.size(); krok *= 2) {
		for (auto start = 0u; start < data.size(); start += 2 * krok) {
			if (start + krok < data.size())
				merge(
					&data[0],
					start,
					start + krok,
					std::min<unsigned>(start + 2 * krok, data.size()));
		}
	}

	////////////////////

	for (auto e : data)
		std::cout << e << std::endl;

	return 0;
}
