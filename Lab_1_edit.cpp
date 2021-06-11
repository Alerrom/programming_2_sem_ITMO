#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <math.h>
#include <exception>

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

float Distant(const CPoint& p_1, const CPoint& p_2) {
	return sqrt(pow(p_1.Absciss() - p_2.Absciss(), 2)
		+ pow(p_1.Ordinat() - p_2.Ordinat(), 2));
}

bool check_point_repeat(const std::vector<CPoint>& line) {
	bool flag = true;
	for (unsigned i = 0; i < line.size() - 1; i++) {
		for (unsigned j = i + 1; j < line.size(); j++) {
			if (line[i].Absciss() == line[j].Absciss() &&
				line[i].Ordinat() == line[j].Ordinat()) {
				flag = false;
				break;
			}
		}
	}
	return flag;
}

class CPolygonalLine {
public:
	explicit CPolygonalLine(const std::vector<CPoint>& PolygonalLine = {CPoint{0.0f, 0.0f}, CPoint{1.0f, 0.0f}})
		: PolygonalLine_(PolygonalLine)
	{
		if (!check_point_repeat(PolygonalLine_))
			throw std::exception("Incorrect input - some points repeat");
		else if (!check_point_number(PolygonalLine))
			throw std::exception("Incorrect input - wrong number of points");
	}

	CPolygonalLine(const CPolygonalLine& other) = default;

	CPolygonalLine& operator=(const CPolygonalLine& other) = default;

	~CPolygonalLine() = default;

	//methods
	void AddPoint(const CPoint& p) {
		if (p.Absciss() == PolygonalLine_[PolygonalLine_.size() - 1].Absciss()
			&& p.Ordinat() == PolygonalLine_[PolygonalLine_.size() - 1].Ordinat()) {
			std::cout << "Sorry, you put the same point in the end. Nothing happend)" << '\n';
			return;
		}
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
			std::cout << "x_" << i << ' ';
			PolygonalLine_[i].Print();
		}
	}

private:
	std::vector<CPoint> PolygonalLine_;

	bool check_point_number(const std::vector<CPoint>& Polygon) {
		return !(Polygon.size() < 2);
	}
};

class CClosedPolygonalLine {
public:
	explicit CClosedPolygonalLine(const std::vector<CPoint>& ClosedPolygonalLine = { CPoint{0.0f, 0.0f}, CPoint{1.0f, 0.0f}, CPoint{0.0f, 1.0f}})
		: ClosedPolygonalLine_(ClosedPolygonalLine)
	{
		if (!check_point_repeat(ClosedPolygonalLine))
			throw std::exception("Incorrect input - some points repeat");
		else if (!check_point_number(ClosedPolygonalLine))
			throw std::exception("Incorrect input - wrong number of point");
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
		for (unsigned i = 0; i < ClosedPolygonalLine_.size(); i++) {
			std::cout << "x_" << i << ' ';
			ClosedPolygonalLine_[i].Print();
		}
	}

private:
	std::vector<CPoint> ClosedPolygonalLine_;

	bool check_point_number(const std::vector<CPoint>& Polygon) {
		return !(Polygon.size() < 3);
	}
};

class CPolygon {
public:
	explicit CPolygon(const std::vector<CPoint>& Polygon = { CPoint{0.0f, 0.0f}, CPoint{1.0f, 0.0f}, 
		CPoint{1.0f, 1.0f}, CPoint{0.0f, 1.0f}})
		: Polygon_(Polygon)
	{
		if (!check_point_number(Polygon))
			throw std::exception("Incorrect input - wrong number of points");
		else if (!check_point_repeat(Polygon))
			throw std::exception("Incorrect input - some points repeat");
		else if (!check_convex(Polygon))
			throw std::exception("Incorrect input - figure not convex");
	}

	CPolygon(const CPolygon& other) = default;

	CPolygon& operator=(const CPolygon& other) = default;

	~CPolygon() = default;

	//methods
	virtual float Square() {
		float ans = 0;
		for (unsigned i = 0; i < Polygon_.size() - 1; i++) {
			ans += Polygon_[i].Absciss() * Polygon_[i + 1].Ordinat() -
				Polygon_[i].Ordinat() * Polygon_[i + 1].Absciss();
		}
		ans += Polygon_[Polygon_.size() - 1].Absciss() * Polygon_[0].Ordinat() -
			Polygon_[Polygon_.size() - 1].Ordinat() * Polygon_[0].Absciss();
		return abs(ans) / 2;
	}

	virtual float Perimeter() {
		float ans = 0;
		for (unsigned i = 1; i < Polygon_.size(); i++)
			ans += Distant(Polygon_[i], Polygon_[i - 1]);
		ans += Distant(Polygon_[0], Polygon_[Polygon_.size() - 1]);
		return ans;
	}

	virtual void Print() {
		for (unsigned i = 0; i < Polygon_.size(); i++) {
			std::cout << "x_" << i << ' ';
			Polygon_[i].Print();
		}
	}

private:
	std::vector<CPoint> Polygon_;

	bool check_point_number(const std::vector<CPoint>& Polygon) {
		return !(Polygon.size() < 3);
	}

	bool check_convex(const std::vector<CPoint>& Polygon) {
		std::vector<float> A_1, A_2;
		bool flag = true;
		unsigned left = 0;
		unsigned right = 0;

		for (unsigned i = 1; i < Polygon.size() - 1; i++) {
			A_1 = { Polygon[i].Absciss() - Polygon[i - 1].Absciss(),
				Polygon[i].Ordinat() - Polygon[i - 1].Ordinat() };
			A_2 = { Polygon[i + 1].Absciss() - Polygon[i].Absciss(),
				Polygon[i + 1].Ordinat() - Polygon[i].Ordinat() };
			if (A_1[0] * A_2[1] - A_1[1] * A_2[0] > 0)
				left++;
			else if (A_1[0] * A_2[1] - A_1[1] * A_2[0] == 0)
				return !flag;
			else
				right++;
		}
		unsigned i = Polygon.size() - 1;

		A_1 = { Polygon[i].Absciss() - Polygon[i - 1].Absciss(),
				Polygon[i].Ordinat() - Polygon[i - 1].Ordinat() };
		A_2 = { Polygon[0].Absciss() - Polygon[i].Absciss(),
			Polygon[0].Ordinat() - Polygon[i].Ordinat() };
		if (A_1[0] * A_2[1] - A_1[1] * A_2[0] > 0)
			left++;
		else if (A_1[0] * A_2[1] - A_1[1] * A_2[0] == 0)
			return !flag;
		else
			right++;

		A_1 = { Polygon[0].Absciss() - Polygon[i].Absciss(),
				Polygon[0].Ordinat() - Polygon[i].Ordinat() };
		A_2 = { Polygon[1].Absciss() - Polygon[0].Absciss(),
			Polygon[1].Ordinat() - Polygon[0].Ordinat() };
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
	CTriangle(const std::vector<CPoint>& Triangle = { CPoint{0.0f, 0.0f}, CPoint{1.0f, 0.0f}, CPoint{0.0f, 1.0f} })
		: CPolygon(Triangle)
	{
		if (!check_point_number(Triangle))
			throw std::exception("Incorrect input - wrong number of points");
	}

	CTriangle(const CTriangle& other) = default;

	CTriangle& operator=(const CTriangle& other) = default;

	~CTriangle() = default;

	//methods

private:
	bool check_point_number(const std::vector<CPoint>& Triangle) {
		return Triangle.size() == 3;
	}
};

class CTrapezoid : public CPolygon {
public:
	explicit CTrapezoid(const std::vector<CPoint>& Trapezoid = { CPoint{0.0f, 0.0f}, CPoint{3.0f, 0.0f},
		CPoint{2.0f, 1.0f}, CPoint{1.0f, 1.0f}})
		: CPolygon(Trapezoid)
	{
		if (!check_point_number(Trapezoid)) 
			throw std::exception("Incorrect input - wrong number of points");
		else if (!check_tr(Trapezoid)) 
			throw std::exception("Incorrect input - not trapezoid");
	}

	CTrapezoid(const CTrapezoid& other) = default;

	CTrapezoid& operator=(const CTrapezoid& other) = default;

	~CTrapezoid() = default;

private:
	bool check_point_number(const std::vector<CPoint>& Trapezoid) {
		return Trapezoid.size() == 4;
	}

	bool check_tr(const std::vector<CPoint>& Trapezoid) {
		std::vector<float> AB, BC, CD, DA;

		AB = { Trapezoid[1].Absciss() - Trapezoid[0].Absciss(),
				Trapezoid[1].Ordinat() - Trapezoid[0].Ordinat() };
		BC = { Trapezoid[2].Absciss() - Trapezoid[1].Absciss(),
			Trapezoid[2].Ordinat() - Trapezoid[1].Ordinat() };
		CD = { Trapezoid[3].Absciss() - Trapezoid[2].Absciss(),
				Trapezoid[3].Ordinat() - Trapezoid[2].Ordinat() };
		DA = { Trapezoid[0].Absciss() - Trapezoid[3].Absciss(),
			Trapezoid[0].Ordinat() - Trapezoid[3].Ordinat() };

		if (AB[0] * CD[1] - AB[1] * CD[0] != 0) {
			if (BC[0] * DA[1] - BC[1] * DA[0] != 0)
				return false;
		}
		else {
			if (BC[0] * DA[1] - BC[1] * DA[0] == 0)
				return false;
		}

		return true;
	}
};

class CRegularPolygon : public CPolygon {
public:
	CRegularPolygon(const std::vector<CPoint>& RegularPolygon = { CPoint{0.0f, 0.0f}, CPoint{1.0f, 0.0f},
		CPoint{1.0f, 1.0f}, CPoint{0.0f, 1.0f} })
		: CPolygon(RegularPolygon)
	{
		if (!check_regular(RegularPolygon))
			throw std::exception("Incorrect input - not regular polygon");
	}

	CRegularPolygon(const CRegularPolygon& other) = default;

	CRegularPolygon& operator=(const CRegularPolygon& other) = default;

	~CRegularPolygon() = default;

	//methods

private:
	bool check_regular(const std::vector<CPoint>& RegularPolygon) {
		float x_mass = 0, y_mass = 0;

		for (unsigned i = 0; i < RegularPolygon.size(); i++) {
			x_mass += RegularPolygon[i].Absciss();
			y_mass += RegularPolygon[i].Ordinat();
		}

		x_mass /= RegularPolygon.size();
		y_mass /= RegularPolygon.size();

		CPoint C_masss{x_mass, y_mass};

		float flag_dist = Distant(RegularPolygon[0], C_masss);
		
		for (unsigned i = 1; i < RegularPolygon.size(); i++) {
			if (flag_dist != Distant(RegularPolygon[i], C_masss))
				return false;
		}

		return true;
	}
};

int main() {
	//CPoint test
	/*
	std::cout << "CPoint test\n";

	CPoint A;
	CPoint B{0, 0.5f};
	CPoint C{-2.2f, 3};
	CPoint D = A;
	CPoint E;
	E = B;

	A.Print();
	B.Print();
	C.Print();
	D.Print();
	E.Print();
	*/

	//CPolygonalLine test
	/*std::cout << "CPolygonalLine test\n";

	CPolygonalLine AB;

	AB.PrintPolygonalLine();
	std::cout << AB.Lenght() << '\n';

	AB.AddPoint(CPoint{3, 4});
	AB.PrintPolygonalLine();
	std::cout << AB.Lenght() << '\n';

	AB.AddPoint(CPoint{1, 2});
	AB.PrintPolygonalLine();
	std::cout << AB.Lenght() << '\n';

	AB.AddPoint(CPoint{1, 2});
	AB.PrintPolygonalLine();
	std::cout << AB.Lenght() << '\n';*/
	
	//CClosedPolygonalLine test
	/*std::cout << "CClosedPolygonalLine test\n";

	CClosedPolygonalLine line;
	line.Print();
	std::cout << line.Lenght() << '\n';

	CClosedPolygonalLine Aline({ CPoint{0,0},CPoint{1,1},CPoint{1,-1},CPoint{-1,-3},CPoint{0,-2} });
	Aline.Print();
	std::cout << Aline.Lenght() << '\n';*/
	
	//CPolygon test
	std::cout << "CPolygon test\n";

	CPolygon fig({ CPoint{-2, 0}, CPoint{2, 3}, CPoint{-2, 3}, CPoint{2, 0}, CPoint{0, 5} });
	std::cout << fig.Square() << '\n';
	std::cout << fig.Perimeter() << '\n';
	fig.Print();
	

	try	{
		CTriangle abc({ CPoint{0,0}, CPoint{0,3}, CPoint{0,0} });
	}
	catch (std::exception &exception) {
		std::cerr << exception.what() << '\n';
	}

	//CTriangle test
	std::cout << "CTriangle test\n";

	CTriangle abc({ CPoint{0,0}, CPoint{0,3}, CPoint{4,0} });

	std::cout << abc.Perimeter() << '\n';
	std::cout << abc.Square() << '\n';
	abc.Print();
	
	//CTrapezoid test
	/*std::cout << "CTrapezoid test\n";

	CTrapezoid tr({ CPoint{0,0},CPoint{8,0},CPoint{5,4},CPoint{3,4} });
	std::cout << tr.Perimeter() << '\n';
	std::cout << tr.Square()<< '\n';
	tr.Print();*/
	
	//CRegularPolygon test
	/*std::cout << "CRegularPolygon test\n";

	CRegularPolygon g({ CPoint{0,0}, CPoint{2,2}, CPoint{0,4}, CPoint{-2,2} });

	std::cout << g.Perimeter() << '\n';
	std::cout << g.Square() << '\n';
	g.Print();*/

	return 0;
}