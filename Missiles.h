///////////////////////////////////////////////////////////////////////
//     _    _ _                 _    _ _
//    / \  | (_) ___ _ __      / \  | | | ___ _   _ 
//   / _ \ | | |/ _ \ '_ \    / _ \ | | |/ _ \ | | |
//  / ___ \| | |  __/ | | |  / ___ \| | |  __/ |_| |
// /_/   \_\_|_|\___|_| |_| /_/   \_\_|_|\___|\__, |
//                                            |___/
//
//  Sourceport / mod copyright © Samuel Gomes
//
///////////////////////////////////////////////////////////////////////

#pragma once

#include "AlienAlley.h"

// Forward declarations
class Effects;

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

	const static int SHOTS_N = 256;

	// Missiles array
	struct
	{
		Sprite* sprite;
		CSize direction;
		int type;
		bool used;
	} shot[SHOTS_N];

	Missiles();
	~Missiles();
	// Update all missile locations, sprites etc.
	void update();
	// This is called by the collision detector if the missile collided with something
	void hit(int n, Effects& fm);
	// Draw all missiles
	void draw();
	// Add a missile from an object on the screen
	bool add(unsigned int type, bool straight, int x, int y);

private:

	// Sprite sizes in spritesheets
	const int spriteSheetSize[MISSILES_TYPE_COUNT][2] = { {4, 18}, {5, 5} };
	 
	// Framebuffer size for audio panning & clipping sprites
	CSize bufferSize;
	// Effects sprite sheets
	ALLEGRO_BITMAP* spriteSheet[MISSILES_TYPE_COUNT];
	// Missile sounds
	ALLEGRO_SAMPLE* sample[MISSILES_TYPE_COUNT];
};
