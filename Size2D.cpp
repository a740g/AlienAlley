#include "Size2D.h"

// Constructor
Size2D::Size2D(int cx, int cy)
{
	initialize(cx, cy);
}

// Set values of width and height
void Size2D::initialize(int cx, int cy)
{
	size.cx = cx;
	size.cy = cy;
}

// Resets values of width and height
void Size2D::finalize()
{
	size.cx = 0;
	size.cy = 0;
}

// Is the size valid?
bool Size2D::isValid()
{
	return size.cx >= 0 && size.cy >= 0;
}

// Returns true of either width or height <= 0
bool Size2D::isNothing()
{
	return size.cx <= 0 || size.cy <= 0;
}

// What is the diagonal length?
int Size2D::getDiagonalLength()
{
	return (int)sqrt(size.cx * size.cx + size.cy * size.cy);
}

// What is the area?
int Size2D::getArea()
{
	return size.cx * size.cy;
}

// What is the perimeter
int Size2D::getPerimeter()
{
	return size.cx * 2 + size.cy * 2;
}

// Resizes the width and height and returns if the size is valid
bool Size2D::resize(int x, int y)
{
	size.cx += x;
	size.cy += y;

	return size.cx >= 0 && size.cy >= 0;
}

// Are two sizes equal?
bool Size2D::operator ==(const Size2D &s)
{
	return size.cx == s.size.cx && size.cy == s.size.cy;
}

// Are to sizes not equal?
bool Size2D::operator !=(const Size2D &s)
{
	return size.cx != s.size.cx || size.cy != s.size.cy;
}
