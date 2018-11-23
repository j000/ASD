#ifndef SORTEDLINKEDLIST_HPP
#define SORTEDLINKEDLIST_HPP

#include <cstddef>
#include <iostream>
#include <stdexcept>

template <typename T>
class SortedLinkedList;

template <typename T>
SortedLinkedList<T>
merge(const SortedLinkedList<T>&, const SortedLinkedList<T>&);

template <typename T>
class SortedLinkedList {
public:
	SortedLinkedList();
	~SortedLinkedList();
	SortedLinkedList(const SortedLinkedList&) = delete;
	SortedLinkedList& operator=(const SortedLinkedList&) = delete;
	SortedLinkedList(SortedLinkedList&&) = default;
	SortedLinkedList& operator=(SortedLinkedList&&) = default;

	std::size_t size() const;
	bool empty() const;
	void push(T);
	T pop();
	T erase(int);
	int find(T) const;
	void remove(T);
	SortedLinkedList& unique();
	void print() const;

	friend SortedLinkedList<T>
	merge<>(const SortedLinkedList<T>&, const SortedLinkedList<T>&);

private:
	class Node {
		friend SortedLinkedList<T>;
		friend SortedLinkedList<T>
		merge<>(const SortedLinkedList<T>&, const SortedLinkedList<T>&);

	public:
		Node() = default;
		Node(T x) : val{x} {};

	private:
		T val{};
		Node* next{};
		Node* prev{};
	};

	Node* m_head{};
	Node* m_tail{};
	std::size_t m_size{0};
};

template <typename T>
SortedLinkedList<T>::SortedLinkedList()
{
	m_head = new Node();
	m_tail = m_head;
}

template <typename T>
SortedLinkedList<T>::~SortedLinkedList()
{
	while (m_head->next) {
		m_head = m_head->next;
		delete m_head->prev;
	}
	delete m_tail;
}

// template <typename T>
// SortedLinkedList<T>::SortedLinkedList(const SortedLinkedList& list)
// 	: SortedLinkedList()
// {
// 	Node* i = list.m_head;
// 	while (i != list.m_tail) {
// 		push(i->val);
// 		i = i->next;
// 	}
// }

template <typename T>
std::size_t SortedLinkedList<T>::size() const
{
	return m_size;
}

template <typename T>
bool SortedLinkedList<T>::empty() const
{
	return m_size == 0;
}

template <typename T>
void SortedLinkedList<T>::push(T x)
{
	Node* tmp = new Node(x);
#if 0
	Node* i = m_head;
	while (i != m_tail && i->val < x)
		i = i->next;
#else
	Node* i = m_tail;
	while (i->prev != nullptr && i->prev->val > x)
		i = i->prev;
#endif
	// insert before i
	if (i->prev)
		i->prev->next = tmp;
	else
		m_head = tmp;
	tmp->prev = i->prev;
	i->prev = tmp;
	tmp->next = i;
	++m_size;
}

template <typename T>
T SortedLinkedList<T>::pop()
{
	if (m_head == m_tail)
		throw std::out_of_range("List is empty");
	T out = m_head->val;
	m_head = m_head->next;
	delete m_head->prev;
	m_head->prev = nullptr;
	--m_size;
	return out;
}

template <typename T>
T SortedLinkedList<T>::erase(int pos)
{
	Node* i = m_head;
	while (pos > 0) {
		i = i->next;
		if (i == m_tail)
			throw std::out_of_range("Position is outside list");
		--pos;
	}
	T out = i->val;
	if (i->prev)
		i->prev->next = i->next;
	else
		m_head = i->next;
	i->next->prev = i->prev;
	delete i;
	--m_size;
	return out;
}

template <typename T>
int SortedLinkedList<T>::find(T x) const
{
	int out = 0;
	for (Node* i = m_head; i != m_tail; ++out, i = i->next) {
		if (i->val == x)
			return out;
	}
	return -1;
}

template <typename T>
void SortedLinkedList<T>::remove(T x)
{
	// TODO: O(n) zamiast O(n^2)
	int i{find(x)};
	T tmp{};
	while ((tmp = erase(i)) == x) {
	};
	push(tmp);
}

template <typename T>
SortedLinkedList<T>& SortedLinkedList<T>::unique()
{
	// TODO
	return *this;
}

template <typename T>
void SortedLinkedList<T>::print() const
{
	Node* i = m_head;
	while (i != m_tail) {
		std::cout << i->val << std::endl;
		i = i->next;
	}
}

template <typename T>
SortedLinkedList<T>
merge(const SortedLinkedList<T>& a, const SortedLinkedList<T>& b)
{
#if 1
	SortedLinkedList<T> out{};
	auto i{a.m_head};
	auto j{b.m_head};
	while (i != a.m_tail && j != b.m_tail) {
		if (i->val < j->val) {
			out.push(i->val);
			i = i->next;
		} else {
			out.push(j->val);
			j = j->next;
		}
	}
	while (i != a.m_tail) {
		out.push(i->val);
		i = i->next;
	}
	while (j != b.m_tail) {
		out.push(j->val);
		j = j->next;
	}
	return out;
#else
	SortedLinkedList<T> out{};
	auto i{a.m_tail->prev};
	auto j{b.m_tail->prev};
	while (i && j) {
		if (i->val > j->val) {
			out.push(i->val);
			i = i->prev;
		} else {
			out.push(j->val);
			j = j->prev;
		}
	}
	while (i) {
		out.push(i->val);
		i = i->prev;
	}
	while (j) {
		out.push(j->val);
		j = j->prev;
	}
	return out;
#endif
}

#endif /* SORTEDLINKEDLIST_HPP */
