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


class Effects
{
public:
	// Type of effects
	const enum
	{
		SPARKS = 0,
		EXPLOSION_TINY,
		EXPLOSION_SMALL,
		EXPLOSION_BIG,
		EFFECTS_TYPE_COUNT
	};

	Effects();
	~Effects();
	bool add(unsigned int type, int x, int y);
	void update();
	void draw();

private:

	// Maximum number of simultaneous effects
	const static int FX_N = 128;

	// Sprite sizes in spritesheets
	const int spriteSheetSize[EFFECTS_TYPE_COUNT][2] = { {20, 20}, {32, 32}, {64, 64}, {80, 80} };

	// Screen width for audio panning
	int bufferWidth;

	// Effects array
	struct FXSprite
	{
		Sprite sprite;
		int type;
		bool used;
	} fx[FX_N];

	// Effects sprite sheets
	ALLEGRO_BITMAP* spriteSheet[EFFECTS_TYPE_COUNT];
	// Effects sounds
	ALLEGRO_SAMPLE* sample[EFFECTS_TYPE_COUNT];
};
