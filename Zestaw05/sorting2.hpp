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

#endif /* SORTING2_HPP */
