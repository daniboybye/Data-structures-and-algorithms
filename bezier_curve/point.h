#pragma once

#ifndef  __HEADER_POINT_
#define  __HEADER_POINT_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <utility>
#include <vector>

class Point
{
public:
	typedef GLdouble type;
	static const GLenum typeFlag = GL_DOUBLE;

	std::pair<type, type> p;

	Point() {}

	Point(const type, const type);

	auto& operator*=(const type);

	auto& operator+=(const Point&);

	void scaling(const type, const type);

	auto isNear(const Point& point, const type d) const;
};

inline auto Point::isNear(const Point& point, const type d) const
{
	const auto x = p.first - point.p.first;
	const auto y = p.second - point.p.second;

	return x * x + y * y <= d * d;
}

inline auto dist(const Point& lhs, const Point& rhs)
{
	return std::hypot(lhs.p.first - rhs.p.first, lhs.p.second - rhs.p.second);
}

inline auto operator==(const Point& lhs, const Point& rhs)
{
	return lhs.isNear(rhs, 1.0);
}

inline auto operator!=(const Point& lhs, const Point& rhs)
{
	return !(lhs == rhs);
}

void scalingPoints(std::vector<Point>&, const Point::type, const Point::type);

inline auto& Point::operator*=(const type num)
{
	this->scaling(num, num);

	return *this;
}

inline auto& Point::operator+=(const Point& other)
{
	p.first += other.p.first;
	p.second += other.p.second;

	return *this;
}

inline const auto operator*(const Point& lhs, const double rhs)
{
	return Point(lhs) *= rhs;
}

inline void Point::scaling(const Point::type x, const Point::type y)
{
	p.first *= x;
	p.second *= y;
}

inline const auto operator+(const Point& lhs, const Point& rhs)
{
	return Point(lhs) += rhs;
}


inline const auto operator*(const double lhs, const Point& rhs)
{
	return rhs * lhs;
}

#endif