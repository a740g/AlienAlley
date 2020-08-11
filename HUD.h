#pragma once

#include "AlienAlley.h"

class HUD
{
public:
	// HUD constructor
	HUD();
	// HUD destructor
	~HUD();
	// Updates all game HUD elements
	void update(int gameScore, int gameLives, int gameShield);
	// Draws the game HUD on the framebuffer
	void draw();

	const static int SHIELD_MAX = 82;
	const static int SCORE_MAX = 9999999;						// this should align with DIGITS_MAX
	const static int LIVES_MAX = 4;

private:
	const static int SHIELD_STATUS_HEIGHT = 22;
	const static int SHIELD_STATUS_LEFT = 191;
	const static int SHIELD_STATUS_TOP = 20;
	const static int SHIELD_STATUS_RIGHT = SHIELD_STATUS_LEFT + SHIELD_MAX - 1;
	const static int SHIELD_STATUS_BOTTOM = SHIELD_STATUS_TOP + SHIELD_STATUS_HEIGHT - 1;
	const static int SCORE_NUMBERS_LEFT = 474;
	const static int SCORE_NUMBERS_TOP = 24;
	const static int DIGITS_MAX = 7;																			// max digits on the score panel
	const int NUMBER_TABLE[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};	// we use to extract digit from numbers

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
};
