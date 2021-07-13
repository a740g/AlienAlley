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

// TODO: account for screen resolution changes, going to windowed mode etc.
class Input
{
public:
	const enum
	{
		MOUSE_MOVE_LEFT,
		MOUSE_MOVE_RIGHT,
		MOUSE_MOVE_UP,
		MOUSE_MOVE_DOWN,
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_COUNT
	};

	Input(ALLEGRO_EVENT_QUEUE* q);
	~Input();
	void update(ALLEGRO_EVENT* event);
	// Return true if a key or button is pressed on the keyboard, mouse or controller
	bool isKeyboardPressed(unsigned char kbKey);
	bool isMousePressed(unsigned char msKey);

private:
	CSize bufferSize;
	ALLEGRO_DISPLAY* display;

	// Key see and released flags
	const enum
	{
		KEY_SEEN = 1,
		KEY_RELEASED
	};
	// Key state array
	unsigned char key[ALLEGRO_KEY_MAX];
	bool mouse[MOUSE_BUTTON_COUNT];
	CSize mouseMovement;
};
