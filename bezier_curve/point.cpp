#include "point.h"

Point::Point(const type lhs, const type rhs)
	:p(std::make_pair(lhs, rhs))
{}

void scalingPoints(std::vector<Point> &v, const Point::type x, const Point::type y)
{
	for (auto& it : v)
		it.scaling(x, y);
}