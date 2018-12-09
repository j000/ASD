#include <iostream>
#include <random>

unsigned get_random(unsigned x)
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	thread_local static std::uniform_int_distribution<decltype(x)> distribution{0, x};
	return distribution(generator);
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Podaj rozmiar listy" << std::endl;
		return EXIT_FAILURE;
	}

	auto i{std::atol(argv[1])};
	if (i <= 0) {
		std::cout << "Podaj poprawny rozmiar listy" << std::endl;
		return EXIT_FAILURE;
	}

	get_random(i);
	while (i > 0) {
		std::cout << get_random(0) << std::endl;
		--i;
	}

	return EXIT_SUCCESS;
}
