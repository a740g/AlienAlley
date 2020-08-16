#pragma once

#include "AlienAlley.h"

class Size2D;

// 2D Point class
class Point2D
{
public:
    // Absolute 2D coordinates
    int x;
    int y;

    Point2D(int px = 0, int py = 0);

    void initialize(int px = 0, int py = 0);
    void finalize();
    void offset(int ox, int oy);
    Size2D getXYDistance(const Point2D& p);
    int getDistance(const Point2D& p);

    bool operator ==(const Point2D& p);
    bool operator !=(const Point2D& p);
};
