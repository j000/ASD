#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <stdexcept>

using std::size_t;

using position_t = size_t;

template <typename T = int, size_t N = 1000000>
class ArrayList {
public:
	void push_back(const T&);
	T pop_back();

	void push_front(const T&);
	T pop_front();

	position_t find(const T&) const;
	position_t erase(const position_t);
	position_t insert(const position_t, const T&);

	inline int size() const noexcept;
	inline bool empty() const noexcept;

	ArrayList() = default;
	~ArrayList();
	ArrayList(const ArrayList&) = delete;
	ArrayList& operator=(const ArrayList&) = delete;
	ArrayList(ArrayList&&) noexcept = delete;
	ArrayList& operator=(ArrayList&&) noexcept = delete;

private:
	size_t m_empty{0};
	T* m_array{new T[N]};
};

template <typename T, size_t N>
ArrayList<T, N>::~ArrayList()
{
	delete[] m_array;
}

template <typename T, size_t N>
void ArrayList<T, N>::push_back(const T& x)
{
	if (m_empty == N)
		throw std::out_of_range{"List is full"};

	m_array[m_empty] = x;
	m_empty++;
}

template <typename T, size_t N>
T ArrayList<T, N>::pop_back()
{
	if (m_empty == 0)
		throw std::out_of_range("List is empty");

	--m_empty;
	return m_array[m_empty];
}

template <typename T, size_t N>
void ArrayList<T, N>::push_front(const T& x)
{
	if (m_empty == N)
		throw std::out_of_range("List is full");

	std::memmove(m_array + 1, m_array, m_empty * sizeof(T));
	++m_empty;
	m_array[0] = x;
}

template <typename T, size_t N>
T ArrayList<T, N>::pop_front()
{
	if (m_empty == 0)
		throw std::out_of_range("List is empty");

	T out = m_array[0];
	--m_empty;
	std::memmove(m_array, m_array + 1, m_empty * sizeof(T));
	return out;
}

template <typename T, size_t N>
position_t ArrayList<T, N>::find(const T& x) const
{
	for (position_t i = 0; i < m_empty; ++i)
		if (x == m_array[i])
			return i;

	return m_empty;
}

template <typename T, size_t N>
position_t ArrayList<T, N>::insert(const position_t pos, const T& x)
{
	if (pos < 0 || pos >= m_empty)
		throw std::out_of_range("Position out of range");

	std::memmove(m_array + pos + 1, m_array + pos, (m_empty - pos) * sizeof(T));
	++m_empty;
	m_array[pos] = x;

	return pos;
}

template <typename T, size_t N>
int ArrayList<T, N>::size() const noexcept
{
	return static_cast<int>(m_empty);
}

template <typename T, size_t N>
bool ArrayList<T, N>::empty() const noexcept
{
	return m_empty == 0;
}

#endif /* ARRAYLIST_HPP */
