#ifndef SORTING2_HPP
#define SORTING2_HPP

#include "Heap.hpp"

#include <algorithm>

template <class Iterator>
inline void heap_sort_iter(Iterator begin, Iterator end)
{
	using Type = typename std::remove_reference<decltype(*begin)>::type;
	auto size = end - begin;
	Heap<Type>::sortIter(begin, size);
}

template <typename Iterator>
inline void heap_sort_rec(Iterator begin, Iterator end)
{
	using Type = typename std::remove_reference<decltype(*begin)>::type;
	auto size = end - begin;
	Heap<Type>::sortRec(begin, size);
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
inline void quick_sort(Iterator begin, Iterator end)
{
	if (std::distance(begin, end) < 2)
		return;

	auto pivot = helper::partition(begin, end);
	quick_sort(begin, pivot);
	quick_sort(pivot, end);
}

#endif /* SORTING2_HPP */
