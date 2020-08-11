#pragma once

#include "AlienAlley.h"

// 2D Size class
class Size2D
{
public:
    // Rectange dimension
    int w;
    int h;

    Size2D(int cx = 0, int cy = 0);				// create from two integers

    void initialize(int cx = 0, int cy = 0);
    void finalize();
    bool isValid();
    bool isNothing();                           // if size is 0,0
    int getDiagonalLength();					// gets the diagonal size
    int getArea();
    int getPerimeter();						    // gets perimeter size
    bool resize(int x, int y);

    // Operations
    bool operator ==(const Size2D& s);
    bool operator !=(const Size2D& s);
};
