// Copyright (C) 2018 Jarosław Rymut
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>
#ifndef STACK_HPP
#define STACK_HPP

#include <array>
#include <cstdlib>
#include <stdexcept>

template <typename T = int, std::size_t N = 100>
class Stack {
public:
	void push(T x);
	T pop();
	T& top();
	std::size_t size();
	bool empty();

private:
	std::size_t m_empty {0};
	T m_vector[N] {};
	// std::array<T, N> m_vector;
};

template <typename T, std::size_t N>
void Stack<T, N>::push(T x)
{
	if (m_empty == N)
		throw std::out_of_range("Stack is full");
	m_vector[m_empty++] = x;
}

template <typename T, std::size_t N>
T Stack<T, N>::pop()
{
	if (m_empty == 0)
		throw std::out_of_range("Stack is empty");
	return m_vector[--m_empty];
}

template <typename T, std::size_t N>
T& Stack<T, N>::top()
{
	if (m_empty == 0)
		throw std::out_of_range("Stack is empty");
	return m_vector[m_empty - 1];
}

template <typename T, std::size_t N>
std::size_t Stack<T, N>::size()
{
	return m_empty;
}

template <typename T, std::size_t N>
bool Stack<T, N>::empty()
{
	return (m_empty == 0);
}

#endif /* STACK_HPP */
