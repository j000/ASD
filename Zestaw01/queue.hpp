#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstdlib>
#include <stdexcept>

template <typename T = int, std::size_t N = 100>
class Queue {
public:
	void push(T);
	T pop();
	T& front();
	std::size_t size();
	inline bool empty();
	inline bool full();

private:
	std::size_t m_head {0}, m_tail {0};
	bool m_full {false};
	T m_vector[N];
};

template <typename T, std::size_t N>
void Queue<T, N>::push(T x)
{
	if (full()) {
		throw std::out_of_range("Queue is full");
	}
	m_vector[m_head] = x;
	m_head = (m_head + 1) % N;
	m_full = m_head == m_tail;
}

template <typename T, std::size_t N>
T Queue<T, N>::pop()
{
	if (empty())
		throw std::out_of_range("Queue is empty");
	std::size_t old = m_tail;
	m_tail = (m_tail + 1) % N;
	m_full = false;
	return m_vector[old];
}

template <typename T, std::size_t N>
T& Queue<T, N>::front()
{
	if (empty())
		throw std::out_of_range("Queue is empty");
	return m_vector[m_tail];
}

template <typename T, std::size_t N>
std::size_t Queue<T, N>::size()
{
	if (full())
		return N;
	if (m_head >= m_tail)
		return m_head - m_tail;
	return N + m_head - m_tail;
}

template <typename T, std::size_t N>
bool Queue<T, N>::empty()
{
	return !m_full && (m_head == m_tail);
}

template <typename T, std::size_t N>
bool Queue<T, N>::full()
{
	return m_full;
}

#endif /* QUEUE_HPP */
