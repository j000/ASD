#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iterator>
#include <limits>
#include <memory>

#if !defined(__cplusplus) || __cplusplus < 201103L
#error "I need C++11!"
#endif
#if __cplusplus < 201402L
// C++11
namespace std {
template <bool B, class True = void>
using enable_if_t = typename std::enable_if<B, True>::type;
template <bool B, class True, class False>
using conditional_t = typename conditional<B, True, False>::type;
}
#endif
// #if __cplusplus < 201703L
// // C++11 C++14
// namespace std {
// template <class Base, class Derived>
// constexpr bool is_base_of_v = std::is_base_of<Base, Derived>::value;
// }
// #endif

template <typename T>
class Vector {
	struct Deleter {
		void operator()(T* p) const
		{
			::operator delete(p);
		}
	};

public:
	// types
	using value_type = T;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = value_type*;
	using const_iterator = const value_type*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// construct/copy/destroy
	Vector() = default;
	explicit Vector(size_type);
	Vector(size_type, const T&);
	// clang-format off
	template <typename InputIterator,
		typename = std::enable_if_t<
			std::is_base_of<
				std::input_iterator_tag,
				typename std::iterator_traits<InputIterator>::iterator_category
			>::value
		>>
	// clang-format on
	Vector(InputIterator, InputIterator);
	Vector(const Vector&);
	Vector(Vector&&) noexcept;
	Vector(std::initializer_list<T>);
	~Vector() noexcept;
	Vector& operator=(const Vector&);
	Vector& operator=(Vector&&) noexcept;
	// TODO: Vector& operator=(std::initializer_list<T>);
	// clang-format off
	template <typename InputIterator,
		typename = std::enable_if_t<
			std::is_base_of<
				std::input_iterator_tag,
				typename std::iterator_traits<InputIterator>::iterator_category
			>::value
		>>
	// clang-format on
	void assign(InputIterator, InputIterator);
	void assign(size_type, const T&);
	void assign(std::initializer_list<T>);

	// iterators
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	const_reverse_iterator crend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;

	// capacity
	bool empty() const noexcept;
	size_type size() const noexcept;
	size_type max_size() const noexcept;
	size_type capacity() const noexcept;
	// TODO: void resize(size_type);
	// TODO: void resize(size_type, const T&);
	void reserve(size_type capacity);
	// TODO: void shrink_to_fit();

	// element access
	reference operator[](size_type position);
	const_reference operator[](size_type position) const;
	reference at(size_type position);
	const_reference at(size_type position) const;
	reference front() noexcept;
	const_reference front() const noexcept;
	reference back() noexcept;
	const_reference back() const noexcept;

	// data access
	T* data() noexcept;
	const T* data() const noexcept;

	// modifiers
	// TODO: template <typename... Args>
	//       reference emplace_back(Args&&...);
	void push_back(const T&);
	void push_back(T&&);
	void pop_back();

	// TODO: template <typename... Args>
	//       iterator emplace(const_iterator, Args&&...);
	iterator insert(const_iterator, const T&);
	iterator insert(const_iterator, T&&);
	// TODO: iterator insert(const_iterator, size_type, const T&);
	// TODO: template <typename InputIterator>
	//       iterator insert(const_iterator, InputIterator, InputIterator);
	// TODO: iterator insert(const_iterator, std::initializer_list<T>);
	iterator erase(const iterator);
	iterator erase(const iterator, const iterator);
	void swap(Vector& other) noexcept;
	void clear() noexcept;

	friend inline bool operator==(const Vector& lhs, const Vector& rhs) noexcept
	{
		if (lhs.size() != rhs.size())
			return false;

		for (size_type i = 0; i < lhs.size(); i++) {
			if (lhs[i] != rhs[i])
				return false;
		}

		return true;
	}

	friend inline bool operator!=(const Vector& lhs, const Vector& rhs) noexcept
	{
		return !(lhs == rhs);
	}

private:
	inline void _destroy() noexcept;
	inline void _destroy(size_type start, size_type end) noexcept;
	inline void _copy_internal(const_reference copy) noexcept(
		std::is_nothrow_copy_constructible<T>::value);
	inline void _move_internal(T&& move) noexcept(
		std::is_nothrow_move_constructible<T>::value);
	inline void _resize_if_needed(size_type elems = 1);
	inline void _reserve_internal(size_type capacity);
	inline void _move_up_internal(size_type start);

	template <typename U = T>
	inline std::enable_if_t<std::is_nothrow_move_constructible<U>::value>
	_copy_values(Vector& destination) noexcept(
		// inline void Vector<T>::_copy_values(Vector& destination) noexcept(
		std::is_nothrow_move_constructible<U>::value)
	{
		for (size_type loop = 0; loop < m_empty; ++loop) {
			destination._move_internal(std::move(m_buffer[loop]));
		}
	}

	template <typename U = T>
	inline std::enable_if_t<!std::is_nothrow_move_constructible<U>::value>
	_copy_values(Vector& destination) noexcept(
		// inline void Vector<T>::_copy_values(Vector& destination) noexcept(
		std::is_nothrow_copy_constructible<U>::value)
	{
		for (size_type loop = 0; loop < m_empty; ++loop) {
			destination._copy_internal(m_buffer[loop]);
		}
	}

public:
	size_type m_capacity{0};
	size_type m_empty{0};
	std::unique_ptr<T[], Deleter> m_buffer{reinterpret_cast<T*>(
		::operator new(m_capacity * sizeof(T)))};
};

template <typename T>
Vector<T>::Vector(size_type size) : m_capacity{size}, m_empty{size}
{
	for (size_type loop = 0; loop < size; ++loop) {
		::new (&m_buffer[loop]) T();
	}
}

template <typename T>
Vector<T>::Vector(size_type size, const_reference value)
	: m_capacity{size}, m_empty{size}
{
	for (size_type loop = 0; loop < size; ++loop) {
		::new (&m_buffer[loop]) T(value);
	}
}

template <typename T>
template <typename InputIterator, typename>
Vector<T>::Vector(InputIterator first, InputIterator last)
	: m_capacity{static_cast<decltype(m_capacity)>(last - first)}
{
	while (first != last) {
		push_back(*first);
		++first;
	}
}

template <typename T>
Vector<T>::Vector(const Vector& copy) : m_capacity{copy.m_empty}
{
	size_type loop{};
	try {
		for (loop = 0; loop < copy.m_empty; ++loop) {
			push_back(copy.m_buffer[loop]);
		}
	} catch (...) {
		--loop;
		_destroy(0, loop);
		throw;
	}
}

template <typename T>
Vector<T>::Vector(Vector&& move) noexcept
{
	this->swap(move);
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list) : m_capacity{list.size()}
{
	for (auto& i : list) {
		push_back(i);
	}
}

template <typename T>
Vector<T>::~Vector() noexcept
{
	_destroy();
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& copy)
{
	if (&copy == this)
		return *this;

	Vector tmp(copy);
	swap(*this, tmp);

	return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& move) noexcept
{
	this->swap(std::move(move));
	return *this;
}

template <typename T>
template <typename InputIterator, typename>
void Vector<T>::assign(InputIterator first, InputIterator last)
{
	Vector tmp(first, last);
	this->swap(tmp);
}

template <typename T>
void Vector<T>::assign(size_type count, const_reference value)
{
	Vector tmp(count, value);
	this->swap(tmp);
}

template <typename T>
void Vector<T>::assign(std::initializer_list<T> list)
{
	Vector tmp(list);
	this->swap(tmp);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::begin() noexcept
{
	// return m_buffer.get();
	return &m_buffer[0];
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::begin() const noexcept
{
	return cbegin();
}

template <typename T>
typename Vector<T>::iterator Vector<T>::end() noexcept
{
	return &m_buffer[m_empty];
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::end() const noexcept
{
	return cend();
}

template <typename T>
typename Vector<T>::reverse_iterator Vector<T>::rbegin() noexcept
{
	return reverse_iterator{end()};
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::rbegin() const noexcept
{
	return crbegin();
}

template <typename T>
typename Vector<T>::reverse_iterator Vector<T>::rend() noexcept
{
	return reverse_iterator{begin()};
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::rend() const noexcept
{
	return crend();
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept
{
	// return m_buffer.get();
	return &m_buffer[0];
}

template <typename T>
typename Vector<T>::const_iterator Vector<T>::cend() const noexcept
{
	return &m_buffer[m_empty];
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::crbegin() const noexcept
{
	return const_reverse_iterator{end()};
}

template <typename T>
typename Vector<T>::const_reverse_iterator Vector<T>::crend() const noexcept
{
	return const_reverse_iterator{begin()};
}

template <typename T>
bool Vector<T>::empty() const noexcept
{
	return m_empty == 0;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::size() const noexcept
{
	return m_empty;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::max_size() const noexcept
{
	return std::numeric_limits<difference_type>::max();
}

template <typename T>
typename Vector<T>::size_type Vector<T>::capacity() const noexcept
{
	return m_capacity;
}

template <typename T>
void Vector<T>::reserve(size_type capacity)
{
	if (capacity <= m_capacity)
		return;

	_reserve_internal(capacity);
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[](const size_type position)
{
	return m_buffer[position];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::
operator[](const size_type position) const
{
	return m_buffer[position];
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(const size_type position)
{
	if (!(position < size()))
		throw std::out_of_range{"Out of range"};

	return m_buffer[position];
}

template <typename T>
typename Vector<T>::const_reference
Vector<T>::at(const size_type position) const
{
	if (!(position < size()))
		throw std::out_of_range{"Out of range"};

	return m_buffer[position];
}

template <typename T>
typename Vector<T>::reference Vector<T>::front() noexcept
{
	return m_buffer[0];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::front() const noexcept
{
	return m_buffer[0];
}

template <typename T>
typename Vector<T>::reference Vector<T>::back() noexcept
{
	return m_buffer[m_empty - 1];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::back() const noexcept
{
	return m_buffer[m_empty - 1];
}

template <typename T>
T* Vector<T>::data() noexcept
{
	return m_buffer.get();
}

template <typename T>
const T* Vector<T>::data() const noexcept
{
	return m_buffer.get();
}

template <typename T>
void Vector<T>::push_back(T&& move)
{
	_resize_if_needed();
	_move_internal(std::move(move));
}

template <typename T>
void Vector<T>::push_back(const T& copy)
{
	_resize_if_needed();
	_copy_internal(copy);
}

template <typename T>
void Vector<T>::pop_back()
{
	--m_empty;
	m_buffer[m_empty].~T();
}

template <typename T>
typename Vector<T>::iterator
Vector<T>::insert(const_iterator pos, const T& copy)
{
	if (pos == end()) {
		push_back(copy);
		return end() - 1;
	}

	size_type start = static_cast<size_type>(pos - &m_buffer[0]);
	_resize_if_needed();

	_move_up_internal(start);
	m_buffer[start] = copy;
	return &m_buffer[start];
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T&& move)
{
	if (pos == end()) {
		push_back(std::move(move));
		return end() - 1;
	}

	size_type start = static_cast<size_type>(pos - &m_buffer[0]);
	_resize_if_needed();

	_move_up_internal(start);
	m_buffer[start] = std::move(move);
	return &m_buffer[start];
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const iterator pos)
{
	const size_type index = static_cast<size_type>(pos - &m_buffer[0]);

	for (size_type loop = index + 1; loop < m_empty; ++loop) {
		m_buffer[loop - 1] = m_buffer[loop];
	}

	pop_back();

	return &m_buffer[index];
}

template <typename T>
typename Vector<T>::iterator
Vector<T>::erase(const iterator first, const iterator last)
{
	if (first == last)
		return iterator{first};

	size_type start = first - &m_buffer[0];
	size_type end = last - &m_buffer[0];

	while (end < m_empty) {
		m_buffer[start] = m_buffer[end];
		++start;
		++end;
	}

	m_empty = start;

	while (start < end) {
		m_buffer[start].~T();
		++start;
	}

	return first;
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept
{
	using std::swap;
	swap(m_capacity, other.m_capacity);
	swap(m_empty, other.m_empty);
	swap(m_buffer, other.m_buffer);
}

template <typename T>
void Vector<T>::clear() noexcept
{
	_destroy();
	m_empty = 0;
}

// specialize swap
template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}

// private functions

template <typename T>
void Vector<T>::_destroy() noexcept
{
	_destroy(0, m_empty);
}

template <typename T>
inline void Vector<T>::_destroy(size_type start, size_type end) noexcept
{
	if (std::is_trivially_destructible<T>::value)
		return;

	for (size_type loop = start; loop < end; ++loop) {
		// try {
		m_buffer[loop].~T();
		// } catch (...) {
		// }
	}
}

template <typename T>
inline void Vector<T>::_copy_internal(const_reference copy) noexcept(
	std::is_nothrow_copy_constructible<T>::value)
{
	new (&m_buffer[m_empty]) T(copy);
	++m_empty;
}

template <typename T>
inline void Vector<T>::_move_internal(T&& move) noexcept(
	std::is_nothrow_move_constructible<T>::value)
{
	new (&m_buffer[m_empty]) T(std::move(move));
	++m_empty;
}

template <typename T>
inline void Vector<T>::_resize_if_needed(size_type elems)
{
	if (m_empty + elems <= m_capacity)
		return;

	size_type capacity = m_capacity / 2;
	if (elems > capacity)
		capacity = elems;

	capacity += m_capacity;

	_reserve_internal(capacity);
}

template <typename T>
inline void Vector<T>::_reserve_internal(size_type capacity)
{
	Vector tmp;
	tmp.m_capacity = capacity;
	tmp.m_buffer.reset(reinterpret_cast<T*>(
		::operator new(capacity * sizeof(T))));

	_copy_values(tmp);

	this->swap(tmp);
}

template <typename T>
inline void Vector<T>::_move_up_internal(size_type start)
{
	if (start >= m_empty)
		return;
	_move_internal(std::move(back()));
	for (size_type loop = m_empty - 2; loop > start; --loop) {
		m_buffer[loop] = std::move(m_buffer[loop - 1]);
	}
}

// vim: tabstop=4 shiftwidth=0 noexpandtab
#endif /* VECTOR_HPP */
