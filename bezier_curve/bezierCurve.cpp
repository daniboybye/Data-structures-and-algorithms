#include "bezierCurve.h"
#include "deCasteljau.h"

#include <cassert>
#include <algorithm>

#define CURVE_WIDTH 4
#define CURVE_COLOR 0.0 , 255.0, 0.0

void BezierCurve::createCurve(const std::vector<Point> &points, std::vector<Point> &helpVector)
{
	DeCasteljau(points, pointsOfTheCurve, helpVector).method();
}

void BezierCurve::draw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(sizeof(Point)/sizeof(Point::type), Point::typeFlag, 0, pointsOfTheCurve.data());

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(CURVE_WIDTH);
	glColor3f(CURVE_COLOR);
	glDrawArrays(GL_POINTS, 0, pointsOfTheCurve.size());
	glDisable(GL_POINT_SMOOTH);

	glDisableClientState(GL_VERTEX_ARRAY);
}