#ifndef SORTEDARRAYLIST_HPP
#define SORTEDARRAYLIST_HPP

#include "Vector.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>

template <typename T>
class SortedArrayList;

template <typename T>
SortedArrayList<T> merge(const SortedArrayList<T>&, const SortedArrayList<T>&);

template <typename T>
class SortedArrayList {
public:
	std::size_t size() const;
	bool empty() const;
	void push(T);
	T pop();
	T erase(int);
	int find(T) const;
	void remove(T);
	SortedArrayList& unique();
	void print() const;

	friend SortedArrayList<T>
	merge<T>(const SortedArrayList<T>&, const SortedArrayList<T>&);

private:
	Vector<T> m_array{};
};

template <typename T>
std::size_t SortedArrayList<T>::size() const
{
	return m_array.size();
}

template <typename T>
bool SortedArrayList<T>::empty() const
{
	return m_array.empty();
}

template <typename T>
void SortedArrayList<T>::push(T x)
{
	auto i = std::lower_bound(m_array.begin(), m_array.end(), x);
	m_array.insert(i, x);
}

template <typename T>
T SortedArrayList<T>::pop()
{
	int tmp = m_array.front();
	m_array.erase(m_array.begin());
	return tmp;
}

template <typename T>
int SortedArrayList<T>::find(T x) const
{
	auto i = std::lower_bound(m_array.begin(), m_array.end(), x);
	if (*i == x)
		return i - m_array.begin();
	return -1;
}

template <typename T>
T SortedArrayList<T>::erase(int i)
{
	auto pos = m_array.begin() + i;
	T out = *pos;
	m_array.erase(pos);
	return out;
}

template <typename T>
void SortedArrayList<T>::remove(T x)
{
	auto i = std::lower_bound(m_array.begin(), m_array.end(), x);
	if (*i != x)
		return;
	auto start = i;
	while (*i == x) {
		++i;
	}
	m_array.erase(start, i);
}

template <typename T>
SortedArrayList<T>& SortedArrayList<T>::unique()
{
	// TODO: do it better
	// for (auto i = m_array.begin() + 1; i != m_array.end(); ++i) {
	// 	if (*(i - 1) == *(i)) {
	// 		m_array.erase(i);
	// 		--i;
	// 	}
	// }
	// return *this;
	if (m_array.empty())
		return *this;

	Vector<T> out{m_array.front()};
	auto i = m_array.begin() + 1;
	while (i != m_array.end()) {
		if (out.back() != *i)
			out.push_back(*i);
		++i;
	}
	m_array.swap(out);
	return *this;
}

template <typename T>
void SortedArrayList<T>::print() const
{
	for (int n : m_array) {
		std::cout << n << std::endl;
	}
}

template <typename T>
SortedArrayList<T>
merge(const SortedArrayList<T>& a, const SortedArrayList<T>& b)
{
	SortedArrayList<T> out;
	auto i = a.m_array.begin();
	auto j = b.m_array.begin();
	out.m_array.reserve(a.size() + b.size());
	while (i != a.m_array.end() && j != b.m_array.end()) {
		if (*i < *j) {
			out.push(*i);
			++i;
		} else {
			out.push(*j);
			++j;
		}
	}
	while (i != a.m_array.end()) {
		out.push(*i);
		++i;
	}
	while (j != b.m_array.end()) {
		out.push(*j);
		++j;
	}
	return out;
}

#endif /* SORTEDARRAYLIST_HPP */
