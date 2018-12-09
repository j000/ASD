#include "Vector.hpp"
#include "sorting.hpp"
#include "sorting2.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

template <typename V, typename... T>
constexpr auto array_of(T&&... t) -> std::array<V, sizeof...(T)>
{
	return {{std::forward<T>(t)...}};
}

static constexpr auto counts = array_of<std::size_t>(
	0u, 2'500u, 5'000u, 10'000u, 20'000u, 40'000u, 80'000u, 160'000u);
static constexpr unsigned MAX{counts.back() * 1000};

unsigned get_random()
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	std::uniform_int_distribution<unsigned> distribution{0, MAX};
	return distribution(generator);
}

template <typename Iterator>
void test_sort(
	const char* name,
	Iterator start_it,
	std::size_t limit,
	void(f)(Iterator, Iterator))
{
	if (limit == 0)
		std::cout << ",\"" << name << "\"";
	else {
		Vector<unsigned> kopia(start_it, start_it + limit);
		auto start = std::chrono::high_resolution_clock::now();
		f(kopia.begin(), kopia.end());
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		std::cout << "," << elapsed.count();
		std::cout.flush();
	}
}

int main(int, char**)
{
	using std::swap;
	std::ios_base::sync_with_stdio(false);

	Vector<unsigned> dane;
	unsigned x;

	while (std::cin >> x)
		dane.push_back(x);

	if (dane.size() < counts.back()) {
		std::cerr << " *** ZA MAŁO DANYCH ***" << std::endl;
		return 1;
	}

	/* Przewaga dla counting sort: */
	// dane[0] = MAX - 1;
#ifdef MALE_DANE
	/* Małe dane */
	Vector<unsigned> dane2(dane);
	for (auto& i : dane2) {
		i %= 128;
	}
#endif

	for (auto j : counts) {
		swap(dane[j-1], dane.back());
		std::cout << j;
		test_sort("std::sort", dane.begin(), j, std::sort);
		test_sort("radix sort", dane.begin(), j, radix_sort);
		test_sort("insertion sort", dane.begin(), j, insertion_sort);
		test_sort("selection sort", dane.begin(), j, selection_sort);
		test_sort("bubble sort", dane.begin(), j, bubble_sort);
		test_sort("counting sort", dane.begin(), j, counting_sort);
		test_sort("qsort", dane.begin(), j, c_qsort);
		test_sort("heap sort (iter)", dane.begin(), j, heap_sort_iter);
		test_sort("heap sort", dane.begin(), j, heap_sort_rec);
		test_sort("quicksort", dane.begin(), j, quick_sort);
		test_sort("merge sort", dane.begin(), j, merge_sort);

#ifdef MALE_DANE
		test_sort("std::sort 2", dane2.begin(), j, std::sort);
		test_sort("radix sort 2", dane2.begin(), j, radix_sort);
		test_sort("insertion sort 2", dane2.begin(), j, insertion_sort);
		test_sort("selection sort 2", dane2.begin(), j, selection_sort);
		test_sort("bubble sort 2", dane2.begin(), j, bubble_sort);
		test_sort("counting sort 2", dane2.begin(), j, counting_sort);
		test_sort("heap sort", dane.begin(), j, heap_sort_iter);
		test_sort("heap sort (rec)", dane.begin(), j, heap_sort_rec);
#endif
		std::cout << std::endl;
		swap(dane[j-1], dane.back());
	}
	return 0;
}
