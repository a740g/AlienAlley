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
	const static int SPEED_DEFAULT = 3;
	const static int INVINCIBLE_TIMER_DEFAULT = 120;

	int bufferWidth;					// framebuffer width
	int bufferHeight;					// framebuffer height
	int shotTimer;						// can we shoot?
	int respawnTimer;					// can we respawn?
	int invincibleTimer;				// hero invincibility
	int lives;							// this should be sync'ed from the HUD
	int speed;							// hero speed

public:
	// Hero sprite
	Sprite sprite;

	Hero();
	~Hero();
	// Update hero location, add missiles and effects based on input
	void update(int gameLives, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, bool shoot, Missiles& mm, Effects& fm);
	void draw();
};
