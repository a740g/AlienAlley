///////////////////////////////////////////////////////////////////////
//     _    _ _                 _    _ _
//    / \  | (_) ___ _ __      / \  | | | ___ _   _ 
//   / _ \ | | |/ _ \ '_ \    / _ \ | | |/ _ \ | | |
//  / ___ \| | |  __/ | | |  / ___ \| | |  __/ |_| |
// /_/   \_\_|_|\___|_| |_| /_/   \_\_|_|\___|\__, |
//                                            |___/
//
//  Conversion/port copyright © Samuel Gomes & Neil Gomes, 1998-2020.
//
///////////////////////////////////////////////////////////////////////

#pragma once

#include "AlienAlley.h"

class Sprite
{
private:
	ALLEGRO_BITMAP* spriteSheet;					// pointer to sprite frame bitmaps
	CSize size;										// sprite size
	CSize frames;									// total frames (cx * cy)
	int currentFrame;								// current sprite frame

public:

	CPoint position;								// position of sprite on 2D plane
	int animationDirection;							// animation driection (can be -ve or +ve)
		
	Sprite(ALLEGRO_BITMAP* ssBitmap = nullptr, int width = 0, int height = 0);
	~Sprite();
	bool setBitmap(ALLEGRO_BITMAP* ssBitmap, int width = 0, int height = 0);
	bool collidesWith(const Sprite& s);
	void draw(int flags = 0);
	void nextFrame();
	bool setFrame(int n);
	void reset();
};
