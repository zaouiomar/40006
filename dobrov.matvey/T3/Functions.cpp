#include "Functions.h"
#include "Geometry.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>

void handleArea(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string arg;
    if (!(iss >> arg)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (arg == "ODD" || arg == "EVEN") {
        if (!hasNoMoreArguments(iss)) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        if (polygons.empty()) {
            std::cout << "0.0\n";
            return;
        }

        EvenOddFilter filter(arg);
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [&filter](double acc, const Polygon& p) {
                return filter(p) ? acc + calculateArea(p.points_) : acc;
            });
        std::cout << sum << "\n";
    }

    else if (arg == "MEAN") {
        if (!hasNoMoreArguments(iss)) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        if (polygons.empty()) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        double total = std::accumulate(
            polygons.begin(), polygons.end(), 0.0, std::bind(
                [](double acc, const Polygon& p) {
                    return acc + calculateArea(p.points_);
                },
                std::placeholders::_1,
                std::placeholders::_2
            )
        );
        std::cout << total / polygons.size() << "\n";
    }

    else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
        int n = std::stoi(arg);
        if (n < 3 || !hasNoMoreArguments(iss)) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [n](double acc, const Polygon& p) {
                return (static_cast<int>(p.points_.size()) == n) ? acc + calculateArea(p.points_) : acc;
            });
        std::cout << sum << "\n";
    }

    else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

bool hasNoMoreArguments(std::istringstream& iss) {
    std::string leftover;
    return !(iss >> leftover);
}

void handleExtremum(std::istringstream& iss, const std::vector<Polygon>& polygons, bool isMax) {
    std::string arg;
    if (!(iss >> arg)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
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
        std::cout << calculateArea(it->points_) << "\n";
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
        std::cout << it->points_.size() << "\n";
    }
    else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void handleCount(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string arg;
    if (!(iss >> arg) || !hasNoMoreArguments(iss)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (arg == "EVEN" || arg == "ODD") {
        EvenOddFilter filter(arg);
        int count = std::count_if(polygons.begin(), polygons.end(), filter);
        std::cout << count << "\n";
    }
    else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
        int n = std::stoi(arg);
        if (n < 3) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        int count = std::count_if(polygons.begin(), polygons.end(),
            [n](const Polygon& p) {
                return static_cast<int>(p.points_.size()) == n;
            });
        std::cout << count << "\n";
    }
    else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void handleEcho(std::istringstream& iss, std::vector<Polygon>& polygons) {
    Polygon target;
    if (!(iss >> target) || !hasNoMoreArguments(iss)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    int inserted = 0;
    for (auto it = polygons.begin(); it != polygons.end(); it++) {
        if (*it == target) {
            it = polygons.insert(std::next(it), target);
            inserted++;
        }
    }

    std::cout << inserted << "\n";
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

void handleRightShapes(const std::vector<Polygon>& polygons) {
    int count = std::count_if(polygons.begin(), polygons.end(), hasRightAngle);
    std::cout << count << "\n";
}
