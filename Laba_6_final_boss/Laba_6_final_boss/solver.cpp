#include "solver.hpp"
#include <iostream>
#include <queue>
#include <map>
#include <windows.h>

Solver::Solver(const Cube& cube) {
	this->cube = cube;
	turns_for_group.resize(4);

	turns_for_group[0] = {
		F, Fp, B, Bp,
		L, Lp, R, Rp,
		U, Up, D, Dp,
		L2, R2, F2, B2, U2, D2
	};

	turns_for_group[1] = {
		L, Lp, R, Rp,
		U, Up, D, Dp,
		U2, F2, L2, R2, B2, D2
	};

	turns_for_group[2] = {
		U, Up, D, Dp,
		U2, F2, L2, R2, B2, D2
	};

	turns_for_group[3] = { 
		U2, F2, L2, R2, B2, D2 
	};
}

bool Solver::next_valid_turn(Rotation prev, Rotation foll) {
	if (prev < foll)
		std::swap(prev, foll);
	return (std::abs((int)prev) % std::abs((int)foll)) != 0;
}

std::list<Rotation> Solver::improove_group(size_t cur_group) {
	std::list<Rotation> res;
	if (cube.on_group(cur_group + 1))
		return res;
	for (size_t i = 0; i < 18; ++i) {
		auto[found, flag] = dls(cube, cur_group, std::list<Rotation>(), i);
		if (!found.empty()) {
			res = found;
			break;
		}
		else if (!flag)
			return std::list<Rotation>();
	}
	return res;
}

std::pair<std::list<Rotation>, bool> Solver::dls(Cube cube, size_t cur_group, std::list<Rotation> cur_rotation, size_t depth) {
	if (depth == 0) {
		cube.combo_move(cur_rotation);
		if (cube.on_group(cur_group + 1))
			return { cur_rotation, true };
		else
			return { std::list<Rotation>(), true };
	}
	bool flag = false;
	for (auto x : turns_for_group[cur_group]) {
		std::list<Rotation> new_rotation{ cur_rotation };
		if (!cur_rotation.empty()) {
			if (next_valid_turn(cur_rotation.back(), x))
				new_rotation.push_back(x);
			else
				continue;
		}
		else
			new_rotation.push_back(x);
		auto [found, remaining] = dls(cube, cur_group, new_rotation, depth - 1);
		if (!found.empty())
			return { found, true };
		if (remaining)
			flag = true;
	}
	return { std::list<Rotation>(), flag };
}

std::list<Rotation> Solver::solution() {
	std::list<Rotation> ans;
	size_t cur_group = 0;
	while (cur_group < 4) {
		if (cube.on_group(cur_group + 1)) {
			++cur_group;
			continue;
		}
		auto seq = improove_group(cur_group);
		Cube tmp(cube);
		tmp.combo_move(seq);
		if (tmp.on_group(cur_group + 1)) {
			++cur_group;
			cube.combo_move(seq);
			for (auto x : seq)
				ans.push_back(x);
			std::cout << "Solved:\n" << cur_group * 25 << '\n';
		}
	}
	return ans;
}