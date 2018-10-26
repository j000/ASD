#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <stdexcept>

using ::std::move;
using ::std::size_t;

using position_t = size_t;

template <typename T = int, size_t N = 1000000>
class ArrayList {
public:
	void push_back(T&&);
	void push_back(const T&);
	T pop_back();

	void push_front(T&&);
	void push_front(const T&);
	T pop_front();

	position_t find(const T&) const;
	position_t erase(const position_t);
	position_t insert(const position_t, const T&);
	position_t insert(const position_t, T&&);

	inline int size() const noexcept;
	inline bool empty() const noexcept;

	// ArrayList();
	~ArrayList();
	// ArrayList(const ArrayList&);
	// ArrayList& operator=(const ArrayList&);
	// ArrayList(ArrayList&&) noexcept;
	// ArrayList& operator=(ArrayList&&) noexcept;

private:
	size_t m_empty {0};
	typename std::aligned_storage<sizeof(T), alignof(T)>::type m_bad_name[N] {};
	T* m_array {reinterpret_cast<T*>(m_bad_name)};
};

template <typename T, size_t N>
ArrayList<T, N>::~ArrayList()
{
	for (auto i = 0u; i != m_empty; ++i) {
		(&m_array[i])->~T();
	}
}

template <typename T, size_t N>
void ArrayList<T, N>::push_back(T&& x)
{
	if (m_empty == N)
		throw ::std::out_of_range("List is full");

	new (&m_array[m_empty]) T {move(x)};
	m_empty++;
}

template <typename T, size_t N>
void ArrayList<T, N>::push_back(const T& x)
{
	if (m_empty == N)
		throw ::std::out_of_range {"List is full"};

	new (&m_array[m_empty]) T {x};
	m_empty++;
}

template <typename T, size_t N>
T ArrayList<T, N>::pop_back()
{
	if (m_empty == 0)
		throw ::std::out_of_range("List is empty");

	--m_empty;
	T out {move(m_array[m_empty])};
	(&m_array[m_empty])->~T();
	return move(out);
}

template <typename T, size_t N>
void ArrayList<T, N>::push_front(T&& x)
{
	if (m_empty == N)
		throw ::std::out_of_range("List is full");

	::std::memmove(m_array + 1, m_array, m_empty * sizeof(T));
	++m_empty;
	m_array[0] = move(x);
}

template <typename T, size_t N>
void ArrayList<T, N>::push_front(const T& x)
{
	if (m_empty == N)
		throw ::std::out_of_range("List is full");

	::std::memmove(m_array + 1, m_array, m_empty * sizeof(T));
	++m_empty;
	new (&m_array[0]) T(x);
}

template <typename T, size_t N>
T ArrayList<T, N>::pop_front()
{
	if (m_empty == 0)
		throw ::std::out_of_range("List is empty");

	auto out(move(m_array[0]));
	--m_empty;
	::std::memmove(m_array, m_array + 1, m_empty * sizeof(T));
	return move(out);
}

template <typename T, size_t N>
position_t ArrayList<T, N>::find(const T& x) const
{
	for (auto i = 0; i < size(); ++i)
		if (x == m_array[i])
			return i;
	return size();
}

template <typename T, size_t N>
position_t ArrayList<T, N>::insert(const position_t pos, T&& x)
{
	if (pos < 0 || pos > size())
		throw ::std::out_of_range("Position out of range");

	::std::memmove(
		m_array + pos + 1, m_array + pos, (m_empty - pos) * sizeof(T));
	++m_empty;
	m_array[pos] = move(x);

	return pos;
}

template <typename T, size_t N>
position_t ArrayList<T, N>::insert(const position_t pos, const T& x)
{
	if (pos < 0 || pos > size())
		throw ::std::out_of_range("Position out of range");

	::std::memmove(
		m_array + pos + 1, m_array + pos, (m_empty - pos) * sizeof(T));
	++m_empty;
	// m_array[pos] = x;
	new (&m_array[pos]) T(x);

	return pos;
}

template <typename T, size_t N>
int ArrayList<T, N>::size() const noexcept
{
	return m_empty;
}

template <typename T, size_t N>
bool ArrayList<T, N>::empty() const noexcept
{
	return m_empty == 0;
}

#endif /* ARRAYLIST_HPP */
