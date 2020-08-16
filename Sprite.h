#pragma once

#include "AlienAlley.h"

class Sprite
{
public:

	std::vector<ALLEGRO_BITMAP*> frameList;			// sprite frame bitmaps
	CSize size;										// sprite size
	int frames;										// total frames
	CPoint position;								// position of sprite on 2D plane
	int currentFrame;								// current sprite frame
	int animationDirection;							// animation driection (can be -ve or +ve)
		
	Sprite(const char* fname = nullptr, int width = 0, int height = 0, int rows = 1, int columns = 1);
	~Sprite();
	bool loadBitmap(const char* fname, int width = 0, int height = 0, int rows = 1, int columns = 1);
	bool collidesWith(const Sprite& s);
	void draw(int flags = 0);
	void nextFrame();
	bool setFrame(int n);
	void reset();
};
