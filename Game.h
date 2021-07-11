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
class Aliens;
class CelestialObjects;
class Hero;
class MainMenu;

class Game
{
private:
	// Key see and released flags
	const enum
	{
		KEY_SEEN = 1,
		KEY_RELEASED
	};

	ALLEGRO_DISPLAY* display = nullptr;						// Allegro display
	ALLEGRO_AUDIO_STREAM* music;							// Streaming music
	ALLEGRO_TIMER* timer;									// Allegro timer
	ALLEGRO_EVENT_QUEUE* queue;								// Allegro queue
	ALLEGRO_FONT* font;										// for drawing game over message

	unsigned char key[ALLEGRO_KEY_MAX];						// Key state array
	unsigned long frames;									// How many frame did we render?
	int screenWidth = 640;									// Screen width - this is updated later
	int screenHeight = 480;									// Screen height - this is updated later

	// Game objects
	MainMenu* mainMenu;
	CelestialObjects* celestialObjects;
	HUD* gameHUD;
	Effects* FX;
	Missiles* missiles;
	Hero* hero;
	Aliens* aliens;

	void updateKeyboard(ALLEGRO_EVENT* event);
	// Checks for all possible collisions
	void checkCollisions();

public:
#ifdef DEBUG
	static char message[UCHAR_MAX];							// onscreen message
#endif // DEBUG

	// Check if the operation succeeded and handle
	static void checkInitialized(bool test, const char* description);
	// Miscellaneous functions
	static int between(int lo, int hi);
	static float between(float lo, float hi);

	Game();
	~Game();
#ifdef DEBUG
	void drawMessage();
#endif // DEBUG
	void run();
};
