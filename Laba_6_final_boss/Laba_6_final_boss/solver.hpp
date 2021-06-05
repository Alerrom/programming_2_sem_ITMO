#pragma once
#include "cube.hpp"
#include <vector>
#include <map>
#include <list>

class Solver {
public:
	Solver() = default;
	Solver(const Cube&);
	std::list<Rotation> solution();

private:
	Cube cube;

	std::vector<std::vector<Rotation>> turns_for_group;
	std::list<Rotation> improove_group(size_t);
	std::pair<std::list<Rotation>, bool> dls(Cube, size_t, std::list<Rotation>, size_t);

	bool next_valid_turn(Rotation, Rotation);
};