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

	al_hide_mouse_cursor(display);		// TODO: we need to move this to a good place later

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

// --- keyboard ---
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

bool shots_collide(bool is_ship, int x, int y, int w, int h)
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shots[i].used)
			continue;

		// don't collide with one's own shots
		if (shots[i].ship == is_ship)
			continue;

		int sw, sh;
		if (is_ship)
		{
			sw = sprites.alien_shot_d.w;
			sh = sprites.alien_shot_d.h;
		}
		else
		{
			sw = sprites.ship_shot_d.w;
			sh = sprites.ship_shot_d.h;
		}

		if (collide(x, y, x + w, y + h, shots[i].x, shots[i].y, shots[i].x + sw, shots[i].y + sh))
		{
			FX->add(FX->SPARKS, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
			shots[i].used = false;
			return true;
		}
	}

	return false;
}

// Checks for all possible collisions
void Game::checkCollisions()
{
	// Hero
	if (hero->invincibleTimer)
		hero->invincibleTimer--;
	else
	{
		if (shots_collide(true, ship.sprite.position.x, ship.sprite.position.y, ship.sprite.size.cx, ship.sprite.size.cy))
		{
			ship.shield -= 2;
			if (ship.shield <= 0 && ship.lives > 0)
			{
				int x = ship.sprite.position.x + (ship.sprite.size.cx / 2);
				int y = ship.sprite.position.y + (ship.sprite.size.cy / 2);
				FX->add(FX->EXPLOSION_SMALL, x, y);

				ship.lives--;
				ship.shield = HUD::SHIELD_MAX;

				ship.respawn_timer = 90;
				ship.invincible_timer = 180;
			}

		}
	}

	// Alien
	if (shots_collide(false, aliens[i].x, aliens[i].y, sprites.alien_d[aliens[i].type].w, sprites.alien_d[aliens[i].type].h))
	{
		aliens[i].life--;
		aliens[i].blink = 4;
	}

}


bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
	if (ax1 > bx2) return false;
	if (ax2 < bx1) return false;
	if (ay1 > by2) return false;
	if (ay2 < by1) return false;

	return true;
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
			hero->update(gameHUD->lives, key[ALLEGRO_KEY_LEFT], key[ALLEGRO_KEY_RIGHT], key[ALLEGRO_KEY_UP], key[ALLEGRO_KEY_DOWN], key[ALLEGRO_KEY_LCTRL] || key[ALLEGRO_KEY_SPACE] || key[ALLEGRO_KEY_RCTRL], *missiles);
			aliens->update();
			gameHUD->update();

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
			hero->draw();
			gameHUD->draw();

			al_flip_display();
			redraw = false;
		}
	}
}
