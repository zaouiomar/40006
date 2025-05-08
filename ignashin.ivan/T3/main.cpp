#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <iterator>
#include "Polygon.h"
#include "Functions.h"
#include "Functors.h"


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
                if (is_empty(data)) {
                    invalidCommand();
                    continue;
                }
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
            if (is_empty(data)) {
                invalidCommand();
                continue;
            }
            std::string arg;
            std::cin >> arg;
            if (arg == "AREA") {
                std::cout << maxMinArea(command, data) << '\n';
            }
            else if (arg == "VERTEXES") {
                std::cout << maxMinVertexes(command, data) << '\n';
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
