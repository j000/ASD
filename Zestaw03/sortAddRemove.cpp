#include "SortedArrayList.hpp"
#include "SortedLinkedList.hpp"

#include <chrono>
#include <iostream>
#include <random>

constexpr unsigned MAX = 50'000;

unsigned get_random(unsigned x)
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	std::uniform_int_distribution<decltype(x)> distribution{0, x};
	return distribution(generator);
}

template <typename T>
inline void test(const char* title)
{
	T list{};
	const auto start = std::chrono::system_clock::now();
	for (auto i = 0u; i < MAX; ++i) {
		list.push(get_random(MAX));
	}
	for (auto i = 1u; i < MAX; ++i) {
		list.erase(get_random(list.size() - 1));
	}
	list.erase(0);
	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << title << ": " << elapsed_seconds.count() << "s\n";
}

int main()
{
	test<SortedArrayList<int>>("Array List");
	test<SortedLinkedList<int>>("Linked List");
}
