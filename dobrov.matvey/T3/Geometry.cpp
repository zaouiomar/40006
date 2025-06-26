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

bool EvenOddFilter::operator()(const Polygon& figure) const {
    return static_cast<int>(figure.points_.size()) % 2 == mod_;
}

double AddArea(double acc, const Polygon& p) {
    return acc + calculateArea(p.points_);
}

double AddFilteredArea::apply(double acc, const Polygon& p) const {
    int size = static_cast<int>(p.points_.size());
    if ((parity_ == Parity::EVEN && size % 2 == 0) ||
        (parity_ == Parity::ODD && size % 2 == 1)) {
        return acc + calculateArea(p.points_);
    }
    return acc;
}

double AddSpecificVertexCountArea::apply(double acc, const Polygon& p) const {
    return (static_cast<int>(p.points_.size()) == n_) ? acc + calculateArea(p.points_) : acc;
}
