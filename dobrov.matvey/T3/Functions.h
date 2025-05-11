#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include"Geometry.h"

std::string handleArea(std::istringstream& iss, const std::vector<Polygon>& polygons);
std::string handleExtremum(std::istringstream& iss, const std::vector<Polygon>& polygons, bool isMax);
std::string handleCount(std::istringstream& iss, const std::vector<Polygon>& polygons);
std::string handleEcho(std::istringstream& iss, std::vector<Polygon>& polygons);
std::string handleRightShapes(const std::vector<Polygon>& polygons);
bool hasNoMoreArguments(std::istringstream& iss);

#endif
