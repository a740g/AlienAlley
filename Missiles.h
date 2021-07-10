///////////////////////////////////////////////////////////////////////
//     _    _ _                 _    _ _
//    / \  | (_) ___ _ __      / \  | | | ___ _   _ 
//   / _ \ | | |/ _ \ '_ \    / _ \ | | |/ _ \ | | |
//  / ___ \| | |  __/ | | |  / ___ \| | |  __/ |_| |
// /_/   \_\_|_|\___|_| |_| /_/   \_\_|_|\___|\__, |
//                                            |___/
//
//  Conversion/port copyright � Samuel Gomes & Neil Gomes, 1998-2020.
//
///////////////////////////////////////////////////////////////////////

#pragma once

#include "AlienAlley.h"

class Missiles
{
public:
	// Type of missiles
	const enum
	{
		HERO = 0,
		ALIEN,
		MISSILES_TYPE_COUNT
	};

	Missiles();
	~Missiles();
	// Update all missile locations, sprites etc.
	void update();
	// Draw all missiles
	void draw();
	// Add a missile from an object on the screen
	bool add(unsigned int type, bool straight, int x, int y);

private:
	const static int SHOTS_N = 256;

	// Sprite sizes in spritesheets
	const int spriteSheetSize[MISSILES_TYPE_COUNT][2] = { {4, 18}, {5, 5} };
	 
	int bufferWidth;		// Screen width for audio panning & clipping sprites
	int bufferHeight;		// For clipping sprites

	// Missiles array
	struct
	{
		Sprite* sprite;
		int dx, dy;
		int type;
		bool used;
	} shot[SHOTS_N];

	// Effects sprite sheets
	ALLEGRO_BITMAP* spriteSheet[MISSILES_TYPE_COUNT];
	// Missile sounds
	ALLEGRO_SAMPLE* sample[MISSILES_TYPE_COUNT];
};
