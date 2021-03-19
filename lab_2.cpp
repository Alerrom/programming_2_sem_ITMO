#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

/*
Реализовать конструктор(ы), конструктор копирования, деструктор, а также следующие
операторы:
1. = (да)
2. ==, != (да оба)
3. +(да), - (унарный и бинарный)(да), +=, -= (все)
4. *(да), / (на число)(да), *=, /= (все)
5. << (да), >>
6. [] (для получения коэффициента i-го члена)(да)
*/

class CPolynom {
public:
	CPolynom(const std::vector<float>& Polynom = {0})
		: Polynom_(delete_zeros(Polynom))
		, deg_(Polynom_.size() - 1)
	{}

	CPolynom(const CPolynom& other)
		: Polynom_(other.Polynom_)
		, deg_(other.deg_)
	{}

	CPolynom& operator=(const CPolynom& other) {
		if (&other == this) {
			return *this;
		}

		Polynom_ = other.Polynom_;
		deg_ = other.deg_;

		return *this;
	}

	~CPolynom() = default;

	friend bool operator==(const CPolynom& lhs, const CPolynom& rhs);
	friend bool operator!=(const CPolynom& lhs, const CPolynom& rhs);

	CPolynom operator+(const CPolynom& pol) {
		std::vector<float> pol_;

		for (unsigned i = 0; i < std::min(deg_, pol.degree()) + 1; ++i) {
			pol_.push_back(Polynom_[i] + pol[i]);
		}

		if (deg_ != pol.degree()) {
			for (unsigned i = std::min(deg_, pol.degree()) + 1; i < std::max(deg_, pol.degree()) + 1; ++i) {
				if (deg_ > pol.degree())
					pol_.push_back(Polynom_[i]);
				else if (pol.degree() > deg_)
					pol_.push_back(pol.Polynom_[i]);
			}
		}

		CPolynom ans(pol_);
		return ans;
	}

	CPolynom operator-(const CPolynom& pol) {
		std::vector<float> pol_;

		for (unsigned i = 0; i < std::min(deg_, pol.degree()) + 1; ++i) {
			pol_.push_back(Polynom_[i] - pol[i]);
		}

		if (deg_ != pol.degree()) {
			for (unsigned i = std::min(deg_, pol.degree()) + 1; i < std::max(deg_, pol.degree()) + 1; ++i) {
				if (deg_ > pol.degree())
					pol_.push_back(Polynom_[i]);
				else if (pol.degree() > deg_)
					pol_.push_back(-pol.Polynom_[i]);
			}
		}

		CPolynom ans(pol_);
		return ans;
	}

	CPolynom operator-() {
		std::vector<float> pol_;

		for (unsigned i = 0; i < deg_ + 1; ++i) {
			pol_.push_back(-Polynom_[i]);
		}

		CPolynom ans(pol_);
		return ans;
	}

	CPolynom operator*(const CPolynom& pol) {
		std::vector<float> tmp(pol.deg_ + deg_ + 1, 0);

		for (unsigned i = 0; i <= deg_; ++i) {
			for (unsigned j = 0; j <= pol.deg_; ++j) {
				tmp[i + j] += Polynom_[i] * pol[j];
			}
		}

		CPolynom ans(tmp);

		return ans;
	}

	CPolynom operator*(float c) {
		std::vector<float> pol_ = Polynom_;

		for (unsigned i = 0; i < deg_ + 1; ++i) {
			pol_[i] *= c;
		}

		CPolynom ans(pol_);
		return ans;
	}

	CPolynom operator/(float c) {
		assert(c != 0);

		std::vector<float> pol_ = Polynom_;

		for (unsigned i = 0; i < deg_ + 1; ++i) {
			pol_[i] /= c;
		}

		CPolynom ans(pol_);
		return ans;
	}

	friend CPolynom& operator+=(CPolynom& lhs, const CPolynom& rhs);
	friend CPolynom& operator-=(CPolynom& lhs, const CPolynom& rhs);
	friend CPolynom& operator*=(CPolynom& lhs, const CPolynom& rhs);
	friend CPolynom& operator*=(CPolynom& lhs, float c);
	friend CPolynom& operator/=(CPolynom& lhs, float c);

	friend std::ostream& operator<<(std::ostream& stream, const CPolynom& value);
	friend std::istream& operator>>(std::istream& stream, CPolynom& value);

	float& operator[](unsigned i) {
		bool flag = (i >= 0) && (i <= deg_);
		assert(flag);

		return Polynom_[i];
	}

	const float& operator[](unsigned i) const {
		bool flag = (i >= 0) && (i <= deg_);
		assert(flag);

		return Polynom_[i];
	}

	unsigned degree() {
		return deg_;
	}

	const unsigned degree() const {
		return deg_;
	}

private:
	std::vector<float> Polynom_;
	unsigned deg_;

	std::vector<float> delete_zeros(const std::vector<float>& pol) {
		std::vector<float> tmp = pol;

		if (pol.size() == 1)
			return tmp;
		else {
			unsigned i = pol.size() - 1;

			while (i > 0 && pol[i] == 0) {
				tmp.pop_back();
				i--;
			}

			return tmp;
		}
	}
};

bool operator==(const CPolynom& lhs, const CPolynom& rhs) {
	return (lhs.Polynom_ == rhs.Polynom_);
}

bool operator!=(const CPolynom& lhs, const CPolynom& rhs) {
	return !(lhs == rhs);
}

CPolynom& operator+=(CPolynom& lhs, const CPolynom& rhs) {
	lhs = lhs + rhs;
	return lhs;
}

CPolynom& operator-=(CPolynom& lhs, const CPolynom& rhs) {
	lhs = lhs - rhs;
	return lhs;
}

CPolynom& operator*=(CPolynom& lhs, const CPolynom& rhs) {
	lhs = lhs * rhs;
	return lhs;
}

CPolynom& operator*=(CPolynom& lhs, float c) {
	lhs = lhs * c;
	return lhs;
}

CPolynom& operator/=(CPolynom& lhs, float c) {
	lhs = lhs / c;
	return lhs;
}

std::ostream& operator<<(std::ostream& stream, const CPolynom& polynom) {
	if (polynom.deg_ == 0) {
		stream << polynom.Polynom_[0];
		return stream;
	}

	for (unsigned i = 0; i <= polynom.deg_; ++i) {
		if (polynom.Polynom_[i] > 0) {
			if (i == 1 && polynom.Polynom_[i - 1] == 0)
				continue;
			else if (i > 0 && i <= polynom.deg_)
				stream << " + ";
			stream << polynom.Polynom_[i];
			if (i >= 2)
				stream << "x^" << i;
			else if (i == 1)
				stream << "x";
		}
		else if (polynom.Polynom_[i] < 0) {
			if (i == 1 && polynom.Polynom_[i - 1] == 0)
				continue;
			else if (i > 0 && i <= polynom.deg_)
				stream << " - ";
			else if (i == 0)
				stream << "-";
			stream << std::abs(polynom.Polynom_[i]);
			if (i >= 2)
				stream << "x^" << i;
			else if (i == 1)
				stream << "x";
		}
	}
	return stream;
}

std::istream& operator>>(std::istream& stream, CPolynom& polynom) {
	std::cout << "Enter polynom degree: ";
	long deg;
	stream >> deg;

	assert(deg >= 0);

	polynom.deg_ = deg;

	std::cout << "Enter " << polynom.deg_ + 1 << " coefs: ";
	polynom.Polynom_.resize(polynom.deg_ + 1);
	for (unsigned i = 0; i <= polynom.deg_; ++i)
		stream >> polynom[i];

	return stream;
}

int main() {
	CPolynom p1({1,0,0,0,2,0,0,0,0});
	CPolynom p2({1, -2, 1});
	CPolynom p3;

	std::cout << "p1 = " << p1 << std::endl;
	std::cout << "p2 = " << p2 << std::endl;
	std::cout << "p3 = " << p3 << std::endl;

	std::cout << "p1 + p3 = " << p1 + p3 << std::endl;
	std::cout << "p3 + p1 = " << p3 + p1 << std::endl;
	std::cout << "p1 + p2 = " << p1 + p2 << std::endl;
	std::cout << "p2 + p1 = " << p2 + p1 << std::endl;

	std::cout << "p1 - p3 = " << p1 - p3 << std::endl;
	std::cout << "p3 - p1 = " << p3 - p1 << std::endl;
	std::cout << "p1 - p2 = " << p1 - p2 << std::endl;
	std::cout << "p3 - p1 = " << p3 - p1 << std::endl;
	std::cout << "-p1 = " << -p1 << std::endl;

	std::cout << "p1 * p3 = " << p1 * p3 << std::endl;
	std::cout << "p3 * p1 = " << p3 * p1 << std::endl;
	std::cout << "p1 * p2 = " << p1 * p2 << std::endl;
	std::cout << "p2 * p1 = " << p2 * p1 << std::endl;
	std::cout << "p1 * 0 = " << p1 * 0 << std::endl;

	std::cout << "p1 / 5 = " << p1 / 5 << std::endl;

	p1 *= p1;

	std::cout << "p1 += p3 is " << p1 << std::endl;

	std::cin >> p3;

	std::cout << p3 << std::endl;

	return 0;
}