#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);
	std::size_t n{};
	std::vector<int> list1;
	std::vector<int> list2;
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

	std::vector<int> out;
	std::merge(
		list1.begin(),
		list1.end(),
		list2.begin(),
		list2.end(),
		std::back_inserter(out));

	auto last = std::unique(out.begin(), out.end());
	out.erase(last, out.end());

	for (int n : out) {
		std::cout << n << std::endl;
	}

	return EXIT_SUCCESS;
}
