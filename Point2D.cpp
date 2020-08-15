#include <math.h>

#include "Point2D.h"

Point2D::Point2D(int px, int py)
{
	initialize(px, py);
}


void Point2D::initialize(int px, int py)
{
	x = px;
	y = py;

}


void Point2D::finalize()
{
	x = 0;
	y = 0;
}


void Point2D::offset(int ox, int oy)
{
	x += ox;
	y += oy;
}


Size2D Point2D::getXYDistance(const Point2D& p)
{
	return Size2D(1 + abs(p.x - x), 1 + abs(p.y - y));
}


int Point2D::getDistance(const Point2D& p)
{
	return getXYDistance(p).getDiagonalLength();
}


bool Point2D::operator ==(const Point2D& p)
{
	return (x == p.x && y == p.y);
}


bool Point2D::operator !=(const Point2D& p)
{
	return (x != p.x || y != p.y);
}
