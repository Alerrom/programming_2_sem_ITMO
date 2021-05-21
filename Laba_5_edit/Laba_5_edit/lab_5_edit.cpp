#include <iostream>
#include <algorithm>
#include <exception>

template<class T = int>
class CircularBuffer {
public:
	CircularBuffer(int capacity = 0)
		:capacity_(capacity + 1) {
		data_ = new T[capacity_ + 1];
	}
	~CircularBuffer() {
		delete data_;
	}

	void push_back(const T& v) {
		data_[back_ind_] = v;
		back_ind_ = (back_ind_ + 1) % capacity_;
		if (size_ == capacity_ - 1)
			front_ind_ = (front_ind_ + 1) % capacity_;
		else
			++size_;
	}
	void push_front(const T& v) {
		data_[front_ind_] = v;
		front_ind_ = (front_ind_ - 1 + capacity_) % capacity_;
		if (size_ == capacity_ - 1)
			back_ind_ = (back_ind_ + 1) % capacity_;
		else
			++size_;
	}

	T& pop_back() {
		--size_;
		back_ind_ = (back_ind_ - 1 + capacity_) % capacity_;
		return data_[back_ind_];
	}
	T& pop_front() {
		--size_;
		front_ind_ = (front_ind_ + 1) % capacity_;
		return data_[front_ind_];
	}


	T& operator[] (int i) {
		//if (abs(i) >= capacity_)
		//	throw std::out_of_range("Think about");

		return data_[(front_ind_ + i + 1) % capacity_];
	}
	const T& operator[] (int i) const {
		//if (abs(i) >= capacity_)
		//	throw std::out_of_range("Think about");

		return data_[(front_ind_ + i + 1) % capacity_];
	}


	T* begin() {//iterator
		//return Iterator<T>(data_, front_ind_, 1, capacity_);
		return &data_[(front_ind_ + 1) % capacity_];
	}
	T* end() {//iterator
		//return Iterator<T>(data_, front_ind_, back_ind_ + 1, capacity_);
		return &data_[back_ind_];
	}

	void change_capacity(int n) {
		T *new_data_ = new T[n];
		for (int i = 0; i < size_; ++i) {
			if (i == n)
				break;
			new_data_[i] = data_[(front_ind_ + i + 1) % capacity_];
		}

		delete[] data_;
		data_ = new_data_;


		size_ = std::min(size_, n);
		front_ind_ = size_;
		back_ind_ = size_ + 1;
		capacity_ = n + 1;
	} 

private:
	T *data_;

	int capacity_;
	int size_ = 0;
	int front_ind_ = 0;
	int back_ind_ = 1;
};

/*
template<class T>
class Iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using different_types = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;
private:
	T* data_ptr_;
	int front_;
	int back_;
	int capacity_;
public:
	Iterator(T* data_ptr, int front, int back, int capacity)
		: data_ptr_(data_ptr)
		, front_(front)
		, back_(back)
		, capacity_(capacity)
	{}
	~Iterator() {}

};
*/
int main() {
	CircularBuffer<int> b(6);


	b.push_back(1);
	b.push_back(2);
	b.push_front(3);
	b.push_front(4);
	b.push_front(5);
	b.push_front(6);


	for (int i = 0; i < 6; ++i)
		std::cout << b[i] << ' ';

	std::cout << std::endl;
	b.push_back(1010);

	for (int i = 0; i < 6; ++i)
		std::cout << b[i] << ' ';

	std::cout << std::endl;
	b.push_front(10220);

	for (int i = 0; i < 6; ++i)
		std::cout << b[i] << ' ';

	b.change_capacity(5);

	std::cout << std::endl;
	for (int i = 0; i < 5; ++i)
		std::cout << b[i] << ' ';

	std::cout << std::endl;

	std::cout << b.begin() << ' ' << b.end() << std::endl;

	return 0;
}