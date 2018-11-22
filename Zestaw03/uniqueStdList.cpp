#include <cstddef>
#include <iostream>
#include <list>

using std::cin;
using std::cout;
using std::endl;

int main(int, char**)
{
	std::size_t n{};
	std::list<int> list1;
	std::list<int> list2;
	int tmp;

	cin >> n;
	while (n > 0) {
		cin >> tmp;
		list1.push_back(tmp);
		--n;
	}

	cin >> n;
	while (n > 0) {
		cin >> tmp;
		list2.push_back(tmp);
		--n;
	}

	list1.merge(list2);
	list1.unique();

	for (int n : list1) {
		std::cout << n << std::endl;
	}

	return EXIT_SUCCESS;
}
