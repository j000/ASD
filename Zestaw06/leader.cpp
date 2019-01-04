#include <cstdlib>
#include <iostream>

int main(int, char**)
{
	int licznik = 0;
	int przywodca = 0;
	int i;
	std::cin >> i;
	while (i > 0) {
		int liczba;
		std::cin >> liczba;

		if (licznik == 0) {
			++licznik;
			przywodca = liczba;
		} else if (liczba == przywodca) {
			++licznik;
		} else {
			--licznik;
		}

		--i;
	}
	if (licznik > 0) {
		std::cout << "Przywódcą jest " << przywodca << std::endl;
	} else {
		std::cout << "Ten ciąg nie ma przywódcy" << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
