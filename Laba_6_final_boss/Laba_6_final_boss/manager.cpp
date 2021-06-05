#include "manager.hpp"
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>

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

Manager::Manager() {
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

void Manager::start() {
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
			give_solution();
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

void Manager::load_cube(const std::string& fname) {
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

void Manager::save_cube(const std::string& fname) {
	std::ofstream fout(fname);
	for (size_t i = 0; i < 6; ++i)
		fout << cube.get_side(i) << '\n';
	fout.close();
}

std::list<Rotation> Manager::parse_turns(const std::string& line) {
	std::stringstream ss;
	std::list<Rotation> rot;
	ss << line;
	std::string turn;
	while (ss >> turn)
		rot.push_back(strings_to_turns[turn]);
	return rot;
}

void Manager::give_solution() {
	Solver sol = Solver{ cube };
	std::list<Rotation> seq = sol.solution();
	for (auto x: seq)
		std::cout << turns_to_strings[x] << ' ';
	cube.combo_move(seq);
	std::cout << '\n' << cube << '\n';
}