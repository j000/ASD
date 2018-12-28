#include <cstdlib> // for atol, EXIT_FAILURE, EXIT_SUCCESS
#include <iomanip> // for operator<<, setprecision
#include <iostream> // for operator<<, endl, cout, ostream, basic_ostream
#include <limits> // for numeric_limits, numeric_limits<>::max_digits10
#include <math.h> // for nextafter
#include <random> // for mt19937, random_device, uniform_real_distribution

double get_random()
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	std::uniform_real_distribution<> distribution{0., std::nextafter(1., 1.1)};
	return distribution(generator);
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "Podaj ilość punktów" << std::endl;
		return EXIT_FAILURE;
	}

	auto i{std::atol(argv[1])};
	if (i <= 0) {
		std::cout << "Niepoprawna ilość punktów" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);
	while (i > 0) {
		std::cout << get_random() << " " << get_random() << std::endl;
		--i;
	}

	return EXIT_SUCCESS;
}
