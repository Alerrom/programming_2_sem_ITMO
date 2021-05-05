#include <iostream>
#include <vector>

#include <sstream>
#include <assert.h>
#include <set>
#include <iterator>
#include <functional>

template<class InputIterator, class UnaryPredicate>//OK
bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
	while (first != last) {
		if (!pred(*first))
			return false;
		++first;
	}
	return true;
}

template<class InputIterator, class UnaryPredicate>//OK
bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
	while (first != last) {
		if (pred(*first))
			return true;
		++first;
	}
	return false;
}

template<class InputIterator, class UnaryPredicate>//OK
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
	while (first != last) {
		if (pred(*first))
			return false;
		++first;
	}
	return true;
}

template<class InputIterator, class UnaryPredicate>//OK
bool one_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
	bool flag = false;
	while (first != last) {
		if (flag && pred(*first))
			return false;

		if (pred(*first) && !flag)
			flag = true;
		++first;
	}
	return true;
}

template <class ForwardIterator, class Predicate>//OK
bool is_sorted(ForwardIterator first, ForwardIterator last, Predicate pred) {
	if (first == last)
		return true;
	ForwardIterator next = first;
	while (++next != last) {
		if (!pred(*next, *first))
			return false;
		++first;
	}
	return true;
}

template <class InputIterator, class UnaryPredicate>//OK
bool is_partitioned(InputIterator first, InputIterator last, UnaryPredicate pred) {
	if (first == last)
		return false;

	if (pred(*first)) {
		for (; first != last; ++first)
			if (!pred(*first))
				break;
		for (; first != last; ++first)
			if (pred(*first))
				return false;
		return true;
	}
	else {
		for (; first != last; ++first)
			if (pred(*first))
				break;
		for (; first != last; ++first)
			if (!pred(*first))
				return false;
		return true;
	}
}

template<class InputIterator, class T>//OK
InputIterator find_not(InputIterator first, InputIterator last, const T& val) {
	while (first != last) {
		if (*first != val)
			return first;
		++first;
	}
	return last;
}

template<class InputIterator, class T>//OK
InputIterator find_backward(InputIterator first, InputIterator last, const T& val) {
	if (first == last)
		return last;

	--last;
	while (last != first) {
		if (*last != val)
			return last;
		--last;
	}
	return first;
}

template<class InputIterator, class UnaryPredicate>//OK
bool is_palindrome(InputIterator first, InputIterator last, UnaryPredicate pred) {
	if (first == last)
		return true;

	--last;
	while (first <= last) {
		if (!(pred(*first) && pred(*last)))
			return false;
		++first;
		--last;
	}
	return true;
}


bool pos(int i) {
	return i > 0;
}

bool even(int i) {
	return i % 2 == 0;
}

struct CPoint {
	double x;
	double y;
};

double rad_dist(CPoint& point) {
	return sqrt(pow(point.x, 2) + pow(point.y, 2));
}

bool rad_vect_more_5(CPoint& point) {
	if (rad_dist(point) > 5)
		return true;
	else
		return false;
}

bool less(int i, int j) {
	return i < j;
}


using namespace std;

class myForwardIterator : public vector<int>::iterator {
	using T = vector<int>::iterator;
	using T::T;
public:

	auto operator--(int) const = delete;
	auto operator--() const = delete;
	auto operator-(difference_type __n) const = delete;
	myForwardIterator(vector<int>::iterator it) : T(it) {}
};

void check() {
	vector<int> a{ 1, 2, 3, 4 };
	vector<int> b{ 1, 2, 3, 4, 2 };
	vector<int> c{ 4, 3, 2, 1, 0 };
	vector<int> d{ 1, 1 };
	vector<int> e{ 1, 2, 3, 4, 3, 2, 1 };
	vector<int> f{ 1, 2, 3, 4, 3, 2, 3 };
	vector<int> e1{ 1, 2, 3, 3, 2, 1 };
	vector<int> f1{ 1, 2, 3, 4, 2, 1 };
	vector<pair<int, int>> h{ {1, 2}, {2, 3}, {3, 4} };




	vector<int> g{ 1 };
	set<int> s{ 1, 2, 3 };

	assert(all_of(a.begin(), a.end(), [](int x) {return x <= 5; }));
	assert(!all_of(a.begin(), a.end(), [](int x) {return x <= 3; }));
	assert(any_of(a.begin(), a.end(), [](int x) {return x == 3; }));
	assert(!any_of(a.begin(), a.end(), [](int x) {return x == 5; }));
	assert(none_of(a.begin(), a.end(), [](int x) {return x > 5; }));
	assert(!none_of(a.begin(), a.end(), [](int x) {return x >= 3; }));
	assert(one_of(b.begin(), b.end(), [](int x) {return x == 3; }));
	assert(!one_of(b.begin(), b.end(), [](int x) {return x == 2; }));
	assert(is_sorted(a.begin(), a.end(), [](int x, int y) {return x < y; }));
	assert(!is_sorted(b.begin(), b.end(), [](int x, int y) {return x < y; }));
	assert(is_sorted(c.begin(), c.end(), [](int x, int y) {return x > y; }));
	assert(is_partitioned(a.begin(), a.end(), [](int x) {return x <= 3; }));
	assert(!is_partitioned(a.begin(), a.end(), [](int x) {return x % 2 == 0; }));
	assert(is_partitioned(a.begin(), a.end(), [](int x) {return x <= 3; }));
	assert(find_not(a.begin(), a.end(), 1) == ++a.begin());
	assert(find_not(d.begin(), d.end(), 1) == d.end());
	assert(find_backward(static_cast<myForwardIterator>(b.begin()), static_cast<myForwardIterator>(b.end()), 2) == --b.end());
	assert(find_backward(static_cast<myForwardIterator>(b.begin()), static_cast<myForwardIterator>(b.end()), 5) == b.end());
	assert(is_palindrome(e.begin(), e.end(), [](int x, int y) {return x == y; }));
	assert(!is_palindrome(f.begin(), f.end(), [](int x, int y) {return x == y; }));
	assert(is_palindrome(e1.begin(), e1.end(), [](int x, int y) {return x == y; }));
	assert(!is_palindrome(f1.begin(), f1.end(), [](int x, int y) {return x == y; }));
	assert(is_palindrome(s.begin(), s.end(), [](int x, int y) {return x + y > 0; }));
	assert(is_palindrome(g.begin(), g.end(), [](int x, int y) {return x == y; }));
	assert(is_sorted(h.begin(), h.end(), [](pair<int, int> x, pair<int, int> y) {return x < y; }));
}

int main() {
	/*
	std::vector<int> v = { 1, 3, 33, 0, -8, -8, -8, -8 };
	std::vector<int> v_1 = { -5, -9, -7, 0, 2, 4, 6 };

	std::vector<CPoint> points = { {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6} };


	std::cout << "For std::vector\n";
	std::cout << "all_of " << all_of(v.begin(), v.end(), pos) << '\n';
	std::cout << "any_of " << any_of(v.begin(), v.end(), even) << '\n';
	std::cout << "none_of " << none_of(v.begin(), v.end(), even) << '\n';
	std::cout << "one_of " << one_of(v.begin(), v.end(), even) << '\n';
	std::cout << "is_sorted " << is_sorted(v.begin(), v.end(), less) << '\n';
	std::cout << "is_partitioned " << is_partitioned(v.begin(), v.end(), pos) << '\n';
	std::cout << "is_partitioned for v_1 " << is_partitioned(v_1.begin(), v_1.end(), pos) << '\n';
	std::vector<int>::iterator p = find_not(v.begin(), v.end(), 1);
	std::cout << "find_not " << *p << '\n';
	p = find_backward(v.begin(), v.end(), 7);
	std::cout << "find_backward " << *p << '\n';
	std::cout << "is_palindrome " << is_palindrome(v.begin(), v.end(), pos) << '\n';


	std::cout << "\nFor CPoint\n";
	std::cout << all_of(points.begin(), points.end(), rad_vect_more_5) << '\n';
	*/

	check();

	return 0;
}