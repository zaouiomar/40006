#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <limits>
#include <iterator>
#include <functional>


struct Point
{
    int x, y;
};
struct Polygon
{
    std::vector< Point > points;
};


std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);


void invalidCommand();
bool isNumber(std::string& arg);
double areaEvenOdd(std::string& arg, std::vector<Polygon>& data);
double areaMean(std::vector<Polygon>& data);
double areaNum(int arg, std::vector<Polygon>& data);
double calculateArea(std::vector<Point>& points);
double maxMinArea(std::string& command, std::vector<Polygon>& data);
size_t maxMinVertexes(std::string& command, std::vector<Polygon>& data);
int count(std::string arg, std::vector<Polygon>& data);
int count(int arg, std::vector<Polygon>& data);
auto perms(Polygon& etalon, std::vector<Polygon>& data);
int rightshapes(std::vector<Polygon>& data);
bool hasRightAngle(std::vector<Point>& figure);


struct EvenOddFilter {
    int mod;
    EvenOddFilter(std::string& arg) : mod(arg == "EVEN" ? 0 : 1) {}
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


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "ERROR no filename" << '\n';
        return 1;
    }

    const std::string filename = argv[1];
    std::vector<Polygon> data;
    std::ifstream input{ filename };

    if (!input.is_open()) {
        std::cerr << "ERROR\n";
        return 1;
    }

    while (!input.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(input),
            std::istream_iterator<Polygon>(),
            std::back_inserter(data)
        );

        if (input.fail() && !input.eof()) {
            input.clear();
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string command;
    std::cout << std::fixed << std::setprecision(1);
    while (std::cin >> command) {
        if (command == "AREA") {
            std::string arg;
            std::cin >> arg;
            if (arg == "ODD" || arg == "EVEN") {
                std::cout << areaEvenOdd(arg, data) << '\n';
            }
            else if (arg == "MEAN") {
                std::cout << areaMean(data) << '\n';

            }
            else if (isNumber(arg) && std::stoi(arg) >= 3) {
                std::cout << areaNum(std::stoi(arg), data) << '\n';
            }
            else {
                invalidCommand();
            }
        }
        else if (command == "MAX" || command == "MIN") {
            if (data.size() == static_cast<size_t>(0)) {
                invalidCommand();
                continue;
            }
            std::string arg;
            std::cin >> arg;
            if (arg == "AREA") {
                maxMinArea(command, data);
            }
            if (arg == "VERTEXES") {
                maxMinVertexes(command, data);
            }
            else {
                invalidCommand();
            }
        }
        else if (command == "COUNT") {
            std::string arg;
            std::cin >> arg;
            if (arg == "EVEN" || arg == "ODD") {
                std::cout << count(arg, data) << '\n';
            }
            else if (isNumber(arg) && std::stoi(arg) >= 3) {
                std::cout << count(std::stoi(arg), data) << '\n';
            }
            else {
                invalidCommand();
            }
        }
        else if (command == "PERMS") {
            Polygon etalon;
            std::string line;

            std::getline(std::cin >> std::ws, line);
            std::istringstream iss(line);

            if (!(iss >> etalon)) {
                invalidCommand();
            }
            else {
                std::cout << perms(etalon, data) << '\n';
            }
        }
        else if (command == "RIGHTSHAPES") {
            std::cout << rightshapes(data) << '\n';
        }
        else {
            invalidCommand();
        }
    }
}


std::istream& operator>>(std::istream& in, Point& p) {
    char ch;
    if (in >> ch && ch == '(' && in >> p.x >> ch && ch == ';' && in >> p.y >> ch && ch == ')') {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
    poly.points.clear();
    int count;

    if (!(in >> count)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (count < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < count; ++i) {
        Point p;
        if (!(in >> p)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        poly.points.push_back(p);
    }

    char nextChar = in.peek();
    if (nextChar != '\n' && nextChar != EOF) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

void invalidCommand() {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isNumber(std::string& arg) {
    try {
        std::stoi(arg);
        return true;
    }
    catch (...) {
        return false;
    }
}

double areaEvenOdd(std::string& arg, std::vector<Polygon>& data) {
    EvenOddFilter filter(arg);

    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [&filter](double sum, Polygon& figure) {
            if (filter(figure)) {
                return sum + calculateArea(figure.points);
            }
            return sum;
        }
    );
    return output;
}

double areaMean(std::vector<Polygon>& data) {
    if (data.size() == static_cast<size_t>(0)) {
        invalidCommand();
        return 0;
    }

    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [](double sum, Polygon& figure) {
            return sum + calculateArea(figure.points);
        }
    );

    return output / static_cast<double>(data.size());
}

double areaNum(int arg, std::vector<Polygon>& data) {
    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [arg](double sum, Polygon& figure) {
            if (figure.points.size() == static_cast<size_t>(arg)) {
                return sum + calculateArea(figure.points);
            }
            return sum;
        }
    );

    return output;
}

double calculateArea(std::vector<Point>& points) {
    double area = 0.0;
    size_t n = points.size();

    for (size_t i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += (points[i].x * points[j].y) - (points[i].y * points[j].x);
    }
    return std::abs(area) / 2.0;
}

double maxMinArea(std::string& command, std::vector<Polygon>& data) {
    auto output = (command == "MAX")
        ? std::max_element(data.begin(), data.end(), AreaComparator())
        : std::min_element(data.begin(), data.end(), AreaComparator());
    return calculateArea(output->points);
}

size_t maxMinVertexes(std::string& command, std::vector<Polygon>& data) {
    auto output = (command == "MAX")
        ? std::max_element(data.begin(), data.end(), VertexCountComparator())
        : std::min_element(data.begin(), data.end(), VertexCountComparator());
    return output->points.size();
}

int count(std::string arg, std::vector<Polygon>& data) {
    EvenOddFilter filter(arg);
    int output = std::count_if(data.begin(), data.end(), filter);
    return output;
}

int count(int arg, std::vector<Polygon>& data) {
    int output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [arg](double sum, Polygon& figure) {
            if (figure.points.size() == static_cast<size_t>(arg)) {
                return sum + 1;
            }
            return sum;
        }
    );
    return output;
}

auto perms(Polygon& etalon, std::vector<Polygon>& data) {
    auto output = std::count_if(data.begin(), data.end(),
        [&etalon](const Polygon& poly) {
            return poly.points.size() == etalon.points.size() &&
                std::is_permutation(
                    poly.points.begin(), poly.points.end(),
                    etalon.points.begin(),
                    std::bind(
                        [](const Point& a, const Point& b) {
                            return a.x == b.x && a.y == b.y;
                        },
                        std::placeholders::_1,
                        std::placeholders::_2
                    ));
        });
    return output;
}

int rightshapes(std::vector<Polygon>& data) {
    int count = std::count_if(data.begin(), data.end(),
        [](Polygon& figure) {
            return hasRightAngle(figure.points);
        });
    return count;
}


bool hasRightAngle(std::vector<Point>& figure) {
    for (size_t i = 0; i < figure.size(); ++i) {
        const Point& prev = figure[(i + figure.size() - 1) % figure.size()];
        const Point& curr = figure[i];
        const Point& next = figure[(i + 1) % figure.size()];

        int ba_x = prev.x - curr.x;
        int ba_y = prev.y - curr.y;
        int bc_x = next.x - curr.x;
        int bc_y = next.y - curr.y;

        int dot = ba_x * bc_x + ba_y * bc_y;

        if (dot == 0) {
            return true;
        }
    }
    return false;
}

