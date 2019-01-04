#include <iostream>

int main(int, char**)
{
	double wynik = 0;
	double sufiks = 0;

	int n;
	std::cin >> n;
	while (n > 0) {
		double liczba;
		std::cin >> liczba;

		if (liczba + sufiks > 0)
			sufiks += liczba;
		else
			sufiks = 0;

		if (sufiks > wynik)
			wynik = sufiks;
		--n;
	}

	std::cout << "Maksymalny segment: " << wynik << std::endl;
}
