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

class Hero
{
private:
	int bufferWidth;					// framebuffer width
	int bufferHeight;					// framebuffer height
	int shotTimer;
	int respawnTimer;
	int lives;

public:
	const static int SPEED_DEFAULT = 3;
	const static int INVINCIBLE_TIMER_DEFAULT = 120;

	// Hero sprite
	Sprite sprite;
	// Hero speed
	int speed;
	// Hero invincibility
	int invincibleTimer;

	Hero();
	~Hero();
	void update(int lives, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, bool shoot);
	void draw();
};

