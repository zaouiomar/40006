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

enum class Parity
{
    EVEN,
    ODD
};

struct EvenOddFilter {
    int mod_;
    EvenOddFilter(const std::string& arg);

    bool operator()(const Polygon& figure) const;
};

struct AddFilteredArea {
    Parity parity_;

    AddFilteredArea(Parity parity) : parity_(parity) {}

    double apply(double acc, const Polygon& p) const;
};

struct AddSpecificVertexCountArea {
    int n_;
    AddSpecificVertexCountArea(int n);

    double apply(double acc, const Polygon& p) const;
};

double AddArea(double acc, const Polygon& p);

#endif
