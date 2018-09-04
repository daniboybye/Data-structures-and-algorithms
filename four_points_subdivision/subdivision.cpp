#include "subdivision.h"

#include <cassert>
#include <algorithm>

#define LINE1_WIDTH  4
#define LINE1_COLOR  0.0,     50.0,    155.0
#define LINE2_WIDTH  4
#define POINT_COLOR  153.0f , 0.0f   , 153.0f
#define LINE2_COLOR  255.0f , 128.0f , 0.0f

const Point::type Subdivision::c1 = - 1.0 / 16.0;
const Point::type Subdivision::c2 = 9.0 / 16.0;

static auto f(const Point& a, const Point& b, const Point& c, const Point& d)
{
	return Subdivision::c1 * (a + d) + Subdivision::c2 * (b + c);
}

void Subdivision::erasePointsNearTo(const Point& p)
{
	points.erase(std::remove_if(points.begin(), points.end(), [p](const Point& t) {
		return p.isNear(t, POINT_WIDTH / 3);
	}), points.end());
	
	nextPoints.clear();

	update();
}

void Subdivision::doNextIteration()
{
	nextPoints.reserve(2 * points.size());

	if (isClose)
	{
		nextPoints.push_back(points.front());
		nextPoints.push_back(f(points.back(), points[0], points[1], points[2]));
	}

	nextPoints.push_back(points[1]);

	const auto n = points.size() - 1;

	for (size_t i = 2; i < n; ++i)
	{
		nextPoints.push_back(f(
			points[i - 2], 
			points[i - 1],
			points[i + 0], 
			points[i + 1]));

		nextPoints.push_back(points[i]);
	}

	if (isClose)
	{
		nextPoints.push_back(f(points[n - 2], points[n - 1], points[n], points.front()));
		nextPoints.push_back(points.back());
		nextPoints.push_back(f(points[n - 1], points[n], points[0], points[1]));
	}
}

void Subdivision::nextIteration()
{
	if (!check())
		return;
	
	points.clear();
	std::swap(points, nextPoints);

	doNextIteration();
}

void Subdivision::update()
{
	if (!check())
		return;

	nextPoints.clear();

	doNextIteration();
}

void Subdivision::addPoint(const Point &p)
{
	points.push_back(p);
	update();
}

void Subdivision::scaling(const Point::type x, const Point::type y)
{
	scalingPoints(nextPoints, x, y);
	scalingPoints(points, x, y);
}

void Subdivision::drawPoints() const
{
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(POINT_WIDTH);
	glColor3f(POINT_COLOR);
	glDrawArrays(GL_POINTS, 0, points.size());
	glDisable(GL_POINT_SMOOTH);
}

void Subdivision::drawPolygon() const
{
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LINE_STIPPLE);
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(LINE2_WIDTH);
	glColor3f(LINE2_COLOR);
	glDrawArrays(isClose ? GL_LINE_LOOP : GL_LINE_STRIP, 0, points.size());
	glPopAttrib();
	glDisable(GL_LINE_SMOOTH);
}

void Subdivision::drawPointsAndPolygon() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(sizeof(Point) / sizeof(Point::type), Point::typeFlag, 0, points.data());

	drawPolygon();
	drawPoints();

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Subdivision::draw() const
{
	drawPointsAndPolygon();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(sizeof(Point) / sizeof(Point::type), Point::typeFlag, 0,nextPoints.data());

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LINE_STIPPLE);
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(LINE1_WIDTH);
	glColor3f(LINE1_COLOR);
	glDrawArrays(isClose ? GL_LINE_LOOP : GL_LINE_STRIP, 0,nextPoints.size());
	glPopAttrib();
	glDisable(GL_LINE_SMOOTH);

	glDisableClientState(GL_VERTEX_ARRAY);
}