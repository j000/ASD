#include <iomanip>
#include <iostream>
#include <random>

double get_random()
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	std::uniform_real_distribution<> distribution{
		0., std::nextafter(1., 1.1)};
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

	std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);
	while (i > 0) {
		std::cout << get_random() << " " << get_random() << std::endl;
		--i;
	}

	return EXIT_SUCCESS;
}
