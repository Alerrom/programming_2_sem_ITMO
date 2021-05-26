#include <iostream>
#include <algorithm>
#include <exception>


template <typename T>
class Iterator {
public:
	using difference_type = std::ptrdiff_t;

	Iterator(T* _data, int _ring_size, int _index, int _head)
		: data_(_data)
		, capacity_(_ring_size)
		, ind_(_index)
		, front_(_head)
	{}

	T* address() { return std::addressof(data_[(front_ + ind_) % capacity_]); }

	bool operator==(const Iterator& rhs) const { return ind_ == rhs.ind_; }
	bool operator!=(const Iterator& rhs) const { return !operator==(rhs); }

	T& operator*() { return data_[(front_ + ind_) % capacity_]; }
	T* operator->() { return  data_ + (front_ + ind_) % capacity_; }

	Iterator& operator++() {
		ind_++;
		return *this;
	}

	Iterator& operator--() {
		ind_--;
		return *this;
	}

	Iterator  operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
	Iterator  operator--(int) { Iterator tmp = *this; --(*this); return tmp; }

	Iterator operator+(difference_type n) const { Iterator tmp = *this; return tmp.operator+=(n); }
	Iterator operator-(difference_type n) const { Iterator tmp = *this; return tmp.operator-=(n); }
	friend Iterator operator+(difference_type n, const Iterator& it) { return it.operator+(n); }
	difference_type operator-(const Iterator& rhs) const { return ind_ - rhs.ind_; }

	bool operator< (const Iterator& rhs) const { return ind_ < rhs.ind_; }
	bool operator> (const Iterator& rhs) const { return ind_ > rhs.ind_; }
	bool operator<=(const Iterator& rhs) const { return ind_ <= rhs.ind_; }
	bool operator>=(const Iterator& rhs) const { return ind_ >= rhs.ind_; }

	Iterator& operator+=(difference_type n)	{
		operator-=(-n);
		return *this;
	}

	Iterator& operator-=(difference_type n)	{
		ind_ -= n;
		return *this;
	}

	T& operator[](difference_type n) {
		return *(Iterator{ *this }.operator+=(n));
	}

private:
	T* data_;
	int front_;
	int ind_;
	int capacity_;
};

/*
template<class T>
class Iterator {
public:
	Iterator(T* data, int front, int capacity, int ind)
		: data_(data)
		, front_(front)
		, capacity_(capacity)
		, ind_(ind)
	{}
	~Iterator() {}

	Iterator& operator+=(int rhs) { 
		ind_ += rhs; 
		ind_ %= capacity_; 
		return *this; 
	}
	Iterator& operator-=(int rhs) { 
		ind_ -= rhs; 
		ind_ = (ind_ + capacity_) % capacity_; 
		return *this; 
	}
	T* operator*() const { 
		return data_ + (front_ + ind_) % capacity_; 
	}
	T* operator->() const { 
		return data_ + (front_ + ind_) % capacity_; 
	}
	T& operator[](int rhs) const { 
		return &data_[(front_ + rhs + 1) % capacity_]; 
	}

	Iterator& operator++() { 
		++ind_; 
		ind_ %= capacity_; 
		return *this; 
	}
	Iterator& operator--() { 
		--ind_; 
		ind_ = (ind_ + capacity_) % capacity_; 
		return *this; 
	}
	Iterator operator++(int) const { 
		Iterator tmp(*this); 
		++ind_; 
		ind_ %= capacity_; 
		return tmp; }
	Iterator operator--(int) const { 
		Iterator tmp(*this); 
		--ind_; 
		ind_ = (ind_ + capacity_) % capacity_; 
		return tmp; 
	}
	int operator-(const Iterator& rhs) const { 
		return Iterator(data_, front_, capacity_, (ind_ - rhs.ind_ + capacity_) % capacity_); 
	}
	Iterator operator+(int rhs) const {
		return Iterator(data_, front_, capacity_, (ind_ + rhs) % capacity_); 
	}
	Iterator operator-(int rhs) const { 
		return Iterator(data_, front_, capacity_, (ind_ - rhs + capacity_) % capacity_);
	}

	bool operator==(const Iterator& rhs) const { 
		return data_[ind_] == rhs.data_[ind_];
	}
	bool operator!=(const Iterator& rhs) const { 
		return data_[ind_] != rhs.data_[ind_];
	}
	bool operator>(const Iterator& rhs) const { 
		return data_[ind_] > rhs.data_[ind_];
	}
	bool operator<(const Iterator& rhs) const {
		return data_[ind_] < rhs.data_[ind_];
	}
	bool operator>=(const Iterator& rhs) const { 
		return data_[ind_] >= rhs.data_[ind_];
	}
	bool operator<=(const Iterator& rhs) const { 
		return data_[ind_] <= rhs.data_[ind_];
	}

private:
	T* data_;
	int front_;
	int ind_;
	int capacity_;
};
*/

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

	Iterator<T> begin();
	Iterator<T> end();
	Iterator<const T> begin() const;
	Iterator<const T> end() const;

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

template<class T>
Iterator<const T> CircularBuffer<T>::begin() const {
	return Iterator<const T>{ data_, capacity_, 0, front_ind_ };
}

template<class T>
Iterator<const T> CircularBuffer<T>::end() const {
	return Iterator<const T>{ data_, capacity_, size_, front_ind_ };
}

template<class T>
Iterator<T> CircularBuffer<T>::begin() {
	return Iterator<T>{ data_, capacity_, 0, front_ind_ };
}

template<class T>
Iterator<T> CircularBuffer<T>::end() {
	return Iterator<T>{ data_, capacity_, size_, front_ind_ };
}


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
	for (int i = 0; i < 8; ++i)
		std::cout << b[i] << ' ';

	std::cout << std::endl;

	std::sort(b.begin(), b.end());


	for (int i = 0; i < 5; ++i)
		std::cout << b[i] << ' ';


	return 0;
}