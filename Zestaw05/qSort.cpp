#include "Vector.hpp"
// #include "sorting.hpp"
// #include "sorting2.hpp"

#include <iostream>
#include <stdlib.h>

template <typename Iterator>
inline void c_qsort(Iterator begin, Iterator end)
{
	using Type = typename std::remove_reference<decltype(*begin)>::type;
	auto compare = [](const void* arg1, const void* arg2) -> int {
		Type lhs = *static_cast<const Type*>(arg1);
		Type rhs = *static_cast<const Type*>(arg2);
		return -(lhs < rhs) + (lhs > rhs);
	};
	qsort(begin, end - begin, sizeof(Type), compare);
}

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	Vector<unsigned> data;
	unsigned x;

	while (std::cin >> x)
		data.push_back(x);

	c_qsort(data.begin(), data.end());

	for (auto e : data)
		std::cout << e << std::endl;

	return 0;
}
