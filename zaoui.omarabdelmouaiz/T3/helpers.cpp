#include "helpers.hpp"
#include <iomanip>
#include "scope_guard.hpp"

size_t omar::getVertices(const Polygon& src)
{
  return src.points.size();
}

bool omar::isPolygonEven(const Polygon& src)
{
  return getVertices(src) % 2 == 0;
}

bool omar::isPolygonOdd(const Polygon& src)
{
  return getVertices(src) % 2 != 0;
}

bool omar::isPolygonOfSize(const Polygon& src, const size_t& userSize)
{
  return getVertices(src) == userSize;
}

bool omar::isStringNumeric(const std::string& str)
{
  using namespace std::placeholders;
  auto isNumericBind = std::bind(isdigit, _1);
  return std::all_of(str.begin(), str.end(), isNumericBind);
}

double omar::areaTermCalculate(const std::vector< Point >& points, size_t index)
{
  size_t next = (index + 1) % points.size();
  double ySum = points[index].y + points[next].y;
  double xDiff = points[index].x - points[next].x;
  return ySum * xDiff;
}

double omar::calcArea(const Polygon& src)
{
  using namespace std::placeholders;

  size_t n = src.points.size();
  std::vector< size_t > indices(n);
  std::iota(indices.begin(), indices.end(), 0);

  std::vector< double > areaTerms(n);
  auto termCalcBind = std::bind(areaTermCalculate, std::cref(src.points), _1);
  std::transform(indices.begin(), indices.end(), std::back_inserter(areaTerms), termCalcBind);

  std::vector< double > ones(areaTerms.size(), 1.0);
  double area = std::inner_product(areaTerms.begin(), areaTerms.end(), ones.begin(), 0.0);

  return std::abs(area) / 2.0;
}

void omar::areaOut(double result, std::ostream& out)
{
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << result << '\n';
}

bool omar::compareAreas(const Polygon& lhs, const Polygon& rhs)
{
  return calcArea(lhs) < calcArea(rhs);
}

bool omar::compareVertices(const Polygon& lhs, const Polygon& rhs)
{
  return getVertices(lhs) < getVertices(rhs);
}

omar::Polygon omar::minElement(const std::vector< Polygon >& data, comparatorFunction comparator)
{
  return *std::min_element(data.begin(), data.end(), comparator);
}

omar::Polygon omar::maxElement(const std::vector< Polygon >& data, comparatorFunction comparator)
{
  return *std::max_element(data.begin(), data.end(), comparator);
}

bool omar::rmEchoHelper(const Polygon& rmPolygon, const Polygon& lhs, const Polygon& rhs)
{
  return (rmPolygon == lhs) && (rmPolygon == rhs);
}

bool omar::compareX(const Point& lhs, const Point& rhs)
{
  return lhs.x < rhs.x;
}

bool omar::compareY(const Point& lhs, const Point& rhs)
{
  return lhs.y < rhs.y;
}

omar::Borders omar::getPolygonBox(const Polygon& src)
{
  auto minX = std::min_element(src.points.begin(), src.points.end(), compareX);
  auto minY = std::min_element(src.points.begin(), src.points.end(), compareY);
  auto maxX = std::max_element(src.points.begin(), src.points.end(), compareX);
  auto maxY = std::max_element(src.points.begin(), src.points.end(), compareY);
  Borders result = Borders((*minX).x, (*minY).y, (*maxX).x, (*maxY).y);
  return result;
}

bool omar::compareMinX(const Polygon& lhs, const Polygon& rhs)
{
  Borders lhsBox = getPolygonBox(lhs);
  Borders rhsBox = getPolygonBox(rhs);
  return lhsBox.minX_ < rhsBox.minX_;
}

bool omar::compareMaxX(const Polygon& lhs, const Polygon& rhs)
{
  Borders lhsBox = getPolygonBox(lhs);
  Borders rhsBox = getPolygonBox(rhs);
  return lhsBox.maxX_ < rhsBox.maxX_;
}

bool omar::compareMinY(const Polygon& lhs, const Polygon& rhs)
{
  Borders lhsBox = getPolygonBox(lhs);
  Borders rhsBox = getPolygonBox(rhs);
  return lhsBox.minY_ < rhsBox.minY_;
}

bool omar::compareMaxY(const Polygon& lhs, const Polygon& rhs)
{
  Borders lhsBox = getPolygonBox(lhs);
  Borders rhsBox = getPolygonBox(rhs);
  return lhsBox.maxY_ < rhsBox.maxY_;
}

omar::Borders omar::getMaxPolygonBox(const std::vector< Polygon >& polys)
{
  auto minX = getPolygonBox(*std::min_element(polys.begin(), polys.end(), compareMinX));
  auto minY = getPolygonBox(*std::min_element(polys.begin(), polys.end(), compareMinY));
  auto maxX = getPolygonBox(*std::max_element(polys.begin(), polys.end(), compareMaxX));
  auto maxY = getPolygonBox(*std::max_element(polys.begin(), polys.end(), compareMaxY));
  Borders result = Borders(minX.minX_, minY.minY_, maxX.maxX_, maxY.maxY_);
  return result;
}

bool omar::isPointInBorders(const omar::Point& pnt, const omar::Borders& box)
{
  return pnt.x >= box.minX_ && pnt.x <= box.maxX_ && pnt.y >= box.minY_ && pnt.y <= box.maxY_;
}
