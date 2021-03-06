#ifndef SORTING2_HPP
#define SORTING2_HPP

#include "Vector.hpp"
#include "sorting.hpp"

#include <algorithm>

template <typename Iterator>
inline void c_qsort(Iterator begin, Iterator end)
{
	using Type = typename std::remove_reference<decltype(*begin)>::type;
	auto compare = [](const void* arg1, const void* arg2) -> int {
		Type lhs = *static_cast<const Type*>(arg1);
		Type rhs = *static_cast<const Type*>(arg2);
		return -(lhs < rhs) + (lhs > rhs);
	};
	qsort(begin, end - begin, sizeof(Type), compare);
}

namespace helper {
template <typename Iterator>
inline Iterator partition(Iterator begin, Iterator end)
{
	using std::swap;
	using Type = typename std::remove_reference<decltype(*begin)>::type;

	std::advance(end, -1);
	/* pick pivot */
	auto mid = std::next(begin, std::distance(begin, end) / 2);
	if (*mid < *begin)
		swap(*begin, *mid);
	if (*end < *begin)
		swap(*begin, *end);
	if (*mid < *end)
		swap(*mid, *end);
	Type pivot = *mid;
	/* partition */
	auto i = begin;
	auto j = end;
	while (true) {
		while (*i < pivot)
			++i;
		while (*j > pivot)
			--j;

		if (std::distance(i, j) <= 0)
			return std::next(j);

		swap(*i, *j);
		++i;
		--j;
	}
}
}

template <typename Iterator>
inline void quick_sort_simple(Iterator begin, Iterator end)
{
	if (std::distance(begin, end) < 2)
		return;

	auto pivot = helper::partition(begin, end);
	quick_sort_simple(begin, pivot);
	quick_sort_simple(pivot, end);
}

namespace helper {
template <typename Iterator>
inline void quick_sort(Iterator begin, Iterator end, unsigned limit = 16)
{
	if (std::distance(begin, end) < 2)
		return;

	if (std::distance(begin, end) < 16 || limit == 0) {
		insertion_sort(begin, end);
		return;
	}

	auto pivot = helper::partition(begin, end);
	quick_sort(begin, pivot, limit - 1);
	quick_sort(pivot, end, limit - 1);
}
}

template <typename Iterator>
inline void quick_sort(Iterator begin, Iterator end)
{
	helper::quick_sort(begin, end, 16);
}

namespace helper {
template <typename Iterator>
inline void merge(const Iterator left, const Iterator right, const Iterator end)
{
	using std::swap;
	using Type = typename std::remove_reference<decltype(*left)>::type;

	Vector<Type> tmp(right - left);
	std::copy(left, right, tmp.begin());

	auto i = tmp.begin();
	const auto end2 = tmp.end();
	auto out = left;
	auto j = right;
	while (i != end2 && j != end) {
		if (*i < *j) {
			*out = *i;
			++i;
		} else {
			*out = *j;
			++j;
		}
		++out;
	}
	while (i != end2) {
		*out = *i;
		++i;
		++out;
	}
}
}

template <typename Iterator>
inline void merge_sort(const Iterator begin, const Iterator end)
{
	auto size = std::distance(begin, end);
	if (size < 2)
		return;

	for (auto krok = 1u; krok < size; krok *= 2) {
		for (auto start = begin; std::distance(start, end) >= 0;
			 start += 2 * krok) {
			if (std::distance(start + krok, end) > 0) {
				if (std::distance(start, end) < 2 * krok) {
					helper::merge(start, start + krok, end);
				} else {
					helper::merge(start, start + krok, start + 2 * krok);
				}
			}
		}
	}
}

#endif /* SORTING2_HPP */
