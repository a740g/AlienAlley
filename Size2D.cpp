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
	return w >= 0 && h >= 0;
}

// Returns true of either width or height <= 0
bool Size2D::isNothing()
{
	return w <= 0 || h <= 0;
}

// What is the diagonal length?
int Size2D::getDiagonalLength()
{
	return (int)sqrt(w * w + h * h);
}

// What is the area?
int Size2D::getArea()
{
	return w * h;
}

// What is the perimeter
int Size2D::getPerimeter()
{
	return w * 2 + h * 2;
}

// Resizes the width and height and returns if the size is valid
bool Size2D::resize(int x, int y)
{
	w += x;
	h += y;

	return w >= 0 && h >= 0;
}

// Are two sizes equal?
bool Size2D::operator ==(const Size2D &s)
{
	return w == s.w && h == s.h;
}

// Are to sizes not equal?
bool Size2D::operator !=(const Size2D &s)
{
	return w != s.w || h != s.h;
}
