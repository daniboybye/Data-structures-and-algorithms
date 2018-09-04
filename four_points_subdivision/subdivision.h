#pragma once

#ifndef __HEADER_SUBDIVISION__
#define __HEADER_SUBDIVISION__

#include "point.h"
#define POINT_WIDTH 27

class Subdivision
{
public:
	static const Point::type c1;
	static const Point::type c2;

	inline auto cbegin() const
	{
		return points.cbegin();
	}

	inline auto cend() const
	{
		return points.cend();
	}

	inline auto curveIsClose() const
	{
		return isClose;
	}

	inline void setIsClose(const bool flag)
	{
		if (isClose != flag)
		{
			isClose = flag;
			update();
		}
	}

	void nextIteration();

	void scaling(const Point::type, const Point::type);

	inline auto getPointsSize() const
	{
		return points.size();
	}

	inline auto& operator[](const int pos) const
	{
		return points[pos];
	}

	inline void setPoint(const int pos, const Point& p)
	{
		points[pos] = p;
		update();
	}

	void erasePointsNearTo(const Point& p);

	inline void clear()
	{
		points.clear();
		nextPoints.clear();
	}

	void addPoint(const Point &p);

	void draw() const;

private:
	std::vector< Point > points;
	std::vector< Point > nextPoints;
	
	bool isClose;

	void drawPoints() const;
	void drawPolygon() const;
	void drawPointsAndPolygon() const;

	void update();

	void doNextIteration();

	auto check() const
	{
		return points.size() > 4 || (points.size() == 4 && isClose);
	}
};

#endif