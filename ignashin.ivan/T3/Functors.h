#pragma once
#include "Polygon.h"
#include "Functions.h"


struct EvenOddFilter {
    int mod;
    EvenOddFilter(bool arg) : mod(arg ? 0 : 1) {}
    bool operator()(const Polygon& figure) {
        return static_cast<int>(figure.points.size()) % 2 == mod;
    }
};

struct VertexCountComparator {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return a.points.size() < b.points.size();
    }
};

struct AreaComparator {
    bool operator()(Polygon& a, Polygon& b) const {
        return calculateArea(a.points) < calculateArea(b.points);
    }
};

struct EqualPoints {
    bool operator()(const Point& a, const Point& b) {
        return a.x == b.x && a.y == b.y;
    }
};
