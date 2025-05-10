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

        if (cmd == "AREA") handleArea(iss, polygons);
        else if (cmd == "MAX") handleExtremum(iss, polygons, true);
        else if (cmd == "MIN") handleExtremum(iss, polygons, false);
        else if (cmd == "COUNT") handleCount(iss, polygons);
        else if (cmd == "ECHO") handleEcho(iss, polygons);
        else if (cmd == "RIGHTSHAPES") handleRightShapes(polygons);
        else std::cout << "<INVALID COMMAND>\n";
    }
}
