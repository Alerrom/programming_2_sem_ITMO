#pragma once
#include <iostream>
#include <vector>
#include <list>

enum Rotation {
	U = 2, Up = -2, U2 = 4,
	D = 3, Dp = -3, D2 = 9,
	F = 5, Fp = -5, F2 = 25,
	B = 7, Bp = -7, B2 = 49,
	L = 11, Lp = -11, L2 = 121,
	R = 13, Rp = -13, R2 = 169
};

class Cube {
public:
	Cube();
	Cube(const Cube&);
	Cube(uint32_t sides[6]);

	Cube& operator=(const Cube&);

	friend std::ostream& operator<<(std::ostream&, const Cube&);

	bool get_side(size_t i);//todo

private:
	uint32_t sides[6] = { 0, 0, 0, 0, 0, 0 };
};