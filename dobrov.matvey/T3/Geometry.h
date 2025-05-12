#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include <vector>
#include <string>

struct Point {
    int x_, y_;

    bool operator==(const Point& other) const;
};

struct Polygon {
    std::vector<Point> points_;

    bool operator<(const Polygon& other) const;

    bool operator==(const Polygon& other) const;
};

std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);

double calculateArea(const std::vector<Point>& points);

struct EvenOddFilter {
    int mod_;
    EvenOddFilter(const std::string& arg);

    bool operator()(const Polygon& figure) const;
};

struct AddFilteredArea {
    const EvenOddFilter& filter_;

    AddFilteredArea(const EvenOddFilter& filter);

    double operator()(double acc, const Polygon& p) const;
};

struct AddSpecificVertexCountArea {
    int n_;
    AddSpecificVertexCountArea(int n);

    double operator()(double acc, const Polygon& p) const;
};

struct AddArea {
    double operator()(double acc, const Polygon& p) const {
        return acc + calculateArea(p.points_);
    }
};

#endif
