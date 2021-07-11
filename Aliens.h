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

// Forward declarations
class HUD;
class Missiles;

class Aliens
{
public:
	// Types of aliens
	const enum
	{
		BUG = 0,
		ARROW,
		VIPER,
		GIM72,
		HOMER,
		BOP,
		EMAGO,
		XTC,
		THICCBOI,
		ALIEN_TYPE_COUNT
	};

	const static int ALIENS_N = 8;				// maximum simultaneous aliens on the screen

	// Aliens array
	struct
	{
		Sprite* sprite;
		int type;
		int shotTimer;
		int blinkTimer;
		int directionTimer;
		int dx;
		int life;
		bool used;
	} alien[ALIENS_N];

	Aliens();
	~Aliens();
	// Run alien AI, update location & sprites
	void update(unsigned long frameCounter, HUD& hm, Missiles& mm, Effects& fm);
	// This is called by the collision detector if the alien collided with something
	void hit(int n, bool critical);
	// Draw alien
	void draw();

private:
	// Sprite sizes in spritesheets
	const int spriteSheetSize[ALIEN_TYPE_COUNT][2] = { {30, 17}, {32, 32}, {32, 32}, {64, 64}, {64, 64}, {64, 64}, {64, 64}, {64, 64}, {80, 75} };

	CSize bufferSize;							// framebuffer size

	// Effects sprite sheets
	ALLEGRO_BITMAP* spriteSheet[ALIEN_TYPE_COUNT];
};
