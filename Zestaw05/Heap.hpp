#ifndef HEAP_HPP
#define HEAP_HPP

#include "Vector.hpp"

#include <cstdlib>
#include <stdexcept>

using size_type = std::size_t;

template <typename T>
class Heap {
public:
	void build(T[], size_type);
	void push(T);
	T& top() noexcept;
	T pop();
	size_type size() const noexcept;
	bool empty() const noexcept;
	bool check() const noexcept;
	static void sortRec(T[], size_type);
	static void sortIter(T[], size_type);

private:
	void heapifyIter(size_type) noexcept;
	static void heapifyIter(T[], size_type, size_type) noexcept;
	void heapifyRec(const size_type) noexcept;
	static void heapifyRec(T[], size_type, const size_type) noexcept;
	static size_type parent(const size_type) noexcept;
	static size_type first_child(const size_type) noexcept;
	static size_type second_child(const size_type) noexcept;
	size_type get_largest(size_type) const noexcept;
	static size_type get_largest(const T[], size_type, size_type) noexcept;

public:
	Vector<T> data{};
};

template <typename T>
size_type Heap<T>::size() const noexcept
{
	return data.size();
}

template <typename T>
bool Heap<T>::empty() const noexcept
{
	return size() == 0;
}

template <typename T>
size_type Heap<T>::parent(size_type child) noexcept
{
	return (child - 1) / 2;
}

template <typename T>
size_type Heap<T>::first_child(size_type parent) noexcept
{
	return 2 * parent + 1;
}

template <typename T>
size_type Heap<T>::second_child(size_type parent) noexcept
{
	return 2 * parent + 2;
}

template <typename T>
bool Heap<T>::check() const noexcept
{
	for (auto i = 1u; i < data.size(); ++i) {
		if (data[i] > data[parent(i)])
			return false;
	}
	return true;
}

template <typename T>
T& Heap<T>::top() noexcept
{
	return data[0];
}

template <typename T>
void Heap<T>::build(T arr[], size_type size)
{
	// TODO: assign copies and swaps, can we do better?
	data.assign(&arr[0], &arr[size]);
	for (auto i = data.size() / 2; i > 0; --i) {
		heapifyRec(i - 1);
	}
}

template <typename T>
T Heap<T>::pop()
{
	if (empty())
		throw std::out_of_range("Kopiec pusty");

	T out{data[0]};
	data[0] = data.back();
	data.pop_back();

	heapifyIter(0);

	return out;
}

template <typename T>
size_type Heap<T>::get_largest(
	const T arr[], size_type arr_size, size_type parent) noexcept
{
	size_type largest = parent;
	if (first_child(parent) < arr_size
		&& arr[largest] < arr[first_child(parent)])
		largest = first_child(parent);
	if (second_child(parent) < arr_size
		&& arr[largest] < arr[second_child(parent)])
		largest = second_child(parent);

	return largest;
}

template <typename T>
size_type Heap<T>::get_largest(size_type parent) const noexcept
{
	return get_largest(data.data(), data.size(), parent);
}

template <typename T>
void Heap<T>::heapifyIter(T arr[], size_type arr_size, size_type i) noexcept
{
	using std::swap;

	size_type largest{};
	while ((largest = get_largest(arr, arr_size, i)) != i) {
		swap(arr[i], arr[largest]);
		i = largest;
	}
}

template <typename T>
void Heap<T>::heapifyIter(size_type i) noexcept
{
	heapifyIter(data.data(), data.size(), i);
}

template <typename T>
void Heap<T>::heapifyRec(T arr[], size_type arr_size, size_type i) noexcept
{
	using std::swap;

	size_type largest = get_largest(arr, arr_size, i);
	if (largest == i)
		return;
	swap(arr[i], arr[largest]);
	heapifyRec(arr, arr_size, largest);
}

template <typename T>
void Heap<T>::heapifyRec(size_type i) noexcept
{
	heapifyRec(data.data(), data.size(), i);
}

template <typename T>
void Heap<T>::push(T x)
{
	// grow by whole levels
	if (data.size() == data.capacity())
		data.reserve(data.size() * 2 + 1);

	data.push_back(x);

	for (auto pos = data.size() - 1; pos > 0 && data[pos] > data[parent(pos)];
		 pos = parent(pos)) {
		using std::swap;
		swap(data[pos], data[parent(pos)]);
	}
}

template <typename T>
void Heap<T>::sortRec(T arr[], size_type arr_size)
{
	using std::swap;

	for (auto i = arr_size / 2; i > 0; --i) {
		heapifyRec(arr, arr_size, i - 1);
	}

	for (auto i = arr_size - 1; i > 0; --i) {
		swap(arr[0], arr[i]);
		heapifyRec(arr, i, 0);
	}
}

template <typename T>
void Heap<T>::sortIter(T arr[], size_type arr_size)
{
	using std::swap;

	for (auto i = arr_size / 2; i > 0; --i) {
		heapifyIter(arr, arr_size, i - 1);
	}

	for (auto i = arr_size - 1; i > 0; --i) {
		swap(arr[0], arr[i]);
		heapifyIter(arr, i, 0);
	}
}

#endif /* HEAP_HPP */
