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

#include "AlienAlley.h"

constexpr auto KEY_SEEN = 1;							// Key see and released flags
constexpr auto KEY_RELEASED = 2;
constexpr auto SHOTS_N = 128;
constexpr auto SHIP_SPEED = 3;
constexpr auto ALIENS_N = 8;

// Types of aliens
enum ALIEN_TYPE_T
{
	ALIEN_TYPE_BUG = 0,
	ALIEN_TYPE_ARROW,
	//ALIEN_TYPE_BOP,
	ALIEN_TYPE_THICCBOI,
	ALIEN_TYPE_N
};

// Rectange dimension
struct DIM_T
{
	int w;
	int h;
};

struct SPRITES_T
{
	ALLEGRO_BITMAP* ship;
	DIM_T ship_d;

	ALLEGRO_BITMAP* ship_shot[2];
	DIM_T ship_shot_d;

	ALLEGRO_BITMAP* alien[ALIEN_TYPE_N];
	DIM_T alien_d[ALIEN_TYPE_N];

	ALLEGRO_BITMAP* alien_shot;
	DIM_T alien_shot_d;

	ALLEGRO_BITMAP* explosion;
	ALLEGRO_BITMAP* sparks;
	ALLEGRO_BITMAP* powerup;
};

struct SHOT_T
{
	int x, y, dx, dy;
	int frame;
	bool ship;
	bool used;
};

struct SHIP_T
{
	int x, y;
	int ship_max_x, ship_max_y;
	int shot_timer;
	int lives;
	int shield;
	int respawn_timer;
	int invincible_timer;
};

struct ALIEN_T
{
	int x, y;
	ALIEN_TYPE_T type;
	int shot_timer;
	int blink;
	int life;
	bool used;
};

unsigned char key[ALLEGRO_KEY_MAX];						// Key state array
ALLEGRO_DISPLAY* disp = nullptr;						// Allegro display
int screen_width = 640;									// Screen width - this is updated later
int screen_height = 480;								// Screen height - this is updated later
long frames;											// How many frame did we render?
long score;												// What is the player score?
ALLEGRO_SAMPLE* sample_shot;
ALLEGRO_SAMPLE* sample_explode[2];
ALLEGRO_AUDIO_STREAM* music;
SPRITES_T sprites;
SHOT_T shots[SHOTS_N];
SHIP_T ship;
ALIEN_T aliens[ALIENS_N];


// Check if a Allegro/pogram initialization succeeded
// If not, it just ends with EXIT_FAILURE
void InitializeCheck(bool test, const char* description)
{
	if (test) return;

	al_show_native_message_box(disp, "Error", "Failed to initialize:", description, nullptr, ALLEGRO_MESSAGEBOX_ERROR);

	exit(EXIT_FAILURE);
}

// --- general ---

int between(int lo, int hi)
{
	return lo + (rand() % (hi - lo));
}

float between(float lo, float hi)
{
	return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
	if (ax1 > bx2) return false;
	if (ax2 < bx1) return false;
	if (ay1 > by2) return false;
	if (ay2 < by1) return false;

	return true;
}

// --- display ---

void disp_init()
{
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_FRAMELESS);
	disp = al_create_display(screen_width, screen_height);
	InitializeCheck(disp, "display");
	screen_width = al_get_display_width(disp);
	screen_height = al_get_display_height(disp);
}

void disp_deinit()
{
	al_destroy_display(disp);
}

// --- keyboard ---

void keyboard_init()
{
	memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
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

// --- sprites ---

void sprites_init()
{
	sprites.ship = al_load_bitmap("dat/gfx/hero.png");
	InitializeCheck(sprites.ship, "dat/gfx/hero.png");
	sprites.ship_d.w = al_get_bitmap_width(sprites.ship);
	sprites.ship_d.h = al_get_bitmap_height(sprites.ship);

	sprites.ship_shot[0] = al_load_bitmap("dat/gfx/hero_shot0.png");
	InitializeCheck(sprites.ship_shot[0], "dat/gfx/hero_shot0.png");
	sprites.ship_shot[1] = al_load_bitmap("dat/gfx/hero_shot1.png");
	InitializeCheck(sprites.ship_shot[1], "dat/gfx/hero_shot1.png");
	sprites.ship_shot_d.w = al_get_bitmap_width(sprites.ship_shot[0]);
	sprites.ship_shot_d.h = al_get_bitmap_height(sprites.ship_shot[1]);

	sprites.alien[0] = al_load_bitmap("dat/gfx/alien_bug.png");
	InitializeCheck(sprites.alien[0], "dat/gfx/alien_bug.png");
	sprites.alien_d[0].w = al_get_bitmap_width(sprites.alien[0]);
	sprites.alien_d[0].h = al_get_bitmap_height(sprites.alien[0]);

	sprites.alien[1] = al_load_bitmap("dat/gfx/alien_arrow.png");
	InitializeCheck(sprites.alien[1], "dat/gfx/alien_arrow.png");
	sprites.alien_d[1].w = al_get_bitmap_width(sprites.alien[1]);
	sprites.alien_d[1].h = al_get_bitmap_height(sprites.alien[1]);

	sprites.alien[2] = al_load_bitmap("dat/gfx/alien_thiccboi.png");
	InitializeCheck(sprites.alien[2], "dat/gfx/alien_thiccboi.png");
	sprites.alien_d[2].w = al_get_bitmap_width(sprites.alien[2]);
	sprites.alien_d[2].h = al_get_bitmap_height(sprites.alien[2]);

	sprites.alien_shot = al_load_bitmap("dat/gfx/alien_shot.png");
	InitializeCheck(sprites.alien_shot, "dat/gfx/alien_shot.png");
	sprites.alien_shot_d.w = al_get_bitmap_width(sprites.alien_shot);
	sprites.alien_shot_d.h = al_get_bitmap_height(sprites.alien_shot);

	sprites.explosion = al_load_bitmap("dat/gfx/explosion_small_ss.png");
	InitializeCheck(sprites.explosion, "dat/gfx/explosion_small_ss.png");

	sprites.sparks = al_load_bitmap("dat/gfx/sparks_ss.png");
	InitializeCheck(sprites.sparks, "dat/gfx/sparks_ss.png");

	sprites.powerup = al_load_bitmap("dat/gfx/powerup_ss.png");
	InitializeCheck(sprites.powerup, "dat/gfx/powerup_ss.png");
}

void sprites_deinit()
{
	al_destroy_bitmap(sprites.ship);

	al_destroy_bitmap(sprites.ship_shot[0]);
	al_destroy_bitmap(sprites.ship_shot[1]);

	al_destroy_bitmap(sprites.alien[0]);
	al_destroy_bitmap(sprites.alien[1]);
	al_destroy_bitmap(sprites.alien[2]);

	al_destroy_bitmap(sprites.sparks);

	al_destroy_bitmap(sprites.explosion);

	al_destroy_bitmap(sprites.powerup);
}

// --- audio ---

/*
	Function: AudioInitialize
	Description:
		Loads audio effects & music
*/
void AudioInitialize()
{
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(128);

	sample_shot = al_load_sample("dat/snd/sfx/alien_shot.flac");
	InitializeCheck(sample_shot, "dat/snd/sfx/alien_shot.flac");

	sample_explode[0] = al_load_sample("dat/snd/sfx/alien_explosion_small.flac");
	InitializeCheck(sample_explode[0], "dat/snd/sfx/alien_explosion_small.flac");
	sample_explode[1] = al_load_sample("dat/snd/sfx/alien_explosion_big.flac");
	InitializeCheck(sample_explode[1], "dat/snd/sfx/alien_explosion_big.flac");

	// Streaming music playback
	music = al_load_audio_stream("dat/snd/mus/alien_main.opus", 2, 2048);
	InitializeCheck(music, "dat/snd/mus/alien_main.opus");
	al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
}

/*
	Function: AudioFinalize
	Description:
		Unloads audio effects & music
*/
void AudioFinalize()
{
	al_destroy_sample(sample_shot);
	al_destroy_sample(sample_explode[0]);
	al_destroy_sample(sample_explode[1]);
	al_destroy_audio_stream(music);
}

// --- shots ---

void shots_init()
{
	for (int i = 0; i < SHOTS_N; i++)
		shots[i].used = false;
}

bool shots_add(bool is_ship, bool straight, int x, int y)
{
	al_play_sample(
		sample_shot,
		0.3,
		0,
		is_ship ? 1.0 : between(1.5f, 1.6f),
		ALLEGRO_PLAYMODE_ONCE,
		nullptr
	);

	for (int i = 0; i < SHOTS_N; i++)
	{
		if (shots[i].used)
			continue;

		shots[i].ship = is_ship;

		if (is_ship)
		{
			shots[i].x = x - (sprites.ship_shot_d.w / 2);
			shots[i].y = y;
		}
		else // alien
		{
			shots[i].x = x - (sprites.alien_shot_d.w / 2);
			shots[i].y = y - (sprites.alien_shot_d.h / 2);

			if (straight)
			{
				shots[i].dx = 0;
				shots[i].dy = 2;
			}
			else
			{

				shots[i].dx = between(-2, 2);
				shots[i].dy = between(-2, 2);
			}

			// if the shot has no speed, don't bother
			if (!shots[i].dx && !shots[i].dy)
				return true;

			shots[i].frame = 0;
		}

		shots[i].frame = 0;
		shots[i].used = true;

		return true;
	}
	return false;
}

void shots_update()
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shots[i].used)
			continue;

		if (shots[i].ship)
		{
			shots[i].y -= 5;

			if (shots[i].y < -(sprites.ship_shot_d.h))
			{
				shots[i].used = false;
				continue;
			}
		}
		else // alien
		{
			shots[i].x += shots[i].dx;
			shots[i].y += shots[i].dy;

			if ((shots[i].x < -(sprites.alien_shot_d.w))
				|| (shots[i].x > screen_width)
				|| (shots[i].y < -(sprites.alien_shot_d.h))
				|| (shots[i].y > screen_height)
				) {
				shots[i].used = false;
				continue;
			}
		}

		shots[i].frame++;
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
			fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
			shots[i].used = false;
			return true;
		}
	}

	return false;
}

void shots_draw()
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shots[i].used)
			continue;

		int frame_display = (shots[i].frame / 2) % 2;

		if (shots[i].ship)
			al_draw_bitmap(sprites.ship_shot[frame_display], shots[i].x, shots[i].y, 0);
		else // alien
		{
			ALLEGRO_COLOR tint =
				frame_display
				? al_map_rgb_f(1, 1, 1)
				: al_map_rgb_f(0.5, 0.5, 0.5)
				;
			al_draw_tinted_bitmap(sprites.alien_shot, tint, shots[i].x, shots[i].y, 0);
		}
	}
}

// --- ship ---

void ship_init()
{
	ship.x = (screen_width / 2) - (sprites.ship_d.w / 2);
	ship.y = (screen_height / 2) - (sprites.ship_d.h / 2);
	ship.ship_max_x = screen_width - sprites.ship_d.w;
	ship.ship_max_y = screen_height - sprites.ship_d.h;
	ship.shot_timer = 0;
	ship.lives = HUD::LIVES_MAX;
	ship.shield = HUD::SHIELD_MAX;
	ship.respawn_timer = 0;
	ship.invincible_timer = 120;
}

void ship_update()
{
	if (ship.lives <= 0)
		return;

	if (ship.respawn_timer)
	{
		ship.respawn_timer--;
		return;
	}

	if (key[ALLEGRO_KEY_LEFT])
		ship.x -= SHIP_SPEED;
	if (key[ALLEGRO_KEY_RIGHT])
		ship.x += SHIP_SPEED;
	if (key[ALLEGRO_KEY_UP])
		ship.y -= SHIP_SPEED;
	if (key[ALLEGRO_KEY_DOWN])
		ship.y += SHIP_SPEED;

	if (ship.x < 0)
		ship.x = 0;
	if (ship.y < 0)
		ship.y = 0;

	if (ship.x > ship.ship_max_x) ship.x = ship.ship_max_x;
	if (ship.y > ship.ship_max_y) ship.y = ship.ship_max_y;

	if (ship.invincible_timer)
		ship.invincible_timer--;
	else
	{
		if (shots_collide(true, ship.x, ship.y, sprites.ship_d.w, sprites.ship_d.h))
		{
			ship.shield -= 2;
			if (ship.shield <= 0 && ship.lives > 0)
			{
				int x = ship.x + (sprites.ship_d.w / 2);
				int y = ship.y + (sprites.ship_d.h / 2);
				fx_add(false, x, y);
				fx_add(false, x + 4, y + 2);
				fx_add(false, x - 2, y - 4);
				fx_add(false, x + 1, y - 5);

				ship.lives--;
				ship.shield = HUD::SHIELD_MAX;

				ship.respawn_timer = 90;
				ship.invincible_timer = 180;
			}

		}
	}

	if (ship.shot_timer)
		ship.shot_timer--;
	else if (key[ALLEGRO_KEY_LCTRL] || key[ALLEGRO_KEY_SPACE] || key[ALLEGRO_KEY_RCTRL])
	{
		int x = ship.x + (sprites.ship_d.w / 2);
		if (shots_add(true, false, x, ship.y))
			ship.shot_timer = 5;
	}
}

void ship_draw()
{
	if (ship.lives <= 0)
		return;
	if (ship.respawn_timer)
		return;
	if (((ship.invincible_timer / 2) % 3) == 1)
		return;

	al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}

// --- aliens ---

void aliens_init()
{
	for (int i = 0; i < ALIENS_N; i++)
		aliens[i].used = false;
}

void aliens_update()
{
	int new_quota =
		(frames % 120)
		? 0
		: between(2, 4)
		;
	int new_x = between(10, screen_width - 50);

	for (int i = 0; i < ALIENS_N; i++)
	{
		if (!aliens[i].used)
		{
			// if this alien is unused, should it spawn?
			if (new_quota > 0)
			{
				new_x += between(40, 80);
				if (new_x > (screen_width - 60))
					new_x -= (screen_width - 60);

				aliens[i].x = new_x;

				aliens[i].y = between(-40, -30);
				aliens[i].type = (ALIEN_TYPE_T)between(0, ALIEN_TYPE_N);
				aliens[i].shot_timer = between(1, 99);
				aliens[i].blink = 0;
				aliens[i].used = true;

				switch (aliens[i].type)
				{
				case ALIEN_TYPE_BUG:
					aliens[i].life = 4;
					break;
				case ALIEN_TYPE_ARROW:
					aliens[i].life = 2;
					break;
				case ALIEN_TYPE_THICCBOI:
					aliens[i].life = 12;
					break;
				}

				new_quota--;
			}
			continue;
		}

		switch (aliens[i].type)
		{
		case ALIEN_TYPE_BUG:
			if (frames % 2)
				aliens[i].y++;
			break;

		case ALIEN_TYPE_ARROW:
			aliens[i].y++;
			break;

		case ALIEN_TYPE_THICCBOI:
			if (!(frames % 4))
				aliens[i].y++;
			break;
		}

		if (aliens[i].y >= screen_width)
		{
			aliens[i].used = false;
			continue;
		}

		if (aliens[i].blink)
			aliens[i].blink--;

		if (shots_collide(false, aliens[i].x, aliens[i].y, sprites.alien_d[aliens[i].type].w, sprites.alien_d[aliens[i].type].h))
		{
			aliens[i].life--;
			aliens[i].blink = 4;
		}

		int cx = aliens[i].x + (sprites.alien_d[aliens[i].type].w / 2);
		int cy = aliens[i].y + (sprites.alien_d[aliens[i].type].h / 2);

		if (aliens[i].life <= 0)
		{
			fx_add(false, cx, cy);

			switch (aliens[i].type)
			{
			case ALIEN_TYPE_BUG:
				score += 20;
				break;

			case ALIEN_TYPE_ARROW:
				score += 15;
				break;

			case ALIEN_TYPE_THICCBOI:
				score += 80;
				fx_add(false, cx - 10, cy - 4);
				fx_add(false, cx + 4, cy + 10);
				fx_add(false, cx + 8, cy + 8);
				break;
			}

			aliens[i].used = false;
			continue;
		}

		aliens[i].shot_timer--;
		if (aliens[i].shot_timer == 0)
		{
			switch (aliens[i].type)
			{
			case ALIEN_TYPE_BUG:
				shots_add(false, false, cx, cy);
				aliens[i].shot_timer = 150;
				break;
			case ALIEN_TYPE_ARROW:
				shots_add(false, true, cx, aliens[i].y);
				aliens[i].shot_timer = 80;
				break;
			case ALIEN_TYPE_THICCBOI:
				shots_add(false, true, cx - 5, cy);
				shots_add(false, true, cx + 5, cy);
				shots_add(false, true, cx - 5, cy + 8);
				shots_add(false, true, cx + 5, cy + 8);
				aliens[i].shot_timer = 200;
				break;
			}
		}
	}
}

void aliens_draw()
{
	for (int i = 0; i < ALIENS_N; i++)
	{
		if (!aliens[i].used)
			continue;
		if (aliens[i].blink > 2)
			continue;

		al_draw_bitmap(sprites.alien[aliens[i].type], aliens[i].x, aliens[i].y, 0);
	}
}

/*
	Function: main
	Description:
		Main program loop. Initializes the program, draws intro screens
		and title pages, and waits for user to hit keystroketo indicated
		what they want to do. Performs all the program-wide initialization
		at start-up and frees and releases resource before exiting.
*/
int main()
{
	InitializeCheck(al_init(), "allegro");
	
	al_set_new_window_title("Alien Alley");

	// Iitialzie native dialog support
	InitializeCheck(al_init_native_dialog_addon(), "native dialog");

	InitializeCheck(al_install_keyboard(), "keyboard");

	disp_init();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / max(max(al_get_display_refresh_rate(disp), al_get_new_display_refresh_rate()), 60.0));
	InitializeCheck(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	InitializeCheck(queue, "queue");

	/* load high-score file */
	//TODO: HighScoresLoad()

	AudioInitialize();

	InitializeCheck(al_init_image_addon(), "image");
	sprites_init();

	InitializeCheck(al_init_primitives_addon(), "primitives");

	InitializeCheck(al_install_audio(), "audio");
	InitializeCheck(al_init_acodec_addon(), "audio codecs");
	InitializeCheck(al_reserve_samples(16), "reserve samples");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	keyboard_init();
	al_hide_mouse_cursor(disp);		// TODO: we need to move this to a good place later
	fx_init();
	shots_init();
	ship_init();
	aliens_init();

	
	MainMenu* mainMenu = new MainMenu();							// Initialize main menu
	CelestialObjects* celestialObjects = new CelestialObjects();	// Initizlize celestial objects
	HUD* gameHUD = new HUD();										// Initialize game HUD

	frames = 0;
	score = 0;

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
			fx_update();
			shots_update();
			celestialObjects->update(key[ALLEGRO_KEY_UP]);		// TODO: this needs to the input agnostic
			ship_update();
			aliens_update();
			gameHUD->update(score, ship.lives, ship.shield);

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

		keyboard_update(&event);

		if (redraw && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));

			celestialObjects->draw();
			aliens_draw();
			shots_draw();
			fx_draw();
			ship_draw();

			gameHUD->draw();

			al_flip_display();
			redraw = false;
		}
	}

	// Save high scores
	//TODO: HighScoresSave();

	delete gameHUD;
	delete celestialObjects;
	delete mainMenu;
	sprites_deinit();
	al_show_mouse_cursor(disp);
	AudioFinalize();
	disp_deinit();
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_shutdown_native_dialog_addon();

	return EXIT_SUCCESS;
}