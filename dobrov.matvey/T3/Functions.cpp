#include "Functions.h"
#include "Geometry.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>

std::string handleArea(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string arg;
    if (!(iss >> arg)) {
        return "<INVALID COMMAND>";
    }

    if (arg == "ODD" || arg == "EVEN") {
        if (!hasNoMoreArguments(iss)) {
            return "<INVALID COMMAND>";
        }
        if (polygons.empty()) {
            return "0.0";
        }

        EvenOddFilter filter(arg);
        AddFilteredArea adder(filter);

        double sum = std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            std::bind(&AddFilteredArea::operator(), adder,
                std::placeholders::_1, std::placeholders::_2)
        );
        return std::to_string(sum);
    }

    else if (arg == "MEAN") {
        if (!hasNoMoreArguments(iss) || polygons.empty()) {
            return "<INVALID COMMAND>";
        }

        AddArea adder;
        double total = std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            std::bind(&AddArea::operator(), adder,
                std::placeholders::_1, std::placeholders::_2)
        );
        return std::to_string(total / polygons.size());
    }

    else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
        int n = std::stoi(arg);
        if (n < 3 || !hasNoMoreArguments(iss)) {
            return "<INVALID COMMAND>";
        }

        AddSpecificVertexCountArea adder(n);
        double sum = std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            std::bind(&AddSpecificVertexCountArea::operator(), adder,
                std::placeholders::_1, std::placeholders::_2)
        );
        return std::to_string(sum);
    }

    return "<INVALID COMMAND>";
}

bool hasNoMoreArguments(std::istringstream& iss) {
    std::string leftover;
    return !(iss >> leftover);
}

std::string handleExtremum(std::istringstream& iss, const std::vector<Polygon>& polygons, bool isMax) {
    std::string arg;
    if (!(iss >> arg)) {
        return "<INVALID COMMAND>";
    }

    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (arg == "AREA") {
        auto it = (isMax)
            ? std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return calculateArea(a.points_) < calculateArea(b.points_);
                })
            : std::min_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return calculateArea(a.points_) < calculateArea(b.points_);
                });
        return std::to_string(calculateArea(it->points_));
    }
    else if (arg == "VERTEXES") {
        auto it = (isMax)
            ? std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return a.points_.size() < b.points_.size();
                })
            : std::min_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return a.points_.size() < b.points_.size();
                });
        return std::to_string(it->points_.size());
    }
    else {
        return "<INVALID COMMAND>";
    }
}

std::string handleCount(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string arg;
    if (!(iss >> arg) || !hasNoMoreArguments(iss)) {
        return "<INVALID COMMAND>";
    }

    if (arg == "EVEN" || arg == "ODD") {
        EvenOddFilter filter(arg);
        int count = std::count_if(polygons.begin(), polygons.end(), filter);
        return std::to_string(count);
    }
    else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
        int n = std::stoi(arg);
        if (n < 3) {
            return "<INVALID COMMAND>";
        }

        int count = std::count_if(polygons.begin(), polygons.end(),
            [n](const Polygon& p) {
                return static_cast<int>(p.points_.size()) == n;
            });
        return std::to_string(count);
    }
    else {
        return "<INVALID COMMAND>";
    }
}

std::string handleEcho(std::istringstream& iss, std::vector<Polygon>& polygons) {
    Polygon target;
    if (!(iss >> target) || !hasNoMoreArguments(iss)) {
        return "<INVALID COMMAND>";
    }

    int inserted = 0;
    for (auto it = polygons.begin(); it != polygons.end(); it++) {
        if (*it == target) {
            it = polygons.insert(std::next(it), target);
            inserted++;
        }
    }

    return std::to_string(inserted);
}

bool hasRightAngle(const Polygon& p) {
    const auto& pts = p.points_;
    size_t n = pts.size();

    for (size_t i = 0; i < n; i++) {
        Point a = pts[(i + n - 1) % n];
        Point b = pts[i];
        Point c = pts[(i + 1) % n];

        int dx1 = b.x_ - a.x_;
        int dy1 = b.y_ - a.y_;
        int dx2 = c.x_ - b.x_;
        int dy2 = c.y_ - b.y_;

        int dot = dx1 * dx2 + dy1 * dy2;
        if (dot == 0) return true;
    }
    return false;
}

std::string handleRightShapes(const std::vector<Polygon>& polygons) {
    int count = std::count_if(polygons.begin(), polygons.end(), hasRightAngle);
    return std::to_string(count);
}
