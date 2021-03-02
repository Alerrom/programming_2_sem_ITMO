#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <math.h>

class CPoint {
public:
	explicit CPoint(float x = 0.0f, float y = 0.0f)
		: x_(x)
		, y_(y)
	{}

	CPoint(const CPoint& other) = default;

	CPoint& operator=(const CPoint& other) = default;

	~CPoint() = default;

	//methods
	float Absciss() const {
		return x_;
	}

	float Ordinat() const {
		return y_;
	}

	void Print() {
		std::cout << "(" << Absciss() << ", " << Ordinat() << ")" << '\n';
	}

private:
	float x_;
	float y_;
};

class CPolygonalLine {
public:
	explicit CPolygonalLine(std::vector<std::vector<float>> PolygonalLine = {{0.0f, 0.0f}, {1.0f, 0.0f}})
		: PolygonalLine_(PolygonalLine)
	{
		if (!check_point_repeat(PolygonalLine_)) {
			std::cout << "Error: Incorrect input" << '\n';
			std::exit(false);
		}
	}

	CPolygonalLine(const CPolygonalLine& other) = default;

	CPolygonalLine& operator=(const CPolygonalLine& other) = default;

	~CPolygonalLine() = default;

	//methods
	void AddPoint(const std::vector<float>& p) {
		if (p[0] == PolygonalLine_[PolygonalLine_.size() - 1][0]
			&& p[1] == PolygonalLine_[PolygonalLine_.size() - 1][1])
			std::cout << "Sorry, you put the same point in the end. Nothing happend)" << '\n';
		PolygonalLine_.push_back(p);
	}

	float Lenght() {
		float lenght = 0;
		for (unsigned i = 1; i < PolygonalLine_.size(); i++) {
			lenght += Distant(PolygonalLine_[i], PolygonalLine_[i - 1]);
		}
		return lenght;
	}

	void PrintPolygonalLine() {
		for (unsigned i = 0; i < PolygonalLine_.size(); i++) {
			std::cout << "x_" << i << ' ' << PolygonalLine_[i][0] << ' ' << PolygonalLine_[i][1] << '\n';
		}
	}

private:
	std::vector<std::vector<float>> PolygonalLine_;

	float Distant(const std::vector<float>& p_1, const std::vector<float>& p_2) {
		return sqrt(pow(p_1[0] - p_2[0], 2)
			+ pow(p_1[1] - p_2[1], 2));
	}

	bool check_point_repeat(const std::vector<std::vector<float>>& ClosedLine) {
		bool flag = true;
		for (unsigned i = 0; i < ClosedLine.size() - 1; i++) {
			for (unsigned j = i + 1; j < ClosedLine.size(); j++) {
				if (ClosedLine[i][0] == ClosedLine[j][0] &&
					ClosedLine[i][1] == ClosedLine[j][1]) {
					flag = false;
					break;
				}
			}
		}
		return flag;
	}
};

class CClosedPolygonalLine {
public:
	explicit CClosedPolygonalLine(std::vector<std::vector<float>> ClosedPolygonalLine = {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}})
		: ClosedPolygonalLine_(ClosedPolygonalLine)
	{
		if (!check_point_repeat(ClosedPolygonalLine)) {
			std::cout << "Error: Incorrect input" << '\n';
			std::exit(false);
		}
	}

	CClosedPolygonalLine(const CClosedPolygonalLine& other) = default;

	CClosedPolygonalLine& operator=(const CClosedPolygonalLine& other) = default;

	~CClosedPolygonalLine() = default;


	//methods
	float Lenght() {
		float lenght = 0;
		for (unsigned i = 1; i < ClosedPolygonalLine_.size(); i++)
			lenght += Distant(ClosedPolygonalLine_[i], ClosedPolygonalLine_[i - 1]);
		lenght += Distant(ClosedPolygonalLine_[0], ClosedPolygonalLine_[ClosedPolygonalLine_.size() - 1]);
		return lenght;
	}

	void Print() {
		for (unsigned i = 0; i < ClosedPolygonalLine_.size(); i++)
			std::cout << "x_" << i << ' ' << ClosedPolygonalLine_[i][0] << ' ' << ClosedPolygonalLine_[i][1] << '\n';
	}

private:
	std::vector<std::vector<float>> ClosedPolygonalLine_;

	float Distant(const std::vector<float>& p_1, const std::vector<float>& p_2) {
		return sqrt(pow(p_1[0] - p_2[0], 2)
			+ pow(p_1[1] - p_2[1], 2));
	}

	bool check_point_repeat(const std::vector<std::vector<float>>& ClosedLine) {
		bool flag = true;
		for (unsigned i = 0; i < ClosedLine.size() - 1; i++) {
			for (unsigned j = i + 1; j < ClosedLine.size(); j++) {
				if (ClosedLine[i][0] == ClosedLine[j][0] &&
					ClosedLine[i][1] == ClosedLine[j][1]) {
					flag = false;
					break;
				}
			}
		}
		return flag;
	}
};

class CPolygon {
public:
	explicit CPolygon(const std::vector<std::vector<float>>& Polygon = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}})
		: Polygon_(Polygon)
	{
		if (!check_point_number(Polygon) || !check_point_repeat(Polygon) || !check_convex(Polygon)) {
			std::cout << "Error: Incorrect input" << '\n';
			std::exit(false);
		}
	}

	CPolygon(const CPolygon& other) = default;

	CPolygon& operator=(const CPolygon& other) = default;

	~CPolygon() = default;

	//methods
	float Square() {
		float ans = 0;
		for (unsigned i = 0; i < Polygon_.size() - 1; i++) {
			ans += Polygon_[i][0] * Polygon_[i + 1][1] -
				Polygon_[i][1] * Polygon_[i + 1][0];
		}
		ans += Polygon_[Polygon_.size() - 1][0] * Polygon_[0][1] -
			Polygon_[Polygon_.size() - 1][1] * Polygon_[0][0];
		return abs(ans) / 2;
	}

	float Perimeter() {
		float ans = 0;
		for (unsigned i = 1; i < Polygon_.size(); i++)
			ans += Dist(Polygon_[i], Polygon_[i - 1]);
		ans += Dist(Polygon_[0], Polygon_[Polygon_.size() - 1]);
		return ans;
	}

	void Print() {
		for (unsigned i = 0; i < Polygon_.size(); i++)
			std::cout << "x_" << i << ' ' << Polygon_[i][0] << ' ' << Polygon_[i][1] << '\n';
	}

private:
	std::vector<std::vector<float>> Polygon_;

	float Dist(const std::vector<float>& p_1, const std::vector<float>& p_2) {
		return sqrt(pow(p_1[0] - p_2[0], 2) 
			+ pow(p_1[1] - p_2[1], 2));
	}

	bool check_point_number(const std::vector<std::vector<float>>& Polygon) {
		bool flag = true;
		if (Polygon.size() < 3)
			flag = false;
		return flag;
	}

	bool check_point_repeat(const std::vector<std::vector<float>>& Polygon) {
		bool flag = true;
		for (unsigned i = 0; i < Polygon.size() - 1; i++) {
			for (unsigned j = i + 1; j < Polygon.size(); j++) {
				if (Polygon[i][0] == Polygon[j][0] &&
					Polygon[i][1] == Polygon[j][1]) {
					flag = false;
					break;
				}
			}
		}
		return flag;
	}

	bool check_convex(const std::vector<std::vector<float>>& Polygon) {
		std::vector<float> A_1, A_2;
		bool flag = true;
		unsigned left = 0;
		unsigned right = 0;

		for (unsigned i = 1; i < Polygon.size() - 1; i++) {
			A_1 = { Polygon[i][0] - Polygon[i - 1][0],
				Polygon[i][1] - Polygon[i - 1][1] };
			A_2 = { Polygon[i + 1][0] - Polygon[i][0],
				Polygon[i + 1][1] - Polygon[i][1] };
			if (A_1[0] * A_2[1] - A_1[1] * A_2[0] > 0)
				left++;
			else if (A_1[0] * A_2[1] - A_1[1] * A_2[0] == 0)
				return !flag;
			else
				right++;
		}
		unsigned i = Polygon.size() - 1;

		A_1 = { Polygon[i][0] - Polygon[i - 1][0],
				Polygon[i][1] - Polygon[i - 1][1] };
		A_2 = { Polygon[0][0] - Polygon[i][0],
			Polygon[0][1] - Polygon[i][1] };
		if (A_1[0] * A_2[1] - A_1[1] * A_2[0] > 0)
			left++;
		else if (A_1[0] * A_2[1] - A_1[1] * A_2[0] == 0)
			return !flag;
		else
			right++;

		A_1 = { Polygon[0][0] - Polygon[i][0],
				Polygon[0][1] - Polygon[i][1] };
		A_2 = { Polygon[1][0] - Polygon[0][0],
			Polygon[1][1] - Polygon[0][1] };
		if (A_1[0] * A_2[1] - A_1[1] * A_2[0] > 0)
			left++;
		else if (A_1[0] * A_2[1] - A_1[1] * A_2[0] == 0)
			return !flag;
		else
			right++;
		if (left != 0 && right !=  0)
			flag = false;

		return flag;
	}
};

class CTriangle: public CPolygon {
public:
	CTriangle(const std::vector<std::vector<float>>& Triangle = {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}})
		: CPolygon(Triangle)
	{
		if (!check_point_number(Triangle)) {
			std::cout << "Error: Incorrect input" << '\n';
			std::exit(false);
		}
	}

	CTriangle(const CTriangle& other) = default;

	CTriangle& operator=(const CTriangle& other) = default;

	~CTriangle() = default;

	//methods
	using CPolygon::Square;

	using CPolygon::Perimeter;

	using CPolygon::Print;

private:
	bool check_point_number(const std::vector<std::vector<float>>& Triangle) {
		bool flag = true;
		if (Triangle.size() != 3)
			flag = false;
		return flag;
	}
};

class CTrapezoid : protected CPolygon {
public:
	explicit CTrapezoid(const std::vector<std::vector<float>>& Trapezoid = {{0.0f, 0.0f},{3.0f, 0.0f},{2.0f, 1.0f},{1.0f, 1.0f}})
		: CPolygon(Trapezoid)
	{
		if (!check_point_number(Trapezoid) || !check_tr(Trapezoid)) {
			std::cout << "Error: Incorrect input" << '\n';
			std::exit(false);
		}
	}

	CTrapezoid(const CTrapezoid& other) = default;

	CTrapezoid& operator=(const CTrapezoid& other) = default;

	~CTrapezoid() = default;

	//methods
	using CPolygon::Square;

	using CPolygon::Perimeter;

	using CPolygon::Print;

private:
	bool check_point_number(const std::vector<std::vector<float>>& Trapezoid) {
		bool flag = true;
		if (Trapezoid.size() != 4)
			flag = false;
		return flag;
	}

	bool check_tr(const std::vector<std::vector<float>>& Trapezoid) {
		std::vector<float> AB, BC, CD, DA;
		bool flag = true;

		AB = { Trapezoid[1][0] - Trapezoid[0][0],
				Trapezoid[1][1] - Trapezoid[0][1] };
		BC = { Trapezoid[2][0] - Trapezoid[1][0],
			Trapezoid[2][1] - Trapezoid[1][1] };
		CD = { Trapezoid[3][0] - Trapezoid[2][0],
				Trapezoid[3][1] - Trapezoid[2][1] };
		DA = { Trapezoid[0][0] - Trapezoid[3][0],
			Trapezoid[0][1] - Trapezoid[3][1] };

		if (AB[0] * CD[1] - AB[1] * CD[0] != 0) {
			if (BC[0] * DA[1] - BC[1] * DA[0] != 0)
				flag = false;
		}
		else {
			if (BC[0] * DA[1] - BC[1] * DA[0] == 0)
				flag = false;
		}

		return flag;
	}
};

class CRegularPolygon {
public:
	CRegularPolygon(unsigned PointsNumber = 4, float Size = 1.0)
		: PointsNumber_(PointsNumber)
		, Size_(Size)
	{
		if (PointsNumber <= 2 || Size <= 0) {
			std::cout << "Error: Incorrect input" << '\n';
			std::exit(false);
		}
	}

	CRegularPolygon(const CRegularPolygon& other) = default;

	CRegularPolygon& operator=(const CRegularPolygon& other) = default;

	~CRegularPolygon() = default;

	//methods
	double Square() {
		return (PointsNumber_ * pow(Size_, 2.0))
			/ (4.0 * tan(M_PI / PointsNumber_));
	}

	float Perimeter() {
		return PointsNumber_ * Size_;
	}

private:
	unsigned PointsNumber_;
	float Size_;
};

int main() {
	//CPoint test
	/*std::cout << "CPoint test\n";

	CPoint A;
	CPoint B{0.5};
	CPoint C{-2.2f, 3};
	CPoint D = A;
	CPoint E;
	E = D;

	A.Print();
	B.Print();
	C.Print();
	D.Print();
	E.Print();*/

	//CPolygonalLine test
	/*std::cout << "CPolygonalLine test\n";

	CPolygonalLine AB;

	AB.PrintPolygonalLine();
	std::cout << AB.Lenght() << '\n';
	AB.AddPoint({3, 4});
	AB.PrintPolygonalLine();
	std::cout << AB.Lenght() << '\n';
	AB.AddPoint({1, 2});
	AB.PrintPolygonalLine();
	std::cout << AB.Lenght() << '\n';*/
	
	//CClosedPolygonalLine test
	/*std::cout << "CClosedPolygonalLine test\n";

	CClosedPolygonalLine line;
	line.Print();
	std::cout << line.Lenght() << '\n';

	CClosedPolygonalLine Aline({ {0,0},{1,1},{-1,1},{-1,-3},{0,-2} });
	Aline.Print();
	std::cout << Aline.Lenght() << '\n';*/
	
	//CPolygon test
	/*std::cout << "CPolygon test\n";

	CPolygon fig({{0, 0}, {1, 0}, {0, -1}, {-1, 1}});
	std::cout << fig.Square() << '\n';
	std::cout << fig.Perimeter() << '\n';
	fig.Print();*/
	
	//CTriangle test
	/*std::cout << "CTriangle test\n";

	CTriangle abc({ {0,0},{0,3},{4,0} });

	std::cout << abc.Perimeter() << '\n';
	std::cout << abc.Square() << '\n';
	abc.Print();*/
	
	//CTrapezoid test
	/*std::cout << "CTrapezoid test\n";

	CTrapezoid tr({ {0,0},{8,0},{5,4},{3,4} });
	std::cout << tr.Perimeter() << '\n';
	std::cout << tr.Square()<< '\n';
	tr.Print();*/
	
	//CRegularPolygon test
	/*std::cout << "CRegularPolygon test\n";

	CRegularPolygon g;
	std::cout << g.Perimeter() << '\n';
	std::cout << g.Square() << '\n';
	*/

	return 0;
}