#include <iostream>
#include <vector>
#include <list>

class Cube {
public:
	Cube() {
		sides[0] = 0x00000000;
		sides[1] = 0x11111111;
		sides[2] = 0x22222222;
		sides[3] = 0x33333333;
		sides[4] = 0x44444444;
		sides[5] = 0x55555555;
	}

	Cube(const Cube& cube) {
		for (size_t i = 0; i < 6; ++i)
			this->sides[i] = cube.sides[i];
	}

	Cube(uint32_t sides[6]) {
		for (size_t i = 0; i < 6; ++i)
			this->sides[i] = sides[i];
	}

	Cube& operator=(const Cube& other) {
		for (size_t i = 0; i < 6; ++i)
			sides[i] = other.sides[i];
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, const Cube& cube);


private:
	uint32_t sides[6] = { 0, 0, 0, 0, 0, 0 };
};