#include <iostream>
#include <vector>

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


int main() {
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

	return 0;
}