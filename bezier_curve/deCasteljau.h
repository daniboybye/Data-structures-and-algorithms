#pragma once

#ifndef __HEADER_DE_CASTELJAU__
#define __HEADER_DE_CASTELJAU__

#include "point.h"

class DeCasteljau
{
public:
	DeCasteljau(const std::vector< Point >&, std::vector< Point >&, std::vector< Point>&);

	void naive();
	void method();

private:
	const std::vector< Point > &controlPoints;
	std::vector< Point > &pointsOfTheCurve;
	std::vector< Point > &temp;

	const Point& findPointOfTheCurve(const Point::type);

	void helpRec(const std::pair< const Point::type, const Point&>&,
					const std::pair< const Point::type, const Point&>&);
};

#endif