#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include"Geometry.h"

double computeAreaEvenOdd(const std::vector<Polygon>& polygons, Parity parity);
double computeAreaMean(const std::vector<Polygon>& polygons);
double computeAreaByVertexCount(const std::vector<Polygon>& polygons, int n);
double computeExtremumArea(const std::vector<Polygon>& polygons, bool isMax);
int computeExtremumVertexes(const std::vector<Polygon>& polygons, bool isMax);
int computeCountEvenOdd(const std::vector<Polygon>& polygons, Parity parity);
int computeCountByVertexCount(const std::vector<Polygon>& polygons, int n);
int computeEcho(std::vector<Polygon>& polygons, const Polygon& target);
int countRightAngleShapes(const std::vector<Polygon>& polygons);
bool hasMoreArguments(std::istream& in);
void invalidCommand();

#endif
