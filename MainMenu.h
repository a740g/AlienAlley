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

class MainMenu
{
private:

	// Screen properties
	ALLEGRO_DISPLAY* display;
	int bufferWidth;
	int bufferHeight;

	// Bitmap properties
	ALLEGRO_BITMAP* bitmap;
	int bitmapWidth;
	int bitmapHeight;

	void drawScaledBitmap();

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
	void drawTitleScreen();
	int userInput();
	void fadeIn();
	void fadeOut();
};