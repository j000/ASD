#ifndef SORTING_HPP
#define SORTING_HPP

#include "Vector.hpp"

#include <algorithm>

template <typename Iterator>
inline void bubble_sort(Iterator begin, Iterator end)
{
	for (; begin != end; ++begin)
		for (auto j = end - 1; j != begin; --j) {
			if (*(j - 1) > *j)
				std::iter_swap(j - 1, j);
		}
}

template <typename Iterator>
inline void counting_sort(Iterator begin, Iterator end)
{
	using Type = typename std::remove_reference<decltype(*begin)>::type;

	Vector<Type> histogram(*begin + 1);
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
	Vector<Type> output(end - begin);
	for (auto i = begin; i != end; ++i) {
		output[histogram[*i]] = *i;
		histogram[*i] += 1;
	}
	for (auto i = begin, j = output.begin(); i != end; ++i, ++j)
		*i = *j;
}

template <typename Iterator>
inline void insertion_sort(Iterator begin, Iterator end)
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

template <typename Iterator>
inline void radix_sort(Iterator begin, Iterator end)
{
	using Type = typename std::remove_reference<decltype(*begin)>::type;

	constexpr unsigned base{16};
	decltype(1 + *begin) pow = 1;
	auto maximum = *std::max_element(begin, end);

	while (pow && pow <= maximum) {
		Vector<Vector<Type>> buckets(base, Vector<Type>());
		for (auto i = begin; i != end; ++i) {
			auto digit = *i / pow % base;
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
	}
	// TODO: handle negative numbers...
	// if (*(end - 1) < 0) {
	// }
}

template <typename Iterator>
inline void selection_sort(Iterator begin, Iterator end)
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

#endif /* SORTING_HPP */
