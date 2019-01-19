#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <type_traits>
#include <vector>

/* rozmiar maski w bitach przy sortowaniu */
constexpr unsigned char shift_bits = 2;

////////////////////////////////////////

template <typename T1, typename T2>
static inline T1 reinterpret(const T2& val)
{
	static_assert(sizeof(T1) == sizeof(T2), "type sizes mismatch");
	static_assert(
		std::is_trivially_copyable<T1>::value
			&& std::is_trivially_copyable<T2>::value,
		"only works for _trivially copyable types");

	// clang-format off
	// access via pointer cast -> undefined behaviour: breaking aliasing rules
	// union -> undefined behaviour: accessing not active member
	// memcpy -> undefined behaviour: breaking aliasing rules
	//        -> not: it's using unsigned char*
	// clang-format on
	T1 magic;
	std::memcpy(&magic, &val, sizeof(magic));
	return magic;
}

////////////////////////////////////////

// if sign is 1 (negative): flips all bits
// if sign is 0 (positive): flips only the sign
static inline void flip(double& f)
{
	auto u = reinterpret<uint64_t>(f);
	if (u & (0x8000'0000'0000'0000)) {
		u ^= 0xFFFF'FFFF'FFFF'FFFF;
	} else {
		u ^= 0x8000'0000'0000'0000;
	}
	f = reinterpret<double>(u);
}

// if sign is 1 (negative): flips only the sign bit back
// if sign is 0 (positive): flips all bits back
static inline void unflip(double& f)
{
	auto u = reinterpret<uint64_t>(f);
	if (u & (0x8000'0000'0000'0000)) {
		u ^= 0x8000'0000'0000'0000;
	} else {
		u ^= 0xFFFF'FFFF'FFFF'FFFF;
	}
	f = reinterpret<double>(u);
}

void sort2(
	double* input,
	size_t size,
	unsigned char shift_bits,
	unsigned char shift)
{
	const unsigned char buckets_count = CHAR_BIT / shift_bits;

	/* warning: ISO C++ forbids variable length array */
	// std::vector<double> buckets[buckets_count];
	auto buckets = std::make_unique<std::vector<double>[]>(buckets_count);
	for (auto i = 0u; i < buckets_count; ++i) {
		buckets[i].reserve(size / buckets_count / 2);
	}

	const uint64_t shift_mask = (1u << shift_bits) - 1u;

	for (auto i = 0u; i < size; ++i) {
		uint64_t c = reinterpret<uint64_t>(input[i]);
		uint64_t nr = (c >> shift) & shift_mask;
		buckets[nr].push_back(input[i]);
	}

	auto a = 0u;
	for (auto i = 0u; i < buckets_count; ++i) {
		for (double number : buckets[i]) {
			input[a++] = number;
		}
	}
}

void sort(double* input, size_t size)
{
	for (auto i = 0u; i < size; ++i) {
		flip(input[i]);
	}

	for (unsigned char shift = 0u; shift < sizeof(double) * CHAR_BIT;
		 shift += shift_bits) {
		sort2(input, size, shift_bits, shift);
	}

	for (auto i = 0u; i < size; ++i) {
		unflip(input[i]);
	}
}

////////////////////////////////////////

double get_random()
{
	thread_local static std::mt19937 generator{std::random_device{}()};
	thread_local static std::uniform_real_distribution<double> distribution{-1.,
																			1.};
	return distribution(generator);
}

int main(int argc, char** argv)
{
	static_assert(
		shift_bits <= CHAR_BIT, "shift_bits musi być mniejsze od CHAR_BIT");

	auto n = 5u;
	if (argc > 1) {
		unsigned long tmp = std::strtoul(argv[1], nullptr, 10);
		if (tmp > 0)
			n = static_cast<unsigned int>(tmp);
	}

	std::vector<double> input(n);
	for (auto& d : input) {
		d = get_random();
	}

	sort(input.data(), n);

	if (!is_sorted(input.begin(), input.end())) {
		std::cout << " *** NOT SORTED *** " << std::endl;
		return 1;
	}

	if (n % 2)
		std::cout << input[n / 2] << std::endl;
	else
		std::cout << (input[n / 2] + input[n / 2 - 1]) / 2 << std::endl;
}
