#ifndef POLYGON_H_
#define POLYGON_H_
#include <vector>


struct Point
{
    int x, y;
};

struct Polygon
{
    std::vector< Point > points;
};

#endif
