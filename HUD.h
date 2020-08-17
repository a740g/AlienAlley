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

class HUD
{
private:

	const static int SHIELD_STATUS_WIDTH = 82;
	const static int SHIELD_STATUS_HEIGHT = 22;
	const static int SHIELD_STATUS_LEFT = 191;
	const static int SHIELD_STATUS_TOP = 20;
	const static int SHIELD_STATUS_RIGHT = SHIELD_STATUS_LEFT + SHIELD_STATUS_WIDTH - 1;
	const static int SHIELD_STATUS_BOTTOM = SHIELD_STATUS_TOP + SHIELD_STATUS_HEIGHT - 1;
	const static int SCORE_NUMBERS_LEFT = 474;
	const static int SCORE_NUMBERS_TOP = 24;
	const static int DIGITS_MAX = 7;																			// max digits on the score panel

	ALLEGRO_FONT* font;					// for drawing game over message
	ALLEGRO_BITMAP* digitSpriteSheet;	// digits bitmap for drawing score
	int digitSpriteWidth;				// width of each digit
	int digitSpriteHeight;				// height of each digit
	ALLEGRO_BITMAP* HUDBitmap;			// bitmap for the HUD panel
	int HUDStartX;						// framebuffer x to render the HUD
	int HUDStartY;						// framebuffer y to render the HUD
	ALLEGRO_BITMAP* lifeBitmap;			// HUD life bitmap
	int lifeBitmapWidth;				// life bitmap width
	int lifeBitmapHeight;				// life bitmap height
	int score;							// game scrore
	int lives;							// game lives
	int shield;							// game shield
	int displayScore;					// score we are going to draw
	int displayShield;					// shield we are going to draw
	int bufferWidth;					// framebuffer width
	int bufferHeight;					// framebuffer height

	static int getDigit(int n, int p);

public:

	const static int SHIELD_MAX = SHIELD_STATUS_WIDTH;
	const static int SCORE_MAX = 9999999;						// this should align with DIGITS_MAX
	const static int LIVES_MAX = 4;

	// HUD constructor
	HUD();
	// HUD destructor
	~HUD();
	// Updates all game HUD elements
	void update(int gameScore, int gameLives, int gameShield);
	// Draws the game HUD on the framebuffer
	void draw();
};
