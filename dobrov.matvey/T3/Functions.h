#ifndef FUNCTIUONS_H_
#define FUNCTIONS_H_
#include "Geometry.h"

void handleArea(std::istringstream& iss, const std::vector<Polygon>& polygons);
void handleExtremum(std::istringstream& iss, const std::vector<Polygon>& polygons, bool isMax);
void handleCount(std::istringstream& iss, const std::vector<Polygon>& polygons);
void handleEcho(std::istringstream& iss, std::vector<Polygon>& polygons);
void handleRightShapes(const std::vector<Polygon>& polygons);
bool hasNoMoreArguments(std::istringstream& iss);

#endif FUNCTIONS_H_
