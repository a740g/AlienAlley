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
class Input;
class Aliens;
class CelestialObjects;
class Hero;
class MainMenu;

class Game
{
private:
	ALLEGRO_DISPLAY* display = nullptr;						// Allegro display
	ALLEGRO_AUDIO_STREAM* music;							// Streaming music
	ALLEGRO_TIMER* timer;									// Allegro timer
	ALLEGRO_EVENT_QUEUE* queue;								// Allegro queue
	ALLEGRO_FONT* font;										// for drawing onscreen / debugging message

	unsigned long frames;									// How many frame did we render?
	CSize screenSize;										// Screen width & Screen height - this is updated later

	// Game objects
	Input* input;
	MainMenu* mainMenu;
	CelestialObjects* celestialObjects;
	HUD* gameHUD;
	Effects* FX;
	Missiles* missiles;
	Hero* hero;
	Aliens* aliens;

	// Checks for all possible collisions
	void checkCollisions();

public:
	// String that can be used for onscreen messages or debugging
	static char message[USHRT_MAX];

	// Check if the operation succeeded and handle
	static void checkInitialized(bool test, const char* description);
	// Miscellaneous functions
	static int between(int lo, int hi);
	static float between(float lo, float hi);

	Game();
	~Game();
	void drawMessage();
	void run();
};
