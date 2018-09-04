#include "deCasteljau.h"

#include <algorithm>
#include <cassert>
#include <utility>

#define STEP 0.002

#define DIST_TO_POINTS_IN_CURVE 3.8

DeCasteljau::DeCasteljau(const std::vector< Point > &points, 
	std::vector< Point > &result, std::vector< Point> &temp)
	: controlPoints(points)
	, pointsOfTheCurve(result)
	, temp(temp)
{
	assert(points.size() >= 2);
}

void DeCasteljau::helpRec(const std::pair< const Point::type, const Point&>& lhs,
	                      const std::pair< const Point::type, const Point&>& rhs)
{
	const auto delta = (lhs.first + rhs.first) / 2.0;
	auto& point = findPointOfTheCurve(delta);

	pointsOfTheCurve.push_back(point);

	if ( !point.isNear(lhs.second, DIST_TO_POINTS_IN_CURVE) )
		helpRec(lhs, std::make_pair(delta, point));

	if ( !point.isNear(rhs.second, DIST_TO_POINTS_IN_CURVE) )
		helpRec(std::make_pair(delta, point), rhs);
}

void DeCasteljau::method()
{
	pointsOfTheCurve.clear();
	pointsOfTheCurve.push_back(controlPoints.front());
	pointsOfTheCurve.push_back(controlPoints.back());

	helpRec(std::make_pair(0.0, controlPoints.front()), std::make_pair(1.0, controlPoints.back()));
}

void DeCasteljau::naive()
{
	pointsOfTheCurve.clear();

	for (Point::type i = 0.0; i <= 1.0; i += STEP)
		pointsOfTheCurve.push_back(findPointOfTheCurve(i));
}

const Point& DeCasteljau::findPointOfTheCurve(const Point::type delta)
{
	temp = controlPoints;
	const auto t = 1.0 - delta;

	while (temp.size() != 1)
	{
		temp.erase(std::transform(temp.cbegin() + 1, temp.cend(), temp.cbegin(), temp.begin(),
			[t, delta](const Point& lhs, const Point& rhs)
		{
			return delta * lhs + t * rhs;
		}));
	}

	return temp.front();
}