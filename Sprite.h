#pragma once

#include "AlienAlley.h"

class Sprite
{
public:

	Size2D size;
	Point2D position;
	int frames;
	int currentFrame;
	int animationDirection;
	std::vector<ALLEGRO_BITMAP*> frameList;
	
	Sprite(const char* fname = nullptr, int width = 0, int height = 0, int rows = 1, int columns = 1);
	~Sprite();
	bool loadBitmap(const char* fname, int width = 0, int height = 0, int rows = 1, int columns = 1);
	bool collidesWith(const Sprite& s);
	void draw(int flags = 0);
	void nextFrame();
	bool setFrame(int n);
	void reset();
};
