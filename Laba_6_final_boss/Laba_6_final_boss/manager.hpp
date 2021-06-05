#pragma once
#include "cube.hpp"
#include "solver.hpp"
#include <map>

class Manager {
public:
	Manager();
	void start();

private:
	Cube cube;

	void load_cube(const std::string&);
	void save_cube(const std::string&);
	void give_solution();

	std::map<Rotation, std::string> turns_to_strings;
	std::map<std::string, Rotation> strings_to_turns;
	std::list<Rotation> parse_turns(const std::string&);
};