#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <vector>
#include <sstream>
#include "Polygon.h"


std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);

void invalidCommand();
bool is_empty(std::vector<Polygon>& data);
bool isNumber(std::string& arg);
double areaEvenOdd(std::string& arg, std::vector<Polygon>& data);
double areaMean(std::vector<Polygon>& data);
double areaNum(int arg, std::vector<Polygon>& data);
double calculateArea(std::vector<Point>& points);
double maxMinArea(std::string& command, std::vector<Polygon>& data);
size_t maxMinVertexes(std::string& command, std::vector<Polygon>& data);
int count(std::string arg, std::vector<Polygon>& data);
int count(int arg, std::vector<Polygon>& data);
int perms(Polygon& etalon, std::vector<Polygon>& data);
int rightshapes(std::vector<Polygon>& data);
bool hasRightAngle(std::vector<Point>& figure);

#endif
