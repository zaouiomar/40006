#include "Geometry.h"
#include <iostream>

bool Point::operator==(const Point& other) const {
	return x_ == other.x_ && y_ == other.y_;
}

bool Polygon::operator<(const Polygon& other) const {
	return calculateArea(this->points_) < calculateArea(other.points_);
}

bool Polygon::operator==(const Polygon& other) const {
	return points_ == other.points_;
}

std::istream& operator>>(std::istream& in, Point& p) {
	char c;
	if (in >> c && c == '(' && in >> p.x_ >> c && c == ';' && in >> p.y_ >> c && c == ')') {
		return in;
	}
	in.setstate(std::ios::failbit);
	return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
	poly.points_.clear();
	int count;
	if (!(in >> count) || count < 3) {
		in.setstate(std::ios::failbit);
		return in;
	}
	for (int i = 0; i < count; ++i) {
		Point p;
		if (!(in >> p)) {
			in.setstate(std::ios::failbit);
			return in;
		}
		poly.points_.push_back(p);
	}
	return in;
}

double calculateArea(const std::vector<Point>& points) {
	double area = 0.0;
	size_t n = points.size();

	for (size_t i = 0; i < n; i++) {
		int j = (static_cast<int>(i + 1)) % n;
		area += (points[i].x_ * points[j].y_) - (points[i].y_ * points[j].x_);
	}
	return std::abs(area) / 2.0;
}

EvenOddFilter::EvenOddFilter(const std::string& arg) : mod_(arg == "EVEN" ? 0 : 1) {}

bool EvenOddFilter::operator()(const Polygon& figure) const {
	return static_cast<int>(figure.points_.size()) % 2 == mod_;
}


double AddArea::operator()(double acc, const Polygon& p) const {
	return acc + calculateArea(p.points_);
}

double AddFilteredArea::operator()(double acc, const Polygon& p) const {
	return filter_(p) ? acc + calculateArea(p.points_) : acc;
}

AddFilteredArea::AddFilteredArea(const EvenOddFilter& filter) : filter_(filter) {}

double AddSpecificVertexCountArea::operator()(double acc, const Polygon& p) const {
	return (static_cast<int>(p.points_.size()) == n_) ? acc + calculateArea(p.points_) : acc;
}

AddSpecificVertexCountArea::AddSpecificVertexCountArea(int n) : n_(n) {}
