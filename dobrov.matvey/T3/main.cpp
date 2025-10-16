#include <iostream>
#include <algorithm>
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

    std::cout << std::fixed << std::setprecision(1);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        std::string arg;

        if (!(iss >> cmd)) {
            invalidCommand();
            continue;
        }

        if (cmd == "RIGHTSHAPES") {
            std::cout << countRightAngleShapes(polygons) << "\n";
            continue;
        }

        if (!(iss >> arg)) {
            invalidCommand();
            continue;
        }

        if (cmd == "AREA") {
            if (arg == "ODD") {
                std::cout << computeAreaEvenOdd(polygons, Parity::ODD) << "\n";
            }
            else if (arg == "EVEN") {
                std::cout << computeAreaEvenOdd(polygons, Parity::EVEN) << "\n";
            }
            else if (arg == "MEAN") {
                if (polygons.empty()) {
                    invalidCommand();
                    continue;
                }

                std::cout << computeAreaMean(polygons) << "\n";
            }
            else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
                int n = std::stoi(arg);
                if (n < 3) {
                    invalidCommand();
                }
                else {
                    std::cout << computeAreaByVertexCount(polygons, n) << "\n";
                }
            }
            else {
                invalidCommand();
            }
        }
        else if (cmd == "MAX") {
            if (polygons.empty()) {
                invalidCommand();
                continue;
            }

            if (arg == "AREA") {
                std::cout << computeExtremumArea(polygons, true) << "\n";
            }
            else if (arg == "VERTEXES") {
                std::cout << computeExtremumVertexes(polygons, true) << "\n";
            }
        }
        else if (cmd == "MIN") {
            if (arg == "AREA") {
                std::cout << computeExtremumArea(polygons, false) << "\n";
            }
            else if (arg == "VERTEXES") {
                std::cout << computeExtremumVertexes(polygons, false) << "\n";
            }
        }
        else if (cmd == "COUNT") {
            if (arg == "EVEN") {
                std::cout << computeCountEvenOdd(polygons, Parity::EVEN) << "\n";
            }
            else if (arg == "ODD") {
                std::cout << computeCountEvenOdd(polygons, Parity::ODD) << "\n";
            }
            else if (std::all_of(arg.begin(), arg.end(), ::isdigit)) {
                int n = std::stoi(arg);
                if (n < 3) {
                    invalidCommand();
                }
                else {
                    std::cout << computeCountByVertexCount(polygons, n) << "\n";
                }
            }
        }
        else if (cmd == "ECHO") {
            std::string rest;
            std::getline(iss, rest);

            std::istringstream polyStream(arg + rest);
            Polygon target;
            if (!(polyStream >> target) || hasMoreArguments(polyStream)) {
                invalidCommand();
                continue;
            }
            int inserted = computeEcho(polygons, target);
            std::cout << inserted << "\n";
        }
        else {
            invalidCommand();
        }
    }
}
