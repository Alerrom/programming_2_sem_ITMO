#include <iostream>
#include <vector>

/*
is_palindrome - возвращает true, если заданна€ последовательность €вл€етс€
палиндромом относительно некоторого услови€. »наче false.
*/

/*
 аждый алгоритм должен быть выполнен в виде шаблонной функции,
позвол€ющей взаимодействовать со стандартными контейнерами STL с
помощью итераторов. ѕредикаты, услови€, операторы сравнени€
должны быть параметризованы.
ѕри сдаче работы требуетс€ продемонстрировать работу алгоритмов как
на стандартных, так и на пользовательских типах данных, например
CPoint, CRational, далее работает ваша индивидуальна€ (не
Уколлективна€Ф) фантази€.
*/

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

template<class InputIterator, class UnaryPredicate>//TODO: придумать, как отказатьс€ от flag
bool one_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
    int flag = 0;
    while (first != last) {
        if (pred(*first))
            flag++;
        ++first;
    }
    return flag == 1;
}

//TODO:переделать шаблоны и придумать реализацию функций
/*
template <class ForwardIterator>//TODO: добавить условие пользовател€
bool is_sorted(ForwardIterator first, ForwardIterator last) {
    if (first == last) 
        return true;
    ForwardIterator next = first;
    while (++next != last) {
        if (*next < *first)     // or, if (comp(*next,*first)) for version (2)
            return false;
        ++first;
    }
    return true;
}
*/

/*
template < class InputIt, class UnaryPredicate>
bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first)
        if (!p(*first))
            break;
    for (; first != last; ++first)
        if (p(*first))
            return  false;
    return  true;
}
*/

template<class InputIterator, class T>//Check
InputIterator find_not(InputIterator first, InputIterator last, const T& val) {
    while (first != last) {
        if (*first != val) 
            return first;
        ++first;
    }
    return last;
}

template<class InputIterator, class T>//Check
InputIterator find_backward(InputIterator first, InputIterator last, const T& val) {
    while (last != first) {
        if (*last != val)
            return last;
        --last;
    }
    return first;
}

//TODO: create function!!!

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};

    /*
    if (all_of(v.begin(), v.end(), [](int i) { return i > 0; }))
        std::cout << "All numbers are positive" << '\n';
    else
        std::cout << "It's wrong" << '\n';

    if (any_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; }))
        std::cout << "Some numbers are even" << '\n';
    else
        std::cout << "It's wrong" << '\n';

    if (none_of(v.begin(), v.end(), [](int i) { return i < 0; }))
        std::cout << "All numbers are negative" << '\n';
    else
        std::cout << "It's wrong" << '\n';

    if (one_of(v.begin(), v.end(), [](int i) { return i == 5; }))
        std::cout << "One number is 5" << '\n';
    else
        std::cout << "It's wrong" << '\n';
    */


	return 0;
}