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

	auto i{std::atol(argv[1])};
	if (i <= 0) {
		std::cout << "Podaj poprawny rozmiar listy" << std::endl;
		return EXIT_FAILURE;
	}

	auto l1{get_random(i)};
	auto l2{i - l1};

	std::cout << l1 << std::endl;
	auto list{0u};
	while (l1 > 0) {
		list += get_random(10);
		std::cout << list << " ";
		--l1;
	}
	std::cout << std::endl;

	std::cout << l2 << std::endl;
	list = 0u;
	while (l2 > 0) {
		list += get_random(10);
		std::cout << list << " ";
		--l2;
	}
	std::cout << std::endl;

	return EXIT_SUCCESS;
}
