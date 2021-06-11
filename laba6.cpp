#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>

enum Rotation {
	U = 2, Up = -2, U2 = 4,
	D = 3, Dp = -3, D2 = 9,
	F = 5, Fp = -5, F2 = 25,
	B = 7, Bp = -7, B2 = 49,
	L = 11, Lp = -11, L2 = 121,
	R = 13, Rp = -13, R2 = 169
};

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
	else
		return ' ';
}

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

	friend std::ostream& operator<<(std::ostream&, const Cube&);

	uint32_t get_val(size_t side, size_t index) {
		return (sides[side] & (0x0000000F << (index * 4))) >> (index * 4);
	}

	uint32_t get_side(size_t index) {
		return sides[index];
	}

	void up(bool reverse = false) {
		for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
			rotate(0);
			uint32_t mask = 0x00000FFF;
			uint32_t temp = sides[1] & mask;
			for (size_t j = 1; j < 4; ++j) {
				sides[j] &= ~mask;
				sides[j] |= sides[j + 1] & mask;
			}
			sides[4] &= ~mask;
			sides[4] |= temp;
		}
	}
	void down(bool reverse = false) {
		for (size_t count = 0; count < (reverse ? 3 : 1); ++count) {
			rotate(5);
			uint32_t mask = 0x0FFF0000;
			uint32_t temp = sides[4] & mask;
			for (size_t j = 4; j > 1; --j) {
				sides[j] &= ~mask;
				sides[j] |= sides[j - 1] & mask;
			}
			sides[1] &= ~mask;
			sides[1] |= temp;
		}
	}
	void left(bool reverse = false) {
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
	void right(bool reverse = false) {
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
	void front(bool reverse = false) {
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
	void back(bool reverse = false) {
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

	bool solved() {
		return  sides[0] == 0x00000000 &&
			sides[1] == 0x11111111 &&
			sides[2] == 0x22222222 &&
			sides[3] == 0x33333333 &&
			sides[4] == 0x44444444 &&
			sides[5] == 0x55555555;
	}

	void combo_move(std::list<Rotation>& data) {
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

private:
	uint32_t sides[6] = { 0, 0, 0, 0, 0, 0 };

	uint32_t get_color(size_t side, size_t row, size_t col) const {
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

	void rotate(size_t side) {
		uint32_t temp = (sides[side] & 0xFF000000) >> 24;
		sides[side] <<= 8;
		sides[side] |= temp;
	}
	void set_val(size_t side, size_t index, uint32_t new_val) {
		sides[side] &= ~(0x0000000F << (index * 4));
		sides[side] |= new_val << (index * 4);
	}
};

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


void commands() {
	std::cout << "Here some commands you can use during session:\n";
	std::cout << "load + f. name - load your cube\n";
	std::cout << "save + f. name - save your cube\n";
	std::cout << "print - show your cube\n";
	std::cout << "new - create new cube\n";
	std::cout << "rotate + ... - use basics commands to your cube\n";
	std::cout << "solve - find solution of your cube\n";
	std::cout << "exit - stop programm\n";
}

class Manager {
public:
	Manager() {
		turns_to_strings = {
		{U, "U"}, {Up, "Up\'"}, {U2, "U2"},
		{L, "B"}, {Lp, "Lp\'"}, {L2, "L2"},
		{F, "F"}, {Fp, "Fp\'"}, {F2, "F2"},
		{R, "R"}, {Rp, "Rp\'"}, {R2, "R2"},
		{B, "B"}, {Bp, "Bp\'"}, {B2, "B2"},
		{D, "D"}, {Dp, "Dp\'"},  {D2, "D2"}
		};

		strings_to_turns = {
			{"U", U}, {"U\'", Up}, {"U2", U2},
			{"L", B}, {"L\'", Lp}, {"L2", L2},
			{"F", F}, {"F\'", Fp}, {"F2", F2},
			{"R", R}, {"R\'", Rp}, {"R2", R2},
			{"B", B}, {"B\'", Bp}, {"B2", B2},
			{"D", D}, {"D\'", Dp},  {"D2", D2}
		};
	}

	void start() {
		std::cout << "Hello man!\n";
		commands();
		std::string comm1, comm2;

		while (true) {
			std::cout << ">>> ";
			std::cin >> comm1;

			if (comm1 == "load") {
				std::cin >> comm2;
				load_cube(comm2);
			}
			else if (comm1 == "save") {
				std::cin >> comm2;
				save_cube(comm2);
			}
			else if (comm1 == "print") {
				std::cout << cube << '\n';
			}
			else if (comm1 == "new") {
				cube = Cube();
				std::cout << cube << '\n';
			}
			else if (comm1 == "rotate") {
				std::getline(std::cin, comm2);
				auto rot = parse_turns(comm2);
				cube.combo_move(rot);
				std::cout << cube << '\n';
			}
			else if (comm1 == "solve") {
				//give_solution
			}
			else if (comm1 == "exit") {
				std::cout << "See you later!\n";
				break;
			}
			else {
				std::cout << "Wrong command\n";
			}
		}
	}

private:
	Cube cube;

	std::map<Rotation, std::string> turns_to_strings;
	std::map<std::string, Rotation> strings_to_turns;

	void load_cube(const std::string& fname) {
		std::ifstream fin(fname);
		if (!fin.good()) {
			fin.close();
			std::cout << "file \"" << fname << "\" not found\n";
			return;
		}
		uint32_t sides[6];
		for (size_t i = 0; i < 6; ++i)
			fin >> sides[i];
		cube = Cube(sides);
		fin.close();
	}

	void save_cube(const std::string& fname) {
		std::ofstream fout(fname);
		for (size_t i = 0; i < 6; ++i)
			fout << cube.get_side(i) << '\n';
		fout.close();
	}

	std::list<Rotation> parse_turns(const std::string& line) {
		std::stringstream ss;
		std::list<Rotation> rot;
		ss << line;
		std::string turn;
		while (ss >> turn)
			rot.push_back(strings_to_turns[turn]);
		return rot;
	}
};

int main() {
	Manager m;
	m.start();

	return 0;
}