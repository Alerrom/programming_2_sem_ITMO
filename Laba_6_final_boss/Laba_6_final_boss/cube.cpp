#include "cube.hpp"
#include <iostream>


Cube::Cube() {
	sides[0] = 0x00000000;
	sides[1] = 0x11111111;
	sides[2] = 0x22222222;
	sides[3] = 0x33333333;
	sides[4] = 0x44444444;
	sides[5] = 0x55555555;
}

Cube::Cube(const Cube& cube) {
	for (size_t i = 0; i < 6; ++i)
		this->sides[i] = cube.sides[i];
}

Cube::Cube(uint32_t sides[6]) {
	for (size_t i = 0; i < 6; ++i)
		this->sides[i] = sides[i];
}

Cube& Cube::operator=(const Cube& other) {
	for (size_t i = 0; i < 6; ++i)
		sides[i] = other.sides[i];
	return *this;
}

uint32_t Cube::get_side(size_t index) {
	return sides[index];
}

uint32_t Cube::get_color(size_t side, size_t row, size_t col) const {
	if (row == 0)
		return (sides[side] & (0x0000000F << (col * 4))) >> (col * 4);

	if (row == 1) {
		if (col == 0)
			return (sides[side] & 0xF0000000) >> 28;
		if (col == 1)
			return side;
		if (col == 2)
			return (sides[side] & 0x0000F000) >> 12;
		return 0xFFFFFFFF;
	}

	if (row == 2)
		return (sides[side] & (0x0F000000 >> (col * 4))) >> (24 - col * 4);

	return 0xFFFFFFFF;
}

void Cube::rotate(size_t side) {
	uint32_t tmp = (sides[side] & 0xFF000000) >> 24;
	sides[side] <<= 8;
	sides[side] |= tmp;
}

uint32_t Cube::get_val(size_t side, size_t index) {
	return (sides[side] & (0x0000000F << (index * 4))) >> (index * 4);
}

void Cube::set_val(size_t side, size_t index, uint32_t new_val) {
	sides[side] &= ~(0x0000000F << (index * 4));
	sides[side] |= new_val << (index * 4);
}

char print_color(uint32_t i) {
	if (i == 0)
		return 'W';
	else if (i == 1)
		return 'O';
	else if (i == 2)
		return 'G';
	else if (i == 3)
		return 'R';
	else if (i == 4)
		return 'B';
	else if (i == 5)
		return 'Y';
}

std::ostream& operator<<(std::ostream& out, const Cube& cube) {
	for (size_t i = 0; i < 3; ++i) {
		out << "    ";
		for (size_t j = 0; j < 3; ++j) {
			out << print_color(cube.get_color(0, i, j));
		}
		out << '\n';
	}
	for (size_t i = 0; i < 3; ++i) {
		for (size_t side = 1; side <= 4; ++side) {
			for (size_t j = 0; j < 3; ++j) 
				out << print_color(cube.get_color(side, i, j));
			out << ' ';
		}
		out << '\n';
	}
	for (size_t i = 0; i < 3; ++i) {
		out << "    ";
		for (size_t j = 0; j < 3; ++j) {
			out << print_color(cube.get_color(5, i, j));
		}
		out << '\n';
	}
	return out;
}

void Cube::up(bool reverse) {
	for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
		rotate(0);
		uint32_t mask = 0x00000FFF;
		uint32_t temp = sides[1] & mask; // LEFT side
		for (size_t j = 1; j < 4; ++j) {
			sides[j] &= ~mask;
			sides[j] |= sides[j + 1] & mask;
		}
		sides[4] &= ~mask;
		sides[4] |= temp;
	}
}
void Cube::down(bool reverse) {
	for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
		rotate(5);
		uint32_t mask = 0x0FFF0000;
		uint32_t temp = sides[4] & mask; // BACK side
		for (size_t j = 4; j > 1; --j) {
			sides[j] &= ~mask;
			sides[j] |= sides[j - 1] & mask;
		}
		sides[1] &= ~mask;
		sides[1] |= temp;
	}
}
void Cube::left(bool reverse) {
	for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
		rotate(1);
		uint32_t t[3];
		for (size_t i = 0; i < 3; ++i)
			t[i] = get_val(4, i + 2);
		for (size_t i = 6; i <= 8; ++i) {
			set_val(4, i - 4, get_val(5, i % 8));
			set_val(5, i % 8, get_val(2, i % 8));
			set_val(2, i % 8, get_val(0, i % 8));
			set_val(0, i % 8, t[i - 6]);
		}
	}
}
void Cube::right(bool reverse) {
	for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
		rotate(3);
		uint32_t t[3];
		for (size_t i = 0; i < 3; ++i)
			t[i] = get_val(4, (i + 6) % 8);
		for (size_t i = 2; i <= 4; ++i) {
			set_val(4, (i + 4) % 8, get_val(0, i));
			set_val(0, i, get_val(2, i));
			set_val(2, i, get_val(5, i));
			set_val(5, i, t[i - 2]);
		}
	}
}
void Cube::front(bool reverse) {
	for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
		rotate(2);
		uint32_t t[3];
		for (size_t i = 0; i < 3; ++i)
			t[i] = get_val(0, i + 4);
		for (size_t i = 0; i < 3; ++i) {
			set_val(0, i + 4, get_val(1, i + 2));
			set_val(1, i + 2, get_val(5, i));
			set_val(5, i, get_val(3, (i + 6) % 8));
			set_val(3, (i + 6) % 8, t[i]);
		}
	}
}
void Cube::back(bool reverse) {
	for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
		rotate(4);
		uint32_t t[3];
		for (size_t i = 0; i < 3; ++i)
			t[i] = get_val(0, i);
		for (size_t i = 0; i < 3; ++i) {
			set_val(0, i, get_val(3, i + 2));
			set_val(3, i + 2, get_val(5, i + 4));
			set_val(5, i + 4, get_val(1, (i + 6) % 8));
			set_val(1, (i + 6) % 8, t[i]);
		}
	}
}

void Cube::combo_move(std::list<Rotation>& data) {
	for (auto com : data) {
		switch (com)
		{
		case Rotation::U:
			up();
			break;
		case Rotation::Up:
			up(true);
			break;
		case Rotation::U2:
			for (size_t i = 0; i < 2; ++i)
				up();
			break;

		case Rotation::D:
			down();
			break;
		case Rotation::Dp:
			down(true);
			break;
		case Rotation::D2:
			for (size_t i = 0; i < 2; ++i)
				down();
			break;

		case Rotation::F:
			front();
			break;
		case Rotation::Fp:
			front(true);
			break;
		case Rotation::F2:
			for (size_t i = 0; i < 2; ++i)
				front();
			break;

		case Rotation::B:
			back();
			break;
		case Rotation::Bp:
			back(true);
			break;
		case Rotation::B2:
			for (size_t i = 0; i < 2; ++i)
				back();
			break;

		case Rotation::L:
			left();
			break;
		case Rotation::Lp:
			left(true);
			break;
		case Rotation::L2:
			for (size_t i = 0; i < 2; ++i)
				left();
			break;

		case Rotation::R:
			right();
			break;
		case Rotation::Rp:
			right(true);
			break;
		case Rotation::R2:
			for (size_t i = 0; i < 2; ++i)
				right();
			break;
		}
	}
}

bool Cube::on_group_1() {
	uint32_t val, round;
	for (size_t i = 1; i <= 4; ++i) {

		// checking U-layer
		val = get_val(0, 2 * i - 1);
		if (val == 1 || val == 3)
			return false;
		if (val == 2 || val == 4) {
			round = get_val(5 - i, 1);
			if (round == 0 || round == 5)
				return false;
		}

		// checking D-layer
		val = get_val(5, 2 * i - 1);
		if (val == 1 || val == 3)
			return false;
		if (val == 2 || val == 4) {
			round = get_val((i + 1) % 5, 5);
			if (round == 0 || round == 5)
				return false;
		}
	}

	// checking slice between UP and DOWN 
	val = get_val(2, 3);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(3, 7);
		if (round == 0 || round == 5)
			return false;
	}
	val = get_val(2, 7);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(1, 3);
		if (round == 0 || round == 5)
			return false;
	}
	val = get_val(4, 3);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(1, 7);
		if (round == 0 || round == 5)
			return false;
	}
	val = get_val(4, 7);
	if (val == 1 || val == 3)
		return false;
	if (val == 2 || val == 4) {
		round = get_val(3, 3);
		if (round == 0 || round == 5)
			return false;
	}
	return true;
}

bool Cube::on_group_2() {
	for (size_t i = 0; i < 4; ++i)
		for (size_t j = 0; j <= 5; j += 5)
			if (get_val(j, i * 2) != 0 && get_val(j, i * 2) != 5)
				return false;

	for (size_t i = 1; i <= 4; ++i) 
		for (size_t j = 3; j <= 7; j += 4) 
			if (get_val(i, j) == 0 || get_val(i, j) == 5)
				return false;

	return true;
}

size_t Cube::get_corner_ind(size_t i) {
	size_t cur_corner[3];
	if (i == 0) {
		cur_corner[0] = get_val(0, 0); // UBL
		cur_corner[1] = get_val(4, 2);
		cur_corner[2] = get_val(1, 0);
	}
	else if (i == 1) {
		cur_corner[0] = get_val(0, 2); // UBR
		cur_corner[1] = get_val(4, 0);
		cur_corner[2] = get_val(3, 2);
	}
	else if (i == 2) {
		cur_corner[0] = get_val(0, 4); // UFR
		cur_corner[1] = get_val(2, 2);
		cur_corner[2] = get_val(3, 0);
	}
	else if (i == 3) {
		cur_corner[0] = get_val(0, 6); // UFL
		cur_corner[1] = get_val(2, 0);
		cur_corner[2] = get_val(1, 2);
	}
	else if (i == 4) {
		cur_corner[0] = get_val(5, 0); // F2L
		cur_corner[1] = get_val(2, 6);
		cur_corner[2] = get_val(1, 4);
	}
	else if (i == 5) {
		cur_corner[0] = get_val(5, 6); // B2L
		cur_corner[1] = get_val(4, 4);
		cur_corner[2] = get_val(1, 6);
	}
	else if (i == 6) {
		cur_corner[0] = get_val(5, 4); // B2R
		cur_corner[1] = get_val(4, 6);
		cur_corner[2] = get_val(3, 4);
	}
	else {
		cur_corner[0] = get_val(5, 2); // F2R
		cur_corner[1] = get_val(2, 4);
		cur_corner[2] = get_val(3, 6);
	}

	if (cur_corner[0] > cur_corner[1]) 
		std::swap(cur_corner[0], cur_corner[1]);
	if (cur_corner[1] > cur_corner[2]) 
		std::swap(cur_corner[1], cur_corner[2]);
	if (cur_corner[0] > cur_corner[1]) 
		std::swap(cur_corner[0], cur_corner[1]);

	size_t res = -1;

	for (size_t i = 0; i < 8; ++i) {
		bool flag = true;
		for (size_t j = 0; j < 3; ++j) {
			if (cur_corner[j] != corners[i][j]) {
				flag = false;
				break;
			}
			if (flag) {
				res = i;
				break;
			}
		}
	}
	return res;
}

bool Cube::check_parity() {
	uint8_t parity = 0;

	for (size_t i = 0; i < 8; i++)
		for (size_t j = i + 1; j < 8; j++)
			parity ^= (get_corner_ind(i) < get_corner_ind(j));
	return parity == 0;
}

bool Cube::on_group_3() {
	size_t ind;

	ind = get_corner_ind(0);
	if (ind != 0 && ind != 2)
		return false;
	ind = get_corner_ind(2);
	if (ind != 0 && ind != 2)
		return false;

	ind = get_corner_ind(1);
	if (ind != 1 && ind != 3)
		return false;
	ind = get_corner_ind(3);
	if (ind != 1 && ind != 3)
		return false;

	ind = get_corner_ind(4);
	if (ind != 4 && ind != 6)
		return false;
	ind = get_corner_ind(6);
	if (ind != 4 && ind != 6)
		return false;

	for (size_t i = 0; i < 6; ++i) {
		if (i == 1 || i == 3)
			continue;
		for (size_t j = 1; j <= 5; j += 4)
			if (get_val(i, j) == 1 || get_val(i, j) == 3)
				return false;
	}

	return check_parity();
}

bool Cube::on_group(size_t g) {
	if (g == 1)
		return on_group_1();
	if (g == 2)
		return on_group_2();
	if (g == 3)
		return on_group_3();
	if (g == 4)
		return solved();
	return true;
}

bool Cube::solved() {
	return  sides[0] == 0x00000000 &&
		sides[1] == 0x11111111 &&
		sides[2] == 0x22222222 &&
		sides[3] == 0x33333333 &&
		sides[4] == 0x44444444 &&
		sides[5] == 0x55555555;
}