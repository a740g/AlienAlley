///////////////////////////////////////////////////////////////////////
//     _    _ _                 _    _ _
//    / \  | (_) ___ _ __      / \  | | | ___ _   _ 
//   / _ \ | | |/ _ \ '_ \    / _ \ | | |/ _ \ | | |
//  / ___ \| | |  __/ | | |  / ___ \| | |  __/ |_| |
// /_/   \_\_|_|\___|_| |_| /_/   \_\_|_|\___|\__, |
//                                            |___/
//
//  Conversion/port copyright � Samuel Gomes & Neil Gomes, 1998-2020.
//
///////////////////////////////////////////////////////////////////////

#include "Game.h"

Game::Game()
{
	// Seed the random number generator
	srand(time(0));

	Game::checkInitialized(al_init(), __FUNCTION__": failed to initialize Allegro");
	Game::checkInitialized(al_init_native_dialog_addon(), __FUNCTION__": failed to initialize Allegro native dialog");
	Game::checkInitialized(al_install_keyboard(), __FUNCTION__": failed to initialize Allegro keyboard");

	// Reset the keyboard state array
	memset(key, 0, sizeof(key));

	al_set_new_window_title("Alien Alley");
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_FRAMELESS);
	display = al_create_display(screenWidth, screenHeight);
	Game::checkInitialized(display, __FUNCTION__": failed to create Allegro display");
	// Capture the what screen width and height were actually set
	screenWidth = al_get_display_width(display);
	screenHeight = al_get_display_height(display);

	timer = al_create_timer(1.0 / max(max(al_get_display_refresh_rate(display), al_get_new_display_refresh_rate()), 60.0));
	Game::checkInitialized(timer, __FUNCTION__": failed to create Allegro timer");
	queue = al_create_event_queue();
	Game::checkInitialized(queue, __FUNCTION__": failed to create Allgro event queue");

	Game::checkInitialized(al_init_image_addon(), __FUNCTION__": failed to initialize Allegro image addon");
	Game::checkInitialized(al_init_primitives_addon(), __FUNCTION__": failed to initialize Allegro primitives addon");
	Game::checkInitialized(al_install_audio(), __FUNCTION__": failed to initialize Allegro audio");
	Game::checkInitialized(al_init_acodec_addon(), __FUNCTION__": failed to initialize Allegro audio codecs");
	Game::checkInitialized(al_reserve_samples(256), __FUNCTION__": failed to reserve Allegro samples");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	// Streaming music playback. TODO: This should be moved out of here
	music = al_load_audio_stream("dat/snd/mus/alien_main.opus", 2, 2048);
	Game::checkInitialized(music, "dat/snd/mus/alien_main.opus");
	al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(music, al_get_default_mixer());

	/* load high-score file */
	//TODO: HighScoresLoad()

	al_hide_mouse_cursor(display);		// TODO: we need to move this to a good place later?

	mainMenu = new MainMenu();							// Initialize main menu
	celestialObjects = new CelestialObjects();			// Initizlize celestial objects
	gameHUD = new HUD();								// Initialize game HUD
	FX = new Effects();									// Initialize effects
	missiles = new Missiles();
	aliens = new Aliens();
	hero = new Hero();
}

Game::~Game()
{
	delete hero;
	delete aliens;
	delete missiles;
	delete FX;
	delete gameHUD;
	delete celestialObjects;
	delete mainMenu;

	al_show_mouse_cursor(display);

	// Save high scores
	//TODO: HighScoresSave();

	al_destroy_audio_stream(music);		// TODO: This should move from here?

	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_shutdown_native_dialog_addon();

	al_destroy_display(display);
}

// Check if a Allegro/program initialization succeeded
// If not, it just ends with EXIT_FAILURE
void Game::checkInitialized(bool test, const char* description)
{
	if (test) return;

	al_show_native_message_box(al_get_current_display(), "Error", "Failed to initialize:", description, nullptr, ALLEGRO_MESSAGEBOX_ERROR);

	exit(EXIT_FAILURE);
}

int Game::between(int lo, int hi)
{
	return lo + (rand() % (hi - lo));
}

float Game::between(float lo, float hi)
{
	return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

// Updates the keyboard state array
void Game::updateKeyboard(ALLEGRO_EVENT* event)
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
	}
}

// Checks for all possible collisions
void Game::checkCollisions()
{
	// Check if hero collided with missiles
	for (int i = 0; i < missiles->SHOTS_N; i++)
	{
		if (!missiles->shot[i].used)
			continue;

		// Don't collide with one's own shots
		if (missiles->shot[i].type == missiles->HERO)
			continue;

		if (hero->sprite->collidesWith(*missiles->shot[i].sprite))
		{
			missiles->hit(i, *FX);
			hero->hit(false, *gameHUD, *FX);
		}
	}

	// Check if hero collided with aliens
	for (int i = 0; i < aliens->ALIENS_N; i++)
	{

	}

	// Check if aliens collided with missiles
	for (int m = 0; m < missiles->SHOTS_N; m++)
	{
		for (int a = 0; a < aliens->ALIENS_N; a++)
		{

		}
	}
}

// This has the main game loop
void Game::run()
{
	frames = 0;

	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;

	mainMenu->drawTitleScreen();
	mainMenu->fadeOut();

	al_start_timer(timer);

	while (1)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			FX->update();
			missiles->update();
			celestialObjects->update(key[ALLEGRO_KEY_UP]);		// TODO: this needs to the input agnostic
			hero->update(key[ALLEGRO_KEY_LEFT], key[ALLEGRO_KEY_RIGHT], key[ALLEGRO_KEY_UP], key[ALLEGRO_KEY_DOWN], key[ALLEGRO_KEY_LCTRL] || key[ALLEGRO_KEY_SPACE] || key[ALLEGRO_KEY_RCTRL], *gameHUD, *missiles);
			aliens->update(frames, *gameHUD, *missiles, *FX);
			gameHUD->update();
			checkCollisions();

			if (key[ALLEGRO_KEY_ESCAPE])
				done = true;

			redraw = true;
			frames++;
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}

		if (done)
			break;

		updateKeyboard(&event);

		if (redraw && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));

			celestialObjects->draw();
			aliens->draw();
			missiles->draw();
			FX->draw();
			hero->draw(*gameHUD);
			gameHUD->draw();

			al_flip_display();
			redraw = false;
		}
	}
}
