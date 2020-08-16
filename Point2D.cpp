#include "Point2D.h"

Point2D::Point2D(int px, int py)
{
	initialize(px, py);
}

void Point2D::initialize(int px, int py)
{
	point.x = px;
	point.y = py;
}

void Point2D::finalize()
{
	point.x = 0;
	point.y = 0;
}

void Point2D::offset(int ox, int oy)
{
	point.x += ox;
	point.y += oy;
}

Size2D Point2D::getXYDistance(const Point2D& p)
{
	return Size2D(1 + abs(p.point.x - point.x), 1 + abs(p.point.y - point.y));
}

int Point2D::getDistance(const Point2D& p)
{
	return getXYDistance(p).getDiagonalLength();
}

bool Point2D::operator ==(const Point2D& p)
{
	return point.x == p.point.x && point.y == p.point.y;
}

bool Point2D::operator !=(const Point2D& p)
{
	return point.x != p.point.x || point.y != p.point.y;
}
