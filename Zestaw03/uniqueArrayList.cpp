#include "SortedArrayList.hpp"

#include <cstddef>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main(int, char**)
{
	std::size_t n{};
	SortedArrayList<int> list1;
	SortedArrayList<int> list2;
	int tmp;

	cin >> n;
	while (n > 0) {
		cin >> tmp;
		list1.push(tmp);
		--n;
	}

	cin >> n;
	while (n > 0) {
		cin >> tmp;
		list2.push(tmp);
		--n;
	}

	merge(list1, list2).unique().print();

	return EXIT_SUCCESS;
}
