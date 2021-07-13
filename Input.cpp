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

#include "Input.h"

Input::Input(ALLEGRO_EVENT_QUEUE* q)
{
	// Reset the keyboard state array
	memset(key, 0, sizeof(key));
	// Reset the mouse state array
	memset(mouse, 0, sizeof(mouse));

	Game::checkInitialized(al_install_keyboard(), __FUNCTION__": failed to initialize Allegro keyboard");
	Game::checkInitialized(al_install_mouse(), __FUNCTION__": failed to initialize Allegro mouse");

	// Hide mouse cursor and then set cursor position
	display = al_get_current_display();
	bufferSize.SetSize(al_get_display_width(display), al_get_display_height(display));
	al_hide_mouse_cursor(display);
	mouseMovement.SetSize(0, 0);
	al_set_mouse_xy(display, bufferSize.cx / 2, bufferSize.cy / 2);

	al_register_event_source(q, al_get_keyboard_event_source());
	al_register_event_source(q, al_get_mouse_event_source());
}

Input::~Input()
{
	al_show_mouse_cursor(display);
	
	al_uninstall_mouse();
	al_uninstall_keyboard();
}

// Updates the keyboard state array
void Input::update(ALLEGRO_EVENT* event)
{
	switch (event->type)
	{
	case ALLEGRO_EVENT_TIMER:
		for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
			key[i] &= KEY_SEEN;
		break;

	case ALLEGRO_EVENT_KEY_DOWN:
		key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
		break;

	case ALLEGRO_EVENT_KEY_UP:
		key[event->keyboard.keycode] &= KEY_RELEASED;
		break;

	case ALLEGRO_EVENT_MOUSE_AXES:
		mouseMovement.cx += event->mouse.dx;
		mouseMovement.cy += event->mouse.dy;
		al_set_mouse_xy(display, bufferSize.cx / 2, bufferSize.cy / 2);
		break;

	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		if (event->mouse.button & 1)
			mouse[MOUSE_BUTTON_LEFT] = true;
		if (event->mouse.button & 2)
			mouse[MOUSE_BUTTON_RIGHT] = true;
		break;

	case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		if (event->mouse.button & 1)
			mouse[MOUSE_BUTTON_LEFT] = false;
		if (event->mouse.button & 2)
			mouse[MOUSE_BUTTON_RIGHT] = false;
		break;
	}

	if (mouseMovement.cx < 0)
	{
		mouse[MOUSE_MOVE_LEFT] = true;
		mouseMovement.cx /= 2;
	}
	else if (mouseMovement.cx > 0)
	{
		mouse[MOUSE_MOVE_RIGHT] = true;
		mouseMovement.cx /= 2;
	}
	else
	{
		mouse[MOUSE_MOVE_LEFT] = false;
		mouse[MOUSE_MOVE_RIGHT] = false;
	}

	if (mouseMovement.cy < 0)
	{
		mouse[MOUSE_MOVE_UP] = true;
		mouseMovement.cy /= 2;
	}
	else if (mouseMovement.cy > 0)
	{
		mouse[MOUSE_MOVE_DOWN] = true;
		mouseMovement.cy /= 2;
	}
	else
	{
		mouse[MOUSE_MOVE_UP] = false;
		mouse[MOUSE_MOVE_DOWN] = false;
	}
}

// Return true if a key or button is pressed on the keyboard, mouse or controller
bool Input::isKeyboardPressed(unsigned char kbKey)
{
	return key[kbKey];
}

bool Input::isMousePressed(unsigned char msKey)
{
	return mouse[msKey];
}
