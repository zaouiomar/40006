#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>

struct Point {
    int x_, y_;

    bool operator==(const Point& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }
};

double calculateArea(const std::vector<Point>& points);

struct Polygon {
    std::vector<Point> points_;

    bool operator<(const Polygon& other) const {
        return calculateArea(this->points_) < calculateArea(other.points_);
    }

    bool operator==(const Polygon& other) const {
        return points_ == other.points_;
    }
};

struct EvenOddFilter {
    int mod_;
    EvenOddFilter(std::string& arg) : mod_(arg == "EVEN" ? 0 : 1) {}

    bool operator()(const Polygon& figure) const {
        return static_cast<int>(figure.points_.size()) % 2 == mod_;
    }
};

void handleArea(std::istringstream& iss, const std::vector<Polygon>& polygons);
void handleExtremum(std::istringstream& iss, const std::vector<Polygon>& polygons, bool isMax);
void handleCount(std::istringstream& iss, const std::vector<Polygon>& polygons);
void handleEcho(std::istringstream& iss, std::vector<Polygon>& polygons);
void handleRightShapes(const std::vector<Polygon>& polygons);
bool hasNoMoreArguments(std::istringstream& iss);
std::istream& operator>>(std::istream& in, Polygon& poly);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: txt file don't exist\n";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Error: cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;

    std::copy(
        std::istream_iterator<Polygon>(input),
        std::istream_iterator<Polygon>(),
        std::back_inserter(polygons)
    );

    std::string line;
    std::cout << std::fixed << std::setprecision(1);
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        if (!(iss >> cmd)) {
            std::cout << "<INVALID COMMAND>\n";
            continue;
        }

        if (cmd == "AREA") handleArea(iss, polygons);
        else if (cmd == "MAX") handleExtremum(iss, polygons, true);
        else if (cmd == "MIN") handleExtremum(iss, polygons, false);
        else if (cmd == "COUNT") handleCount(iss, polygons);
        else if (cmd == "ECHO") handleEcho(iss, polygons);
        else if (cmd == "RIGHTSHAPES") handleRightShapes(polygons);
        else std::cout << "<INVALID COMMAND>\n";
    }
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
    for (int i = 0; i < count; i++) {
        Point p;
        if (!(in >> p)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        poly.points_.push_back(p);
    }
    return in;
}

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
            std::cout << "<INVALID COMMAND>\n";
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
            std::cout << "0.0\n";
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

double calculateArea(const std::vector<Point>& points) {
    double area = 0.0;
    size_t n = points.size();

    for (size_t i = 0; i < n; i++) {
        int j = (static_cast<int>(i + 1)) % n;
        area += (points[i].x_ * points[j].y_) - (points[i].y_ * points[j].x_);
    }
    return std::abs(area) / 2.0;
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
