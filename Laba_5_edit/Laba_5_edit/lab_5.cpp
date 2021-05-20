#include <iostream>
#include <stdexcept>
#include <vector>
#include <deque>

/*
Реализовать кольцевой буфер в виде stl-совместимого
контейнера (например, может быть использован с стандартными
алгоритмами), обеспеченного итератором произвольного доступа.
Реализация не должна использовать ни одни из контейнеров STL.

Буфер должен обладать следующими возможностями:
1. Вставка и удаление в конец    //OK
2. Вставка и удаление в начало   //OK
3. Доступ в конец, начало        //OK
4. Доступ по индексу             //OK
5. Изменение капасити            //OK
//TODO: исключения и итератор произвольного доступа

*/

template<class D_Type = int>
class CircularBuffer {
public:
	CircularBuffer(int capacity) 
		: size_(capacity)
		, capacity_(capacity + 1)
	{}
	~CircularBuffer() {
		delete data_;
	}

	bool empty() {
		return curr_size_ == capacity_ - 1;
	}

	D_Type& operator[] (int i) {//negative index
		if (i < 0)
			return data_[(front_ind_ + i) % capacity_];
		return data_[(front_ind_ + i + 1) % capacity_];
	}
	const D_Type& operator[] (int i) const {
		if (i < 0)
			return data_[(front_ind_ + i) % capacity_];
		return data_[(front_ind_ + i + 1) % capacity_];
	}
	
	void push_back(const D_Type& v) {
		if (curr_size_ + 1 == capacity_)
			change_capacity(2 * curr_size_);
		data_[last_ind_] = v;
		last_ind_ = (last_ind_ + 1) % capacity_;
		++curr_size_;
	}
	void push_front(const D_Type& v) {
		if (curr_size_ + 1 == capacity_)
			change_capacity(2 * curr_size_);
		data_[front_ind_] = v;
		front_ind_ = (front_ind_ - 1 + capacity_) % capacity_;
		++curr_size_;
	}

	D_Type& pop_back() {
		--curr_size_;
		last_ind_ = (last_ind_ - 1 + capacity_) % capacity_;
		return data_[last_ind_];
	}
	D_Type& pop_front() {
		--curr_size_; 
		front_ind_ = (front_ind_ + 1) % capacity_;
		return data_[front_ind_];
	}

	D_Type* begin() {
		return &data_[(front_ind_ + 1) % capacity_];
	}
	D_Type* end() {
		return &data_[last_ind_];
	}

	void change_capacity(int new_capasity) {
		D_Type *new_data = new D_Type[new_capasity + 1];

		for (int i = 0; i < curr_size_; ++i) {
			if (i == new_capasity)
				break;
			new_data[i] = data_[(front_ind_ + i + 1) % capacity_];
		}
		delete data_;

		data_ = new_data;
		front_ind_ = new_capasity; 
		last_ind_ = (new_capasity > curr_size_) ? curr_size_ : new_capasity;
		capacity_ = new_capasity + 1;
	}


private:
	int front_ind_ = 0;
	int last_ind_ = 1;
	int curr_size_ = 0;
	int size_;

	int capacity_;
	D_Type *data_ = new D_Type[size_ + 1];
};

template<class T>
class Iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using different_types = std::ptrdiff_t;
	using value_type = T;
	using pointer = value_type*;
	using reference = value_type&;
};

int main() {
	CircularBuffer<int> b(6);


	b.push_back(1);
	b.push_back(2);
	b.push_front(3);
	b.push_front(4);
	b.push_front(5);
	b.push_front(6);
	b.push_back(100500);


	for (int i = 0; i < 6; ++i)
		std::cout << b[i] << ' ';
	std::cout << '\n';


	b.change_capacity(10);

	for (int i = 0; i < 10; ++i)
		std::cout << b[i] << ' ';
	std::cout << '\n';

	
	std::cout << b[-4] << std::endl;
	std::cout << b.begin() << " " << b.end() << '\n';

	
	for (int i = 0; i < 6; ++i)
		std::cout << b.pop_front() << ' ';
	std::cout << '\n';
	

	return 0;
}
