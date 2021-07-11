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
class Sprite;

class Hero
{
private:
	const static int SPEED_DEFAULT = 3;
	const static int INVINCIBLE_TIMER_DEFAULT = 180;
	const static int RESPAWN_TIMER_DEFAULT = 90;
	const static int SHOT_TIMER_DEFAULT = 20;

	int bufferWidth;					// framebuffer width
	int bufferHeight;					// framebuffer height
	int shotTimer;						// can we shoot?
	int respawnTimer;					// can we respawn?
	int invincibleTimer;				// hero invincibility
	int speed;							// hero speed

public:
	// Hero sprite
	Sprite* sprite;

	Hero(HUD& hm);
	~Hero();
	// Update hero location, add missiles and effects based on input
	void update(bool moveLeft, bool moveRight, bool moveUp, bool moveDown, bool shoot, HUD& hm, Missiles& mm);
	// This is called by the collision detector if the hero collided with something
	void hit(bool critical, HUD& hm, Effects& fm);
	void draw(HUD& hm);
};
