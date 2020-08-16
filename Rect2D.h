#pragma once

#include "AlienAlley.h"

class Size2D;
class Point2D;

// 2D Rect class
class Rect2D
{
public:
    // Win32 RECT
    RECT rect;

    Rect2D(int ax = 0, int ay = 0, int bx = 0, int by = 0);
    Rect2D(const Point2D& p1, const Point2D& p2);
    Rect2D(const Point2D& p, const Size2D& s);

    void initialize(int ax = 0, int ay = 0, int bx = 0, int by = 0);
    void initialize(const Point2D& p1, const Point2D& p2);
    void initialize(const Point2D& p, const Size2D& s);
    void finalize();
    bool isValid();
    void normalize();
    bool isEmpty();								// if size is 0,0
    Point2D getLeftTop();
    Point2D getRightBottom();
    void setLeftTop(const Point2D& p);
    void setRightBottom(const Point2D& p);
    Size2D getSize();
    void inflate(int x, int y);
    void inflate(const Size2D& s);
    int getDiagonalLength();
    int getArea();
    int getPerimeter();
    bool overlaps(const Rect2D& r);
    void move(int x, int y);
    void move(const Point2D& p);
    void makeIntersection(const Rect2D& r);
    void makeUnion(const Rect2D& r);
    void subtract(const Rect2D& r);
    bool intersects(const Rect2D& r);
    bool contains(int p, int y);
    bool contains(const Point2D& p);
    bool contains(const Rect2D& r);

    bool operator ==(const Rect2D& r);
    bool operator !=(const Rect2D& r);
};
