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
	CSize frames;									// total frames (cx * cy)
	int currentFrame;								// current sprite frame
	int frameSkipMax;								// total number of frames to skip while updating
	int frameSkipCount;								// current frame skip count

public:

	ALLEGRO_BITMAP* spriteSheet;					// pointer to sprite frame bitmaps
	CSize size;										// sprite *frame* size
	CPoint position;								// position of sprite on 2D plane. This is the center of the sprite
	int animationDirection;							// animation driection (can be -ve or +ve)
	int playCount;									// number of times our animation has been played
	CRect boundary;									// Sprite boundary
		
	Sprite(ALLEGRO_BITMAP* ssBitmap = nullptr, int width = 0, int height = 0, int frameSkip = 0);
	bool setBitmap(ALLEGRO_BITMAP* ssBitmap, int width = 0, int height = 0, int frameSkip = 0);
	bool collidesWith(const Sprite& s);
	void draw(int flags = 0);
	void update();
	bool setFrame(int n);
	void reset();
};
