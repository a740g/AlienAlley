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

class MainMenu
{
private:

	// Screen properties
	ALLEGRO_DISPLAY* display;
	CSize bufferSize;

public:

	// Type of objects
	const enum
	{
		START_GAME,
		SHOW_HIGHSCORE,
		EXIT_GAME,
	};

	MainMenu();
	void drawTitleScreen();
	int userInput();
	void fadeIn();
	void fadeOut();
};