#ifndef CURSORLIST_HPP
#define CURSORLIST_HPP

#include <cstddef>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

using std::size_t;

template <typename T = int, T N = (T{0} - 1)>
class optional {
	T m_value;

public:
	constexpr inline optional() : m_value(N)
	{
	}
	constexpr inline optional(const T& i) : m_value{i}
	{
	}
	constexpr inline optional(T&& i) : m_value{std::forward<T>(i)}
	{
	}

	constexpr inline bool has_value() const
	{
		return m_value != N;
	}
	constexpr inline const T& value() const
	{
		return m_value;
	}

	template <typename U>
	operator U() = delete;
};

using pointer_t = optional<size_t>;

template <typename T = int, size_t N = 1000000>
class CursorList {
public:
	CursorList();
	~CursorList();

	CursorList(const CursorList&) = delete;
	CursorList& operator=(const CursorList&) = delete;
	CursorList(CursorList&&) = delete;
	CursorList& operator=(CursorList&&) = delete;

	void push_front(const T&);
	T pop_front();
	void push_back(const T&);
	T pop_back();

	pointer_t find(const T&) const;
	pointer_t erase(const pointer_t pos);
	pointer_t insert(const pointer_t pos, const T&);

	size_t size() const;
	bool empty() const;

	friend std::ostream& operator<<(std::ostream& os, const CursorList& cl)
	{
		for (pointer_t tmp{cl.m_head}; tmp.has_value();
			 tmp = cl.m_array[tmp.value()].next) {
			os << cl.m_array[tmp.value()].value << ", ";
		}
		os << "<>";
		return os;
	}

private:
	struct Node {
		T value{};
		pointer_t next{};

		Node() = default;
		Node(T&& move) : value{std::forward<T>(move)} {};
	};

	pointer_t allocate();
	void free(pointer_t);

	Node* m_array{};
	// no static array for gcc :(
	// https://gcc.godbolt.org/z/LN-81v
	pointer_t m_head{};
	pointer_t m_free{0};
	size_t m_size{0};
};

template <typename T, size_t N>
CursorList<T, N>::CursorList() : m_array{new Node[N]}
{
	for (auto i = 0u; i < N - 1; ++i) {
		m_array[i].next = pointer_t{i + 1};
	}
	m_array[N - 1].next = pointer_t{};
}

template <typename T, size_t N>
CursorList<T, N>::~CursorList()
{
	delete[] m_array;
}

template <typename T, size_t N>
pointer_t CursorList<T, N>::allocate()
{
	if (!m_free.has_value())
		throw std::out_of_range{"List is full"};

	pointer_t out{m_free};
	m_free = m_array[m_free.value()].next;
	return out;
}

template <typename T, size_t N>
void CursorList<T, N>::free(const pointer_t pos)
{
	m_array[pos.value()].next = m_free;
	m_free = pos;
}

template <typename T, size_t N>
size_t CursorList<T, N>::size() const
{
	return m_size;
}

template <typename T, size_t N>
bool CursorList<T, N>::empty() const
{
	return m_size == 0;
}

template <typename T, size_t N>
void CursorList<T, N>::push_front(const T& copy)
{
	pointer_t tmp{allocate()};
	m_array[tmp.value()].value = copy;
	m_array[tmp.value()].next = m_head;
	m_head = tmp;

	++m_size;
}

template <typename T, size_t N>
T CursorList<T, N>::pop_front()
{
	if (empty())
		throw std::out_of_range{"List is empty"};

	T out{m_array[m_head.value()].value};
	const pointer_t tmp{m_array[m_head.value()].next};
	free(m_head);
	m_head = tmp;

	--m_size;
	return out;
}

template <typename T, size_t N>
void CursorList<T, N>::push_back(const T& copy)
{
	pointer_t tmp{allocate()};
	m_array[tmp.value()].value = copy;
	m_array[tmp.value()].next = pointer_t{};

	if (!m_head.has_value()) {
		m_head = tmp;
	} else {
		pointer_t i{m_head};
		while (m_array[i.value()].next.has_value()) {
			i = m_array[i.value()].next;
		}
		m_array[i.value()].next = tmp;
	}

	++m_size;
}

template <typename T, size_t N>
T CursorList<T, N>::pop_back()
{
	if (empty())
		throw std::out_of_range{"List is empty"};

	pointer_t i{m_head};
	pointer_t prev{};
	while (m_array[i.value()].next.has_value()) {
		prev = i;
		i = m_array[i.value()].next;
	}

	if (prev.has_value())
		m_array[prev.value()].next = {};
	free(i);
	--m_size;

	if (i.value() == m_head.value())
		m_head = {};

	return m_array[i.value()].value;
}

template <typename T, size_t N>
pointer_t CursorList<T, N>::find(const T& x) const
{
	pointer_t tmp{m_head};
	while (tmp.has_value()) {
		if (m_array[tmp.value()].value == x)
			return tmp;
		tmp = m_array[tmp.value()].next;
	}

	return tmp;
}

template <typename T, size_t N>
pointer_t CursorList<T, N>::insert(const pointer_t pos, const T& x)
{
	if (!pos.has_value())
		return {};

	pointer_t i{m_head};
	pointer_t prev{};
	while (m_array[i.value()].next.has_value() && i.value() != pos.value()) {
		prev = i;
		i = m_array[i.value()].next;
	}

	pointer_t tmp = allocate();
	m_array[tmp.value()].value = x;
	m_array[tmp.value()].next = pos;

	m_array[prev.value()].next = tmp;
}

template <typename T, size_t N>
pointer_t CursorList<T, N>::erase(const pointer_t pos)
{
	if (empty())
		throw std::out_of_range{"List is empty"};

	// invalid pointer
	if (!pos.has_value() || pos.value() < 0 || pos.value() >= N)
		return {};

	// beginning
	if (m_head.value() == pos.value()) {
		m_head = m_array[m_head.value()].next;
		free(pos);
		--m_size;
		return m_head;
	}

	{
		Node& x = m_array[pos.value()];
		// inside list, easy
		if (x.next.has_value()) {
			pointer_t for_deletion{x.next};
			x = std::move(m_array[x.next.value()]);
			free(for_deletion);
			--m_size;
			return pos;
		}
	}

	// end of list, need to find previous element
	for (pointer_t tmp{m_head}; tmp.has_value();
		 tmp = m_array[tmp.value()].next) {
		if (m_array[tmp.value()].next.value() == pos.value()) {
			// now we delete
			m_array[tmp.value()].next = pointer_t{};
			free(pos);
			--m_size;
			return {};
		}
	}

	return {};
}

#endif /* CURSORLIST_HPP */
