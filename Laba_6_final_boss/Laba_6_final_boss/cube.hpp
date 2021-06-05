#pragma once
#include <iostream>
#include <vector>
#include <list>


/*
  0
1 2 3 4
  5

	WWW
	WWW
	WWW
OOO GGG RRR BBB
OOO GGG RRR BBB
OOO GGG RRR BBB
	YYY
	YYY
	YYY

    UUU
    UUU
    UUU
LLL FFF RRR BBB
LLL FFF RRR BBB
LLL FFF RRR BBB
    DDD
	DDD
	DDD

Each side is set of 4-bits slices
  0000 0000 0000 0000 0000 0000 0000 0000
  0    1    2    3    4    5    6    7

0 1 2
7 x 3 x - ind of side
6 5 4
*/


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

	void up(bool reverse = false);
	void down(bool reverse = false);
	void left(bool reverse = false);
	void right(bool reverse = false);
	void front(bool reverse = false);
	void back(bool reverse = false);

	bool on_group(size_t);
	bool on_group_1();
	bool on_group_2();
	bool on_group_3();
	bool solved();
	bool check_parity();

	void combo_move(std::list<Rotation>&);
	uint32_t get_val(size_t, size_t);
	uint32_t get_side(size_t);
	size_t get_corner_ind(size_t);

private:
	uint32_t sides[6] = { 0, 0, 0, 0, 0, 0 };

	uint32_t corners[8][3] = {
		{0, 1, 4},   //UBL
	    {0, 3, 4},   //UBR
	    {0, 2, 3},   //UFR
	    {0, 1, 2},   //UFL
	    {1, 2, 5},   //F2L
	    {1, 4, 5},   //B2L
	    {3, 4, 5},   //B2R
	    {2, 3, 5}    //F2R
	};

	uint32_t get_color(size_t, size_t, size_t) const;
	void rotate(size_t);
	void set_val(size_t, size_t, uint32_t);
};