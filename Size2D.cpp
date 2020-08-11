#include "Size2D.h"

// Constructor
Size2D::Size2D(int cx, int cy)
{
	initialize(cx, cy);
}

// Set values of width and height
void Size2D::initialize(int cx, int cy)
{
	w = cx;
	h = cy;
}

// Resets values of width and height
void Size2D::finalize()
{
	w = 0;
	h = 0;
}

// Is the size valid?
bool Size2D::isValid()
{
	return (w >= 0 && h >= 0);
}

// Returns true of either width or height <= 0
bool Size2D::isNothing()
{
	return (w <= 0 || h <= 0);
}

// What is the diagonal length?
int Size2D::getDiagonalLength()
{
	return (isValid() ? (int)sqrt((float)(w * w + h * h)) : 0);
}

// What is the area?
int Size2D::getArea()
{
	return (isValid() ? w * h : 0);
}

// What is the perimeter
int Size2D::getPerimeter()
{
	return (isValid() ? w * 2 + h * 2 : 0);
}

// Resizes the width and height
bool Size2D::resize(int x, int y)
{
	w += x;
	h += y;

	return isValid();
}

// Are two sizes equal?
bool Size2D::operator ==(const Size2D &s)
{
	return (w == s.w && h == s.h);
}

// Are to sizes not equal?
bool Size2D::operator !=(const Size2D &s)
{
	return (w != s.w || h != s.h);
}
