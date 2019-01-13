#include "Vector.hpp"
#include "sorting.hpp"
#include "sorting2.hpp"

#include <iostream>

template <typename T>
class Counter {
public:
	Counter() = default;
	explicit Counter(T x) : store{x}
	{
	}
	~Counter() = default;
	Counter(const Counter& other) : store{other.store}
	{
		// ++assignments;
	}
	Counter(Counter&& other) : store{std::move(other.store)}
	{
		// ++assignments;
	}
	Counter& operator=(const Counter& other)
	{
		auto temp = other;
		swap(temp);
		return *this;
	}
	Counter& operator=(Counter&& other)
	{
		++assignments;
		store = std::move(other.store);
		return *this;
	}

	void swap(Counter& rhs) noexcept
	{
		using std::swap;
		++assignments;
		swap(store, rhs.store);
	}

	friend inline void swap(Counter& lhs, Counter& rhs)
	{
		lhs.swap(rhs);
	}

	friend inline bool operator==(const Counter& lhs, const Counter& rhs)
	{
		return cmp(lhs, rhs) == 0;
	}
	friend inline bool operator!=(const Counter& lhs, const Counter& rhs)
	{
		return cmp(lhs, rhs) != 0;
	}
	friend inline bool operator<(const Counter& lhs, const Counter& rhs)
	{
		return cmp(lhs, rhs) < 0;
	}
	friend inline bool operator>(const Counter& lhs, const Counter& rhs)
	{
		return cmp(lhs, rhs) > 0;
	}
	friend inline bool operator<=(const Counter& lhs, const Counter& rhs)
	{
		return cmp(lhs, rhs) <= 0;
	}
	friend inline bool operator>=(const Counter& lhs, const Counter& rhs)
	{
		return cmp(lhs, rhs) >= 0;
	}

	friend inline std::ostream&
	operator<<(std::ostream& os, const Counter& c) // OK
	{
		return os << c.store;
	}

	static inline unsigned long get_assignments()
	{
		return assignments;
	}

	static inline unsigned long get_comparations()
	{
		return comparations;
	}

private:
	static inline int cmp(const Counter& lhs, const Counter& rhs)
	{
		++comparations;
		return -(lhs.store < rhs.store) + (lhs.store > rhs.store);
	}

	T store{};
	static unsigned long comparations;
	static unsigned long assignments;
};

template <>
unsigned long Counter<unsigned>::assignments = 0;
template <>
unsigned long Counter<unsigned>::comparations = 0;

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);

	using Type = Counter<unsigned>;
	Vector<Type> data;
	unsigned x;

	while (std::cin >> x)
		data.push_back(Type(x));

	merge_sort(data.begin(), data.end());
	std::cerr << "Assignments: " << Type::get_assignments() << std::endl;
	std::cerr << "Comparations: " << Type::get_comparations() << std::endl;

	for (const auto& e : data)
		std::cout << e << std::endl;

	return 0;
}
