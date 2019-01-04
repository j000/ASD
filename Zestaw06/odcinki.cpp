#include <iostream>

int other(int a)
{
	return 1 - a;
}

int main(int, char**)
{
	double suma[2]{0, 0};
	int t = 0;
	double wynik = 0;

	int n;
	std::cin >> n;
	double poprzedni;
	std::cin >> poprzedni;
	--n;
	while (n > 0) {
		double biezacy;
		std::cin >> biezacy;

		suma[t] += biezacy - poprzedni;
		poprzedni = biezacy;

		if (suma[t] < suma[other(t)]) {
			wynik += suma[other(t)];
			suma[0] = suma[1] = 0;
		}

		t = other(t);
		--n;
	}

	if (suma[0] > suma[1])
		wynik += suma[0];
	else
		wynik += suma[1];

	std::cout << "Maksymalna suma: " << wynik << std::endl;
}
