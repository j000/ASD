#include <cstdlib> // for atol, EXIT_FAILURE, EXIT_SUCCESS
#include <iomanip> // for operator<<, setprecision
#include <iostream> // for operator<<, endl, cout, ostream, basic_ostream
#include <limits> // for numeric_limits, numeric_limits<>::max_digits10
#include <math.h> // for nextafter
#include <random> // for mt19937, random_device, uniform_real_distribution

int get_random()
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	thread_local static std::uniform_int_distribution<> distribution{0, 1};
	return distribution(generator);
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Podaj rozmiar" << std::endl;
		return EXIT_FAILURE;
	}

	auto i{std::atol(argv[1])};
	if (i <= 0) {
		std::cout << "Niepoprawny rozmiar" << std::endl;
		return EXIT_FAILURE;
	}

	auto j = i;
	if (argc > 2) {
		j = std::atol(argv[1]);
		if (j <= 0) {
			std::cout << "Niepoprawny rozmiar" << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::cout << i << " " << j << std::endl;
	while (i > 0) {
		for (auto tmp{1u}; tmp < j; ++tmp) {
			std::cout << get_random() << " ";
		}
		std::cout << get_random() << std::endl;
		--i;
	}

	return EXIT_SUCCESS;
}
