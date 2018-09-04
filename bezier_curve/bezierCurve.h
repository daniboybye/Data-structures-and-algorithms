#pragma once

#ifndef __BEZIER_CURVE__
#define __BEZIER_CURVE__

#include "point.h"
#include <vector>

class BezierCurve
{
public:
	void createCurve(const std::vector<Point> &,std::vector<Point>&);
	void draw() const;

	inline void scaling(const Point::type x, const Point::type y)
	{
		scalingPoints(pointsOfTheCurve, x, y);
	}

private:
	std::vector< Point > pointsOfTheCurve;
};

#endif 