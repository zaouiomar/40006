#include "Functions.h"
#include "Geometry.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iomanip>

double computeAreaEvenOdd(const std::vector<Polygon>& polygons, Parity parity) {
    AddFilteredArea adder(parity);
    return std::accumulate(
        polygons.begin(), polygons.end(), 0.0,
        std::bind(&AddFilteredArea::apply, adder,
            std::placeholders::_1, std::placeholders::_2));
}

double computeAreaMean(const std::vector<Polygon>& polygons) {
    double total = std::accumulate(
        polygons.begin(), polygons.end(), 0.0,
        std::bind(&AddArea, std::placeholders::_1, std::placeholders::_2)
    );
    return total / polygons.size();
}

double computeAreaByVertexCount(const std::vector<Polygon>& polygons, int n) {
    AddSpecificVertexCountArea adder(n);
    return std::accumulate(
        polygons.begin(), polygons.end(), 0.0,
        std::bind(&AddSpecificVertexCountArea::apply, adder,
            std::placeholders::_1, std::placeholders::_2));
}

double computeExtremumArea(const std::vector<Polygon>& polygons, bool isMax) {
    auto cmpArea = [](const Polygon& a, const Polygon& b) {
        return calculateArea(a.points_) < calculateArea(b.points_);
        };

    auto it = isMax
        ? std::max_element(polygons.begin(), polygons.end(), cmpArea)
        : std::min_element(polygons.begin(), polygons.end(), cmpArea);
    return calculateArea(it->points_);
}

int computeExtremumVertexes(const std::vector<Polygon>& polygons, bool isMax) {
    auto cmpVertexes = [](const Polygon& a, const Polygon& b) {
        return a.points_.size() < b.points_.size();
        };

    auto it = isMax
        ? std::max_element(polygons.begin(), polygons.end(), cmpVertexes)
        : std::min_element(polygons.begin(), polygons.end(), cmpVertexes);
    return it->points_.size();
}

int computeCountEvenOdd(const std::vector<Polygon>& polygons, Parity parity) {
    return std::count_if(polygons.begin(), polygons.end(),
        [parity](const Polygon& p) {
            int size = static_cast<int>(p.points_.size());
            if (parity == Parity::EVEN) {
                return size % 2 == 0;
            }
            else {
                return size % 2 == 1;
            }
        });
}

int computeCountByVertexCount(const std::vector<Polygon>& polygons, int n) {
    return std::count_if(polygons.begin(), polygons.end(),
        [n](const Polygon& p) {
            return static_cast<int>(p.points_.size()) == n;
        });
}

int computeEcho(std::vector<Polygon>& polygons, const Polygon& target) {
    std::vector<size_t> indices;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(indices),
        [&](const Polygon& p) -> size_t {
            return (p == target) ? (&p - &polygons[0]) : std::numeric_limits<size_t>::max();
        });

    indices.erase(std::remove(indices.begin(), indices.end(), std::numeric_limits<size_t>::max()), indices.end());

    std::accumulate(indices.rbegin(), indices.rend(), 0, [&](int acc, size_t idx) {
        polygons.insert(polygons.begin() + idx + 1, target);
        return acc + 1;
        });

    return static_cast<int>(indices.size());
}

bool hasRightAngle(const Polygon& p) {
    const auto& pts = p.points_;
    size_t n = pts.size();

    return std::any_of(pts.begin(), pts.end(), [&](const Point& b) {
        size_t i = &b - &pts[0];
        Point a = pts[(i + n - 1) % n];
        Point c = pts[(i + 1) % n];

        int dx1 = b.x_ - a.x_;
        int dy1 = b.y_ - a.y_;
        int dx2 = c.x_ - b.x_;
        int dy2 = c.y_ - b.y_;

        int dot = dx1 * dx2 + dy1 * dy2;
        return dot == 0;
        });
}

int countRightAngleShapes(const std::vector<Polygon>& polygons) {
    return static_cast<int>(std::count_if(polygons.begin(), polygons.end(), hasRightAngle));
}

void invalidCommand() {
    std::cout << "<INVALID COMMAND>\n";
}
