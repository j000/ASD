#include <iostream>

int main(int, char**)
{
	int x;
	std::cin >> x;

	int n1;
	std::cin >> n1;
	Vector<int> a(n1);
	for (auto i = 0; i < n1; i++)
		std::cin >> a[i];

	int n2;
	std::cin >> n2;
	Vector<int> b(n2);
	for (auto i = 0; i < n2; i++)
		std::cin >> b[i];

	int l = 0;
	int r = n2 - 1;

	while (l < n1 && r >= 0) {
		if (a[l] + b[r] == x) {
			std::cout << a[l] << " + " << b[r] << " = " << x << "\n";
			return EXIT_SUCCESS;
		} else if (a[l] + b[r] > x) {
			--r;
		} else {
			++l;
		}
	}

	std::cout << "Dla " << x << " takie liczby nie istnieją\n";
	return EXIT_FAILURE;
}
