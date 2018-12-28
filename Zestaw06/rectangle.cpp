#include "Vector.hpp"

#include <cstdlib>
#include <iostream>

bool czy_same_jedynki(
	const Vector<Vector<unsigned char>>& a,
	unsigned od_x,
	unsigned od_y,
	unsigned do_x,
	unsigned do_y)
{
	for (auto x = od_x; x <= do_x && x < a.size(); ++x)
		for (auto y = od_y; y <= do_y && y < a[0].size(); ++y)
			if (a[x][y] != 1)
				return false;

	return true;
}

unsigned area(Vector<Vector<unsigned char>> a, unsigned i, unsigned j)
{
	unsigned max = 0;
	unsigned base = 0 - 1;
	unsigned y = i;
	while (y < a.size()) {
		unsigned width = 0;
		unsigned x = j;
		while (x < a[0].size() && a[y][x] == 1) {
			++width;
			++x;
		}
		base = std::min(base, width);
		if (base == 0)
			break;
		max = std::max(max, (y - i + 1) * base);
		++y;
	}
	return max;
}

unsigned area(const Vector<unsigned>& a, Vector<int>& stack, const int index)
{
	const int tmp{stack.back()};
	stack.pop_back();
	unsigned out{a[tmp]}; // wysokość

	if (stack.empty()) {
		// stos jest pusty
		out *= index;
	} else {
		// stos nie jest pusty
		out *= index - 1 - stack.back();
	}

	return out;
}

unsigned max_rect(const Vector<unsigned>& a)
{
	unsigned max{0};
	Vector<int> stack;

	auto index = 0u;
	while (index < a.size()) {
		if (stack.empty() || a[index] >= a[stack.back()]) {
			stack.push_back(index);
			++index;
		} else {
			unsigned current = area(a, stack, index);
			if (current > max)
				max = current;
		}
	}

	while (!stack.empty()) {
		unsigned current = area(a, stack, a.size());
		if (current > max)
			max = current;
	}

	return max;
}

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	unsigned wiersze, kolumny;
	std::cin >> wiersze >> kolumny;

	Vector<Vector<unsigned char>> input(
		wiersze, Vector<unsigned char>(kolumny, 0));

	for (auto i = 0u; i < wiersze; ++i)
		for (auto j = 0u; j < kolumny; ++j) {
			int tmp;
			std::cin >> tmp;
			input[i][j] = static_cast<unsigned char>(tmp);
		}

	////////////////////////////////////////

	Vector<unsigned> tmp(kolumny, 0);
	unsigned biggest{1};

	// tworzymy histogram
	for (auto i = 0u; i < wiersze; ++i) {
		for (auto j = 0u; j < kolumny; ++j) {
			if (input[i][j] == 1)
				tmp[j] += 1;
			else
				tmp[j] = 0;
		}

		// liczymy max pole pod histogramem
		unsigned current_max = max_rect(tmp);
		if (current_max > biggest)
			biggest = current_max;
	}

	std::cout << biggest << std::endl;

	////////////////////////////////////////

	std::cout << "Brute force:" << std::endl;

	unsigned biggest_bruteforce{1};
	for (auto i = 0u; i < wiersze; ++i)
		for (auto j = 0u; j < kolumny; ++j)
			if (input[i][j] == 1) {
				unsigned tmp = area(input, i, j);
				if (tmp > biggest_bruteforce)
					biggest_bruteforce = tmp;
			}
	std::cout << biggest_bruteforce << std::endl;

	////////////////////////////////////////

	if (biggest != biggest_bruteforce)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
