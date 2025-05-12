#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include "Functions.h"

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

    while (!input.eof()) {
        std::copy(
            std::istream_iterator<Polygon>(input),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );

        if (input.fail() && !input.eof()) {
            input.clear();
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

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

        if (cmd == "AREA") {
            std::cout << handleArea(iss, polygons) << "\n";
        }
        else if (cmd == "MAX") {
            std::cout << handleExtremum(iss, polygons, true) << "\n";
        }
        else if (cmd == "MIN") {
            std::cout << handleExtremum(iss, polygons, false) << "\n";
        }
        else if (cmd == "COUNT") {
            std::cout << handleCount(iss, polygons) << "\n";
        }
        else if (cmd == "ECHO") {
            std::cout << handleEcho(iss, polygons) << "\n";
        }
        else if (cmd == "RIGHTSHAPES") {
            std::cout << handleRightShapes(polygons) << "\n";
        }
        else std::cout << "<INVALID COMMAND>\n";
    }
}
