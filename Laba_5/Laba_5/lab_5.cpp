#include <iostream>
#include <assert.h>
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
3. Доступ в конец, начало        //NO
4. Доступ по индексу             //OK
5. Изменение капасити            //NO
//TODO: исключения и итератор произвольного доступа

*/


template<unsigned Size, class D_Type = int>
class CircularBuffer {
public:
	typedef uint16_t Ind_t;

	CircularBuffer() {}
	~CircularBuffer() {}

	bool IsEmpty() {
		return Size - 1 == first_ind_ + last_ind_ - 1;
	}

	//TODO: create methods and capasity
	D_Type& operator[] (Ind_t i) {
		return data_[(first_ind_ + i + 1) % capacity_];
	}
	const D_Type& operator[] (Ind_t i) const {
		return data_[(first_ind_ + i + 1) % capacity_];
	}
	
	void push_back(const D_Type& v){
		data_[last_ind_] = v;
		last_ind_ = (last_ind_ + 1) % capacity_;
		++curr_size_;
	}
	void push_first(const D_Type& v) {
		data_[first_ind_] = v;
		first_ind_ = (first_ind_ - 1 + capacity_) % capacity_;
		++curr_size_;
	}

	D_Type& pop_back() {//допилить исключения
		--curr_size_;
		assert(curr_size_ >= 0);
		//if (curr_size_ < 0)
			//std::cerr << "You ran out of memory!" << '\n';
		last_ind_ = (last_ind_ - 1 + capacity_) % capacity_;
		return data_[last_ind_];
	}
	D_Type& pop_first() {
		--curr_size_; 
		assert(curr_size_ >= 0);
		//if (curr_size_ < 0)
			//std::cout << "You ran out of memory!" << '\n';
		first_ind_ = (first_ind_ + 1) % capacity_;
		return data_[first_ind_];
	}

	int* begin() {
		std::cout << (first_ind_) % capacity_ << '\n';
		return &data_[(first_ind_ + 1) % capacity_];
	}
	int* end() {
		std::cout << last_ind_ << '\n';
		return &data_[last_ind_];
	}

	void change_capacity(unsigned new_size) {

	}


private:
	Ind_t first_ind_ = 0;
	Ind_t last_ind_ = 1;
	long curr_size_ = 0;

	Ind_t capacity_ = Size + 1;
	D_Type data_[Size + 1];
};

int main() {
	CircularBuffer<6> b;


	b.push_back(1);
	b.push_back(2);
	b.push_first(3);
	b.push_first(4);
	b.push_first(5);
	b.push_first(6);


	for (int i = 0; i < 6; ++i)
		std::cout << b[i] << ' ';
	std::cout << '\n';

	for (int i = 0; i < 6; ++i)
		std::cout << b.pop_first() << ' ';
	std::cout << '\n';

	std::cout << b.begin() << " " << b.end() << '\n';

	return 0;
}
