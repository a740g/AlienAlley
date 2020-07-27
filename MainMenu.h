#pragma once

#include "AlienAlley.h"

class MainMenu
{
public:

	// Type of objects
	enum UserInput
	{
		START_GAME,
		SHOW_HIGHSCORE,
		EXIT_GAME,
	};

	MainMenu();
	~MainMenu();
	void drawIntroCreditsScreen();
	void drawTitleScreen();
	int userInput();
	void fadeIn();
	void fadeOut();

private:

	// Screen properties
	ALLEGRO_DISPLAY* display = NULL;
	int bufferWidth = 0;
	int bufferHeight = 0;

	// Bitmap properties
	ALLEGRO_BITMAP* bitmap = NULL;
	int bitmapWidth = 0;
	int bitmapHeight = 0;

	void drawScaledBitmap();
};