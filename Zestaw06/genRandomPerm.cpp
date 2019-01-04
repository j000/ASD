#include "Vector.hpp"

#include <iomanip>
#include <iostream>
#include <random>

unsigned get_random(unsigned x)
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	std::uniform_int_distribution<decltype(x)> distribution{0, x};
	return distribution(generator);
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Podaj rozmiar listy" << std::endl;
		return EXIT_FAILURE;
	}

	unsigned n;
	{
		auto tmp{std::atol(argv[1])};
		if (tmp <= 0) {
			std::cout << "Niepoprawny rozmiar" << std::endl;
			return EXIT_FAILURE;
		}
		n = static_cast<unsigned>(tmp);
	}

	Vector<unsigned> array(n);
	for (auto i = 0u; i < n; ++i) {
		array[i] = i + 1;
	}

	for (auto i = n - 1; i > 0; --i) {
		auto j = get_random(i);
		using std::swap;
		swap(array[i], array[j]);
	}

	for (const auto i : array) {
		std::cout << i << std::endl;
	}

	return EXIT_SUCCESS;
}
