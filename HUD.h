///////////////////////////////////////////////////////////////////////
//     _    _ _                 _    _ _
//    / \  | (_) ___ _ __      / \  | | | ___ _   _ 
//   / _ \ | | |/ _ \ '_ \    / _ \ | | |/ _ \ | | |
//  / ___ \| | |  __/ | | |  / ___ \| | |  __/ |_| |
// /_/   \_\_|_|\___|_| |_| /_/   \_\_|_|\___|\__, |
//                                            |___/
//
//  Sourceport / mod copyright � Samuel Gomes
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
	const static int DIGITS_MAX = 7;															// max digits on the score panel

	ALLEGRO_BITMAP* digitSpriteSheet;															// digits bitmap for drawing score
	CSize digitSpriteSize;																		// width & height of each digit
	ALLEGRO_BITMAP* HUDBitmap;																	// bitmap for the HUD panel
	ALLEGRO_BITMAP* lifeBitmap;																	// HUD life bitmap
	CSize lifeBitmapSize;																		// life bitmap width & height
	int displayScore;																			// score we are going to draw
	int displayShield;																			// shield we are going to draw
	CSize bufferSize;																			// framebuffer width & height

	static int getDigit(int n, int p);

public:

	const static int SHIELD_MAX = SHIELD_STATUS_WIDTH;
	const static int SCORE_MAX = 9999999;														// this should align with DIGITS_MAX
	const static int LIVES_MAX = 4;

	CPoint HUDStartPosition;																	// framebuffer x,y to render the HUD

	int score;																					// game scrore
	int lives;																					// game lives
	int shield;																					// game shield

	// HUD constructor
	HUD();
	// HUD destructor
	~HUD();
	// Updates all game HUD elements
	void update();
	// Draws the game HUD on the framebuffer
	void draw();
};
