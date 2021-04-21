#include "pugixml.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <map>
#include <set>


double dist(double x_1, double x_2, double y_1, double y_2) {
	return sqrt(pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2));
}


class Station {
public:
	Station() 
	{}

	//Set_methods
	void set_id(unsigned id) {
		id_ = id;
	}
	void set_type(const std::string& type) {
		type_ = type;
	}
	void set_streets(const std::string& street) {
		street_ = split(street, ", ");
	}
	void set_routes(const std::string& routes) {
		routes_ = split(routes, ".");
		if (routes_.size() == 1)
			routes_ = split(routes, ",");
	}
	void set_coords(const std::string& coords) {
		std::vector<std::string> tmp = split(coords, ",");
		std::string::size_type sz;

		coords_ = { std::stod(tmp[0], &sz),std::stod(tmp[1], &sz) };

	}

	//Get_methods
	unsigned get_id() const {
		return id_;
	}
	std::string get_type() const {
		return type_;
	}
	const std::vector<std::string>& get_streets() const {
		return street_;
	}
	const std::vector<std::string>& get_routes() const {
		return routes_;
	}
	std::pair<double, double> get_coords() const {
		return coords_;
	}

	static double get_route_length(const std::vector<std::pair<double, double>>& arr) {
		double ans = 0.0;
		for (unsigned i = 1; i < arr.size(); ++i) {
			ans += dist(arr[i].first, arr[i - 1].first, arr[i].second, arr[i - 1].second);
		}
		return ans;
	}


private:
	unsigned id_;
	std::string type_;
	std::vector<std::string> street_;
	std::vector<std::string> routes_;
	std::pair<double, double> coords_;

	std::vector<std::string> split(const std::string& str, const std::string& s) const {
		std::vector<std::string> arr;
		std::string delim(s);

		size_t prev = 0;
		size_t next;
		size_t delta = delim.length();

		while ((next = str.find(delim, prev)) != std::string::npos) {
			arr.push_back(str.substr(prev, next - prev));
			prev = next + delta;
		}

		arr.push_back(str.substr(prev));

		return arr;
	}
};


int main() {
	system("chcp 65001");

	pugi::xml_document doc;
	if (!doc.load_file("data_old.xml"))
		return -1;

	std::vector<Station> All_st;


	//Reading
	for (auto& station : doc.child("dataset").children("transport_station")) {
		Station current;

		current.set_id(atoi(station.child_value("number")));
		current.set_type(station.child_value("type_of_vehicle"));
		current.set_streets(station.child_value("location"));
		current.set_routes(station.child_value("routes"));
		current.set_coords(station.child_value("coordinates"));

		All_st.push_back(current);
	}


	//Preparing data
	std::set<std::string> types;
	std::map<std::string, unsigned> stops_at_street;

	for (const Station& s: All_st) {
		types.insert(s.get_type());
		for (const std::string& street: s.get_streets()) {
			if (stops_at_street.find(street) != stops_at_street.end())
				stops_at_street[street] += 1;
			else
				stops_at_street[street] = 1;
		}
	}


	//Solving 1Q
	std::map<std::string, std::pair<std::string, unsigned>> max_stops_at_route_for_type;

	for (const auto& type : types) {
		max_stops_at_route_for_type[type].first = "";
		max_stops_at_route_for_type[type].second = 0;
		std::map<std::string, unsigned> stops_at_route;
		
		for (const auto& s : All_st) {
			if (s.get_type() != type)
				continue;

			for (const auto& route : s.get_routes()) {
				if (stops_at_route.find(route) != stops_at_route.end())
					stops_at_route[route] += 1;
				else
					stops_at_route[route] = 1;
			}
		}
		 
		for (auto [route, num_of_stops] : stops_at_route) {
			if (num_of_stops > max_stops_at_route_for_type[type].second) {
				max_stops_at_route_for_type[type].second = num_of_stops;
				max_stops_at_route_for_type[type].first = route;
			}
		}
	}


	//Solving 2Q
	std::map<std::string, std::pair<std::string, double>> max_route_for_type;

	for (const auto& type : types) {
		max_route_for_type[type].first = "";
		max_route_for_type[type].second = 0.0;
		std::map<std::string, std::vector<std::pair<double, double>>> stops_cors_at_route;

		for (const auto& s : All_st) {
			if (s.get_type() != type)
				continue;

			for (const auto& route : s.get_routes()) {
				if (stops_cors_at_route.find(route) != stops_cors_at_route.end())
					stops_cors_at_route[route].push_back(s.get_coords());
				else 
					stops_cors_at_route[route] = std::vector<std::pair<double, double>>({ s.get_coords() });
			}
		}

		for (auto [route, stops_coors] : stops_cors_at_route) {
			double length = Station::get_route_length(stops_coors);

			if (length > max_route_for_type[type].second) {
				max_route_for_type[type].second = length;
				max_route_for_type[type].first = route;
			}
		}
	}

	//Solving 3Q
	std::string street_with_max_routes = "";
	unsigned max_stops_num = 0;

	for (auto [street, stops_num] : stops_at_street) {
		if (street != "") {
			if (stops_num > max_stops_num) {
				max_stops_num = stops_num;
				street_with_max_routes = street;
			}
		}
	}


	//Output
	/*
	std::ofstream fout("ans.txt");

	fout << "1. Маршрут с наибольшим числом остановок\n";
	for (const auto& type : types) {
		fout << type << " : " << max_stops_at_route_for_type[type].first << '\n';
	}
	fout << std::endl;

	fout << "2. Cамые длинные маршруты\n";
	for (const auto& type : types) {
		fout << type << " : " << max_route_for_type[type].first << '\n';
	}
	fout << std::endl;

	fout << "3. Улица с наибольшим числом остановок\n";
	fout << street_with_max_routes << " -> " << max_stops_num << " остановок\n";
	fout.close();
	*/

	std::cout << "1. Max stops at rout\n";
	for (const auto& type : types) {
		std::cout << type << " : " << max_stops_at_route_for_type[type].first << '\n';
	}
	std::cout << std::endl;

	std::cout << "2. The longest routs\n";
	for (const auto& type : types) {
		std::cout << type << " : " << max_route_for_type[type].first << '\n';
	}
	std::cout << std::endl;

	std::cout << "3. The biggest number of stops at street\n";
	std::cout << street_with_max_routes << " with " << max_stops_num << " stops\n";

	return 0;
}