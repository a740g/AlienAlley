#include "Rect2D.h"

Rect2D::Rect2D(int ax, int ay, int bx, int by)
{
	initialize(ax, ay, bx, by);
}

Rect2D::Rect2D(const Point2D &p1, const Point2D &p2)
{
	initialize(p1, p2);
}

Rect2D::Rect2D(const Point2D &p, const Size2D &s)
{
	initialize(p, s);
}

void Rect2D::initialize(int ax, int ay, int bx, int by)
{
	SetRect(&rect, ax, ay, bx, by);
}

void Rect2D::initialize(const Point2D &p1, const Point2D &p2)
{
	SetRect(&rect, p1.point.x, p1.point.y, p2.point.x, p2.point.y);
}

void Rect2D::initialize(const Point2D &p, const Size2D &s)
{
	SetRect(&rect, p.point.x, p.point.y, p.point.x + s.size.cx - 1, p.point.y + s.size.cy - 1);
}

void Rect2D::finalize()
{
	SetRectEmpty(&rect);
}

bool Rect2D::isValid()
{
	return rect.right >= rect.left && rect.bottom >= rect.top;
}

void Rect2D::normalize()
{
	if (rect.right < rect.left) std::swap(rect.right, rect.left);
	if (rect.bottom < rect.top) std::swap(rect.bottom, rect.top);
}

bool Rect2D::isEmpty()
{
	return IsRectEmpty(&rect);
}

Point2D Rect2D::getLeftTop()
{
	return Point2D(rect.left, rect.top);
}

Point2D Rect2D::getRightBottom()
{
	return Point2D(rect.right, rect.bottom);
}

void Rect2D::setLeftTop(const Point2D &p)
{
	rect.left = p.point.x;
	rect.top = p.point.y;
}

void Rect2D::setRightBottom(const Point2D &p)
{
	rect.right = p.point.x;
	rect.bottom = p.point.y;
}

Size2D Rect2D::getSize()
{
	return Size2D(1 + rect.right - rect.left, 1 + rect.bottom - rect.top);
}

void Rect2D::inflate(int x, int y)
{
	InflateRect(&rect, x, y);
}

void Rect2D::inflate(const Size2D &s)
{
	InflateRect(&rect, s.size.cx, s.size.cy);
}

int Rect2D::getDiagonalLength()
{
	return getSize().getDiagonalLength();
}

int Rect2D::getArea()
{
	return getSize().getArea();
}

int Rect2D::getPerimeter()
{
	return getSize().getPerimeter();
}

bool Rect2D::overlaps(const Rect2D &r)
{
	return !(rect.left > r.rect.right || r.rect.left > rect.right || rect.top > r.rect.bottom || r.rect.top > rect.bottom);
}

void Rect2D::move(int x, int y)
{
	OffsetRect(&rect, x, y);
}

void Rect2D::move(const Point2D &p)
{
	OffsetRect(&rect, p.point.x, p.point.y);
}

void Rect2D::makeIntersection(const Rect2D &r)
{
	RECT rc;

	IntersectRect(&rc, &rect, &r.rect);
	CopyRect(&rect, &rc);
}

void Rect2D::makeUnion(const Rect2D &r)
{
	RECT rc;

	UnionRect(&rc, &rect, &r.rect);
	CopyRect(&rect, &rc);
}

void Rect2D::subtract(const Rect2D& r)
{
	RECT rc;

	SubtractRect(&rc, &rect, &r.rect);
	CopyRect(&rect, &rc);
}

bool Rect2D::intersects(const Rect2D& r)
{
	return rect.left < r.rect.right && rect.right > r.rect.left && rect.top < r.rect.bottom && rect.bottom > r.rect.top;
}

bool Rect2D::contains(int x, int y)
{
	POINT pt = {x, y};

	return PtInRect(&rect, pt);
}

bool Rect2D::contains(const Point2D &p)
{
	return PtInRect(&rect, p.point);
}

bool Rect2D::contains(const Rect2D &r)
{
	return r.rect.left >= rect.left && r.rect.top >= rect.top && r.rect.right <= rect.right && r.rect.bottom <= rect.bottom;
}

bool Rect2D::operator ==(const Rect2D &r)
{
	return EqualRect(&rect, &r.rect);
}

bool Rect2D::operator !=(const Rect2D &r)
{
	return !EqualRect(&rect, &r.rect);
}
