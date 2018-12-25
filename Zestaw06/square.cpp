#include "Vector.hpp" // for Vector, Vector<>::iterator, Vector<>::va...

#include <cmath> // for sqrt
#include <initializer_list> // for initializer_list
#include <iomanip> // for operator<<, setprecision
#include <iostream> // for operator<<, ostream, endl, istream, cout
#include <iterator> // for distance, next, iterator_traits
#include <limits> // for numeric_limits, numeric_limits<>::max_di...
#include <sstream> // for swap
#include <stdexcept> // for length_error
#include <stdlib.h> // for EXIT_FAILURE, EXIT_SUCCESS
#include <type_traits> // for enable_if_t, is_base_of<Paste>
#include <utility> // for make_pair, operator==, pair

class Point {
public:
	Point(double x, double y) : _x{x}, _y{y}
	{
	}

	~Point() = default;
	Point() = default;
	Point(const Point&) = default;
	Point(Point&&) = default;
	Point& operator=(const Point&) = default;
	Point& operator=(Point&&) = default;

	inline double x() const noexcept
	{
		return _x;
	}

	inline double y() const noexcept
	{
		return _y;
	}

	friend inline bool operator==(const Point& lhs, const Point& rhs)
	{
		return cmp_x(lhs, rhs) == 0 && cmp_y(lhs, rhs) == 0;
	}

	friend inline std::ostream& operator<<(std::ostream& os, const Point& c)
	{
		return os << "(" << c.x() << ", " << c.y() << ")";
	}

	double distance_squared(const Point& other) const
	{
		return (other.x() - x()) * (other.x() - x())
			+ (other.y() - y()) * (other.y() - y());
	}

	double distance(const Point& other) const
	{
		return std::sqrt(distance_squared(other));
	}

	friend inline std::istream& operator>>(std::istream& is, Point& c)
	{
		return is >> c._x >> c._y;
	}

	static inline int cmp_x(const Point& lhs, const Point& rhs) noexcept
	{
		return -(lhs.x() < rhs.x()) + (lhs.x() > rhs.x());
	}

	static inline bool compare_x(const Point& lhs, const Point& rhs) noexcept
	{
		return lhs.x() < rhs.x();
	}

	static inline int cmp_y(const Point& lhs, const Point& rhs) noexcept
	{
		return -(lhs.y() < rhs.y()) + (lhs.y() > rhs.y());
	}

	static inline bool compare_y(const Point& lhs, const Point& rhs) noexcept
	{
		return lhs.y() < rhs.y();
	}

	static inline Point read_point(std::istream& is)
	{
		double x, y;
		is >> x >> y;
		return Point(x, y);
	}

private:
	double _x{0};
	double _y{0};
};

namespace helper {
template <typename Iterator, typename Functor>
inline Iterator
partition(const Iterator begin, const Iterator _end, const Functor is_smaller)
{
	using std::swap;
	// using Type = typename std::remove_reference<decltype(*begin)>::type;
	using Type = typename std::iterator_traits<Iterator>::value_type;

	const auto end = _end - 1;

	/* pick pivot */
	auto mid = std::next(begin, std::distance(begin, end) / 2);
	if (is_smaller(*mid, *begin))
		swap(*begin, *mid);
	if (is_smaller(*end, *begin))
		swap(*begin, *end);
	if (is_smaller(*mid, *end))
		swap(*mid, *end);
	const Type pivot = *mid;
	/* partition */
	auto i = begin;
	auto j = end;
	while (true) {
		while (is_smaller(*i, pivot))
			++i;
		while (is_smaller(pivot, *j))
			--j;

		if (std::distance(i, j) <= 0)
			return std::next(j);

		swap(*i, *j);
		++i;
		--j;
	}
}

template <typename Iterator, typename Functor>
inline void insertion_sort(
	const Iterator begin, const Iterator end, const Functor is_smaller)
{
	for (auto i = begin + 1; i != end; ++i) {
		const auto value = *i;
		auto j = i - 1;
		while (std::distance(begin, j) >= 0 && is_smaller(value, *j)) {
			*(j + 1) = *j;
			--j;
		}
		*(j + 1) = value;
	}
}
}

template <typename Iterator, typename Functor>
inline void quick_sort(
	const Iterator begin,
	const Iterator end,
	const Functor is_smaller,
	const unsigned limit = 16)
{
	if (std::distance(begin, end) < 2)
		return;

	if (std::distance(begin, end) < 16 || limit == 0) {
		helper::insertion_sort(begin, end, is_smaller);
		return;
	}

	const auto pivot = helper::partition(begin, end, is_smaller);
	quick_sort(begin, pivot, is_smaller, limit - 1);
	quick_sort(pivot, end, is_smaller, limit - 1);
}

using Para = std::pair<Point, Point>;

double distance(Para x)
{
	return x.first.distance(x.second);
}

double distance_squared(Para x)
{
	return x.first.distance_squared(x.second);
}

// https://akrzemi1.wordpress.com/2016/03/21/concepts-without-concepts/

template <typename Iterator>
std::enable_if_t<
	std::is_base_of<
		std::forward_iterator_tag,
		typename std::iterator_traits<Iterator>::iterator_category>::value,
	Para>
find_closest_rec(Iterator begin, Iterator end)
{
	// std::cout << __func__ << ": " << begin << ", " << end << std::endl;
	// std::cout << __func__ << ": dist: " << std::distance(begin, end)
	// << std::endl;
	switch (std::distance(begin, end)) {
	case 2:
		// std::cout << __func__ << ": return" << std::endl;
		return std::make_pair(*begin, *(begin + 1));
	case 3: {
		// brute force
		--end;
		const auto mid = begin + 1;
		Para result = std::make_pair(*begin, *end);
		auto min_dist_squared = distance_squared(result);
		if (mid->distance_squared(*end) < min_dist_squared) {
			result = std::make_pair(*mid, *end);
			min_dist_squared = distance_squared(result);
		}
		if (mid->distance_squared(*begin) < min_dist_squared) {
			result = std::make_pair(*begin, *mid);
		}
		// std::cout << __func__ << ": return" << std::endl;
		return result;
	}
	// default:
	case 1:
	case 0:
		throw std::length_error("złe argumenty");
	}

	const auto mid = begin + (end - begin) / 2;

	const auto tmp_result0 = find_closest_rec(begin, mid);
	const double tmp_result0_dist = distance_squared(tmp_result0);
	const auto tmp_result1 = find_closest_rec(mid, end);
	const double tmp_result1_dist = distance_squared(tmp_result1);
	double min_dist_squared;
	Para result;
	if (tmp_result0_dist < tmp_result1_dist) {
		result = tmp_result0;
		min_dist_squared = tmp_result0_dist;
	} else {
		result = tmp_result1;
		min_dist_squared = tmp_result1_dist;
	}

	Vector<Point> strip{*mid};
	// std::cout << __func__ << ": add mid: " << *mid << std::endl;
	{
		double tmp_dist = std::sqrt(min_dist_squared);
		for (auto i = mid - 1; i != begin && mid->x() - i->x() <= tmp_dist;
			 --i) {
			strip.push_back(*i);
			// std::cout << __func__ << ": add left: " << *i << std::endl;
		}
		if (mid->x() - begin->x() <= tmp_dist)
			strip.push_back(*begin);
		for (auto i = mid + 1; i != end && mid->x() - i->x() <= tmp_dist; ++i) {
			strip.push_back(*i);
			// std::cout << __func__ << ": add right: " << *i << std::endl;
		}
	}
	// std::cout << __func__ << ": strip: " << strip.size() << std::endl;

	quick_sort(strip.begin(), strip.end(), Point::compare_y);

	for (auto i = strip.begin(); i != strip.end(); ++i) {
		for (auto j = std::next(i); std::distance(i, j) < 6 && j != strip.end();
			 ++j) {
			auto dist = i->distance_squared(*j);
			if (dist < min_dist_squared) {
				result = std::make_pair(*i, *j);
				min_dist_squared = dist;
			}
		}
	}

	// std::cout << __func__ << ": return" << std::endl;
	return result;
}

Para find_closest(Vector<Point> v)
{
	quick_sort(v.begin(), v.end(), Point::compare_x);
	// for (const auto& i : v)
	// 	std::cout << i << std::endl;

	return find_closest_rec(v.begin(), v.end());
	// return find_closest_rec(&v.front(), &v.back() + 1);
}

int main(int, char**)
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::setprecision(std::numeric_limits<double>::max_digits10);

	Vector<Point> data;
	Point p;

	while (std::cin >> p)
		data.push_back(p);

	Para ans = find_closest(data);

	std::cout << ans.first << " and " << ans.second << std::endl;
	std::cout << distance(ans) << std::endl;

	std::cout << "Brute force:" << std::endl;

	double min_dist_squared = std::numeric_limits<double>::infinity();
	Para brute_force;
	for (auto i = data.begin(); i != data.end(); ++i) {
		// std::cout << *i << std::endl;
		for (auto j = std::next(i); j != data.end(); ++j) {
			// std::cout << "\t" << *j;
			double tmp = i->distance_squared(*j);
			if (tmp < min_dist_squared) {
				// std::cout << "+";
				min_dist_squared = tmp;
				brute_force = std::make_pair(*i, *j);
			}
			// std::cout << std::endl;
		}
	}
	std::cout << brute_force.first << " and " << brute_force.second
			  << std::endl;
	std::cout << distance(brute_force) << std::endl;

	if (distance(brute_force) > distance(ans)) {
		std::cout << "brute_force > ans" << std::endl;
		return EXIT_FAILURE;
	}
	if (brute_force == ans
		|| (ans.first == brute_force.second
			&& ans.second == brute_force.first)) {
		std::cout << " *** OK *** " << std::endl;
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}
