#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T = int>
class LinkedList {
public:
	~LinkedList();

	void push_front(T&&);
	void push_front(const T&);
	T pop_front();

	void push_back(T&&);
	void push_back(const T&);
	T pop_back();

	T* find(T);
	T* erase(T*);
	T* insert(T*, T);

	inline int size() const;
	inline bool empty() const;

private:
	struct Node {
		T value{};
		Node* prev{};
		Node* next{};

		Node() = default;
		Node(const T& copy) : value{copy} {}
		Node(T&& move) : value{std::forward(move)} {}
	};

	Node* m_head{};
	Node* m_tail{};
	std::size_t m_size{};

	friend class Tester;
};

template <typename T>
LinkedList<T>::~LinkedList()
{
	Node* loop = m_head;
	while (loop) {
		Node* tmp = loop->next;
		delete loop;
		loop = tmp;
	}
}

template <typename T>
void LinkedList<T>::push_front(T&& move)
{
	Node* tmp = new Node(std::forward(move));
	tmp->next = m_head;

	if (m_head)
		m_head->prev = tmp;

	m_head = tmp;

	if (!m_tail)
		m_tail = tmp;
	++m_size;
}

template <typename T>
void LinkedList<T>::push_front(const T& copy)
{
	Node* tmp = new Node();
	tmp->value = copy;
	tmp->next = m_head;

	if (m_head)
		m_head->prev = tmp;

	m_head = tmp;

	if (!m_tail)
		m_tail = tmp;
	++m_size;
}

template <typename T>
T LinkedList<T>::pop_front()
{
	if (empty())
		throw std::out_of_range("List is empty");

	--m_size;

	Node* tmp = m_head;

	if (m_tail == m_head)
		m_tail = nullptr;

	m_head = m_head->next;
	if (m_head)
		m_head->prev = nullptr;
	T out(std::move(tmp->value));
	delete tmp;
	return out;
}

template <typename T>
void LinkedList<T>::push_back(T&& move)
{
	Node* tmp = new Node(std::forward(move));
	tmp->prev = m_tail;

	if (m_tail)
		m_tail->next = tmp;

	m_tail = tmp;

	if (!m_head)
		m_head = tmp;
	++m_size;
}

template <typename T>
void LinkedList<T>::push_back(const T& copy)
{
	Node* tmp = new Node();
	tmp->value = copy;
	tmp->prev = m_tail;

	if (m_tail)
		m_tail->next = tmp;

	m_tail = tmp;

	if (!m_head)
		m_head = tmp;
	++m_size;
}

template <typename T>
T LinkedList<T>::pop_back()
{
	if (empty())
		throw std::out_of_range("List is empty");

	--m_size;

	Node* tmp = m_tail;

	if (m_tail == m_head)
		m_head = nullptr;

	m_tail = m_tail->prev;
	if (m_tail)
		m_tail->next = nullptr;
	T out(std::move(tmp->value));
	delete tmp;
	return out;
}

template <typename T>
T* LinkedList<T>::find(T x)
{
	Node* tmp = m_head;
	while (tmp) {
		if (tmp->value == x)
			return &tmp->value;
		tmp = tmp->next;
	}
	return nullptr;
}

template <typename T>
T* LinkedList<T>::erase(T* pos)
{
	if (!pos)
		return nullptr;

	Node* tmp = static_cast<Node*>(pos);

	Node* i = m_head;
	while (i && i != tmp) {
		i = i->next;
	}
	if (!i)
		return nullptr;

	i = i->next;

	if (tmp->prev)
		tmp->prev->next = tmp->next;
	if (tmp->next)
		tmp->next->prev = tmp->prev;

	delete tmp;
	return i;
}

template <typename T>
T* LinkedList<T>::insert(T*, T)
{
	return nullptr;
}

template <typename T>
int LinkedList<T>::size() const
{
	return static_cast<int>(m_size);
}

template <typename T>
bool LinkedList<T>::empty() const
{
	return m_size == 0;
}

#endif /* LINKEDLIST_HPP */
