///////////////////////////////////////////////////////////////////////
//     _    _ _                 _    _ _
//    / \  | (_) ___ _ __      / \  | | | ___ _   _ 
//   / _ \ | | |/ _ \ '_ \    / _ \ | | |/ _ \ | | |
//  / ___ \| | |  __/ | | |  / ___ \| | |  __/ |_| |
// /_/   \_\_|_|\___|_| |_| /_/   \_\_|_|\___|\__, |
//                                            |___/
//
//  Conversion/port copyright (c) Samuel Gomes & Neil Gomes, 1998-2020.
//  All rights reserved.
//
///////////////////////////////////////////////////////////////////////

#include "AlienAlley.h"

unsigned char key[ALLEGRO_KEY_MAX];						// Key state array
ALLEGRO_DISPLAY* disp = NULL;							// Allegro display
int screen_width = 640;									// Screen width - this is updated later
int screen_height = 480;								// Screen height - this is updated later
long frames;											// How many frame did we render?
long score;												// What is the player score?
ALLEGRO_SAMPLE* sample_shot;
ALLEGRO_SAMPLE* sample_explode[2];
ALLEGRO_AUDIO_STREAM* music;
ALLEGRO_FONT* font;
long score_display;
SPRITES_T sprites;
FX_T fx[FX_N];
SHOT_T shots[SHOTS_N];
SHIP_T ship;
ALIEN_T aliens[ALIENS_N];


// Check if a Allegro/pogram initialization succeeded
// If not, it just ends with EXIT_FAILURE
void InitializeCheck(bool test, const char* description)
{
	if (test) return;

	al_show_native_message_box(disp, "Error", "Failed to initialize:", description, NULL, ALLEGRO_MESSAGEBOX_ERROR);

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


/*
	Function: BoundingRectCompute
	Description:
		Calculates the bounding rectangle for a sprite given its
		position, width and height.
*/
void BoundingRectCompute(const POINT_T* p, const DIM_T* d, RECT_T* r)
{
	r->a.x = p->x;
	r->a.y = p->y;
	r->b.x = r->a.x + d->w - 1;
	r->b.y = r->a.y + d->h - 1;
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
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
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

	sprites.life = al_load_bitmap("dat/gfx/life.png");
	InitializeCheck(sprites.life, "dat/gfx/life.png");
	sprites.life_d.w = al_get_bitmap_width(sprites.life);
	sprites.life_d.h = al_get_bitmap_height(sprites.life);

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

	music = al_load_audio_stream("dat/snd/mus/alien_main.opus", 2, 2048);
	InitializeCheck(music, "music");
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

// --- fx ---

void fx_init()
{
	for (int i = 0; i < FX_N; i++)
		fx[i].used = false;
}

void fx_add(bool spark, int x, int y)
{
	if (!spark)
		al_play_sample(sample_explode[between(0, 2)], 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

	for (int i = 0; i < FX_N; i++)
	{
		if (fx[i].used)
			continue;

		fx[i].x = x;
		fx[i].y = y;
		fx[i].frame = 0;
		fx[i].spark = spark;
		fx[i].used = true;
		return;
	}
}

void fx_update()
{
	for (int i = 0; i < FX_N; i++)
	{
		if (!fx[i].used)
			continue;

		fx[i].frame++;

		if ((!fx[i].spark && (fx[i].frame == (EXPLOSION_FRAMES * 2)))
			|| (fx[i].spark && (fx[i].frame == (SPARKS_FRAMES * 2)))
			)
			fx[i].used = false;
	}
}

void fx_draw()
{
	for (int i = 0; i < FX_N; i++)
	{
		if (!fx[i].used)
			continue;

		int frame_display = fx[i].frame / 2;
		ALLEGRO_BITMAP* bmp = fx[i].spark ? sprites.sparks : sprites.explosion;

		int frame_side_size = al_get_bitmap_height(bmp);	// since each fx frame width & height are the same
		int x = fx[i].x - (frame_side_size / 2);
		int y = fx[i].y - (frame_side_size / 2);
		al_draw_bitmap_region(bmp, frame_display * frame_side_size, 0, frame_side_size, frame_side_size, x, y, 0);
	}
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
		NULL
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
	ship.lives = 3;
	ship.respawn_timer = 0;
	ship.invincible_timer = 120;
}

void ship_update()
{
	if (ship.lives < 0)
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
			int x = ship.x + (sprites.ship_d.w / 2);
			int y = ship.y + (sprites.ship_d.h / 2);
			fx_add(false, x, y);
			fx_add(false, x + 4, y + 2);
			fx_add(false, x - 2, y - 4);
			fx_add(false, x + 1, y - 5);

			ship.lives--;
			ship.respawn_timer = 90;
			ship.invincible_timer = 180;
		}
	}

	if (ship.shot_timer)
		ship.shot_timer--;
	else if (key[ALLEGRO_KEY_X])
	{
		int x = ship.x + (sprites.ship_d.w / 2);
		if (shots_add(true, false, x, ship.y))
			ship.shot_timer = 5;
	}
}

void ship_draw()
{
	if (ship.lives < 0)
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
				score += 200;
				break;

			case ALIEN_TYPE_ARROW:
				score += 150;
				break;

			case ALIEN_TYPE_THICCBOI:
				score += 800;
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

// --- hud ---

void hud_init()
{
	font = al_create_builtin_font();
	InitializeCheck(font, "font");

	score_display = 0;
}

void hud_deinit()
{
	al_destroy_font(font);
}

void hud_update()
{
	if (frames % 2)
		return;

	for (long i = 5; i > 0; i--)
	{
		long diff = 1 << i;
		if (score_display <= (score - diff))
			score_display += diff;
	}
}

void hud_draw()
{
	al_draw_textf(
		font,
		al_map_rgb_f(1, 1, 1),
		1, 1,
		0,
		"%06ld",
		score_display
	);

	int spacing = sprites.life_d.w + 1;
	for (int i = 0; i < ship.lives; i++)
		al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);

	if (ship.lives < 0)
		al_draw_text(
			font,
			al_map_rgb_f(1, 1, 1),
			screen_width / 2, screen_height / 2,
			ALLEGRO_ALIGN_CENTER,
			"G A M E  O V E R"
		);
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

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	InitializeCheck(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	InitializeCheck(queue, "queue");

	disp_init();

	/* load high-score file */
	//TODO: HighScoresLoad()

	AudioInitialize();

	InitializeCheck(al_init_image_addon(), "image");
	sprites_init();

	hud_init();

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

	// Initizlize celestial objects
	CelestialObjects celestialObjects(screen_width, screen_height);

	frames = 0;
	score = 0;

	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;

	al_start_timer(timer);

	while (1)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			fx_update();
			shots_update();
			celestialObjects.update();
			ship_update();
			aliens_update();
			hud_update();

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

			celestialObjects.draw();
			aliens_draw();
			shots_draw();
			fx_draw();
			ship_draw();

			hud_draw();

			al_flip_display();
			redraw = false;
		}
	}

	// Save high scores
	//TODO: HighScoresSave();

	sprites_deinit();
	hud_deinit();
	al_show_mouse_cursor(disp);
	AudioFinalize();
	disp_deinit();
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_shutdown_native_dialog_addon();

	return EXIT_SUCCESS;
}

#if 0
/*

Type SPRITE_T
Active As Boolean
p As POINT_T
vx As Integer				' x velocity
vy As Integer				' y velocity
bDraw As Boolean			' was draw
ObjectSpec1 As Integer
ObjectSpec2 As Integer
Bounds As Rect_t
End Type

Type HIGH_SCORE_T
Text As String * HIGH_SCORE_TEXT_LEN
Score As Integer
End Type


/* GLOBAL VARIABLES */
Dim Shared GamePalette(0 To PAL_SIZE - 1) As Palette
Dim Shared HeroShields As Integer = MAX_HERO_SHIELDS
Dim Shared HighScore(0 To 9) As HIGH_SCORE_T
Dim Shared ExplosionSound As SAMPLE Pointer
Dim Shared LaserSound As SAMPLE Pointer
Dim Shared GameMusic As MIDI Pointer
Dim Shared MapLine(0 To(SCREEN_WIDTH / TILE_WIDTH) - 1) As Integer		' each tile is 32x32. so 640 / 32 = 20
Dim Shared Tile(0 To NUM_TILES - 1) As BITMAP Pointer
Dim Shared MapBuffer As BITMAP Pointer									' this is the pre-rendered background bitmap
Dim Shared MapScrollStep As Integer = MAP_SCROLL_STEP_NORMAL			' # of pixels to scroll the background
Dim Shared MapLineCounter As Integer = -TILE_HEIGHT						' this keeps track of the new tiles positions
Dim Shared FrameBuffer As BITMAP Pointer
Dim Shared HUDBitmap As BITMAP Pointer
Dim Shared HUDNumberBitmap(0 To 9) As BITMAP Pointer
Dim Shared Hero As SPRITE_T
Dim Shared Alien(0 To MAX_ALIENS - 1) As SPRITE_T
Dim Shared HeroMissile(0 To MAX_HERO_MISSILES - 1) As SPRITE_T
Dim Shared AlienMissile(0 To MAX_ALIEN_MISSILES - 1) As SPRITE_T
Dim Shared Explosion(0 To MAX_EXPLOSIONS - 1) As SPRITE_T
Dim Shared HeroBitmap As BITMAP Pointer
Dim Shared AlienBitmap As BITMAP Pointer
Dim Shared MissileBitmap As BITMAP Pointer
Dim Shared MissileTrailUpBitmap As BITMAP Pointer
Dim Shared MissileTrailDnBitmap As BITMAP Pointer
Dim Shared ExplosionBitmap(0 To MAX_EXPLOSION_BITMAPS - 1) As BITMAP Pointer
Dim Shared As UInteger HeroWidth, HeroHeight
Dim Shared As UInteger HeroXMin, HeroXMax
Dim Shared As UInteger HeroYMin, HeroYMax
Dim Shared As UInteger AlienWidth, AlienHeight
Dim Shared As UInteger MissileWidth, MissileHeight
Dim Shared As UInteger MissileTrailWidth, MissileTrailHeight
Dim Shared As UInteger ExplosionWidth, ExplosionHeight
Dim Shared AlienGenCounter As UInteger
Dim Shared GunBlinkCounter As UInteger
Dim Shared GunBlinkState As Boolean
Dim Shared GunColor As Palette
Dim Shared HeroAllowFire As Boolean
Dim Shared Ticks As UInteger											' Timer ticks to control game speed


' Our timer interrupt handler
' Simply updates the global Ticks variable
Sub Ticker Cdecl()
Ticks += 1
End Sub

' Plays an sfx keeping in mind the source of the sound
Sub SfxPlay(ByVal s As SAMPLE Pointer, ByVal x As Integer, ByVal y As Integer, ByVal b As BITMAP Pointer)
' Find the midpoint absolute location of the object and scale it down to the Allegro limits
Dim p As Integer = ((x + b->w / 2) / (SCREEN_WIDTH - 1)) * 255
play_sample(s, 255, p, 1000 + IIf(Int(Timer) Mod 2, Rnd * p, Rnd * -p), FALSE)
End Sub





' Collision testing routine (works only for 8-bit bitmaps at the moment!)
Function CollisionTest(ByVal bmp1 As BITMAP Pointer, bmp1pt As POINT_T, ByVal bmp2 As BITMAP Pointer, bmp2pt As POINT_T) As Boolean
Dim As RECT_T r1, r2, r
Dim As Integer x1, y1, x2, y2, x1s, x2s, w, h, i, j

' Create 'bmp1' bounding rectangle
BoundingRectCompute(bmp1pt.x, bmp1pt.y, bmp1->w, bmp1->h, r1)

' Create 'bmp2' bounding rectangle
BoundingRectCompute(bmp2pt.x, bmp2pt.y, bmp2->w, bmp2->h, r2)

' Leave if the rectangle do not collide
If(r1.a.x > r2.b.x Or r2.a.x > r1.b.x Or r1.a.y > r2.b.y Or r2.a.y > r1.b.y) Then
Return FALSE
End If

' Rectangle collision; do a detailed check
' Find the intersecting rectangle
r.a.x = IIf(r1.a.x > r2.a.x, r1.a.x, r2.a.x)
r.a.y = IIf(r1.a.y > r2.a.y, r1.a.y, r2.a.y)
r.b.x = IIf(r1.b.x < r2.b.x, r1.b.x, r2.b.x)
	r.b.y = IIf(r1.b.y < r2.b.y, r1.b.y, r2.b.y)

	' Find the width and height of the intersecting rectangle
	w = 1 + r.b.x - r.a.x
	h = 1 + r.b.y - r.a.y

	' Find the starting x & y positions of intersection in the bitmaps
	y1 = r.a.y - bmp1pt.y
	y2 = r.a.y - bmp2pt.y
	x1s = r.a.x - bmp1pt.x
	x2s = r.a.x - bmp2pt.x

	' Check both bitmap for collission
	For j = 0 To h - 1
	x1 = x1s
	x2 = x2s
	For i = 0 To w - 1
	If(_getpixel(bmp1, x1, y1) < > 0 AndAlso _getpixel(bmp2, x2, y2) < > 0) Then
	Return TRUE
	End If

	x1 += 1
	x2 += 1
	Next

	y1 += 1
	y2 += 1
	Next

	Return FALSE
	End Function


	/*
		Function: LoadSprites
		Description:
			Loads the hero, alien, and missile sprites and initializes the
			sprite structures.
	*/
	Sub SpritesInitialize()
	Dim i As Integer

	/* load hero spaceship */
	HeroBitmap = load_bitmap(ExePath & "/dat/gfx/hero.pcx", @GamePalette(0))
	HeroWidth = HeroBitmap->W
	HeroHeight = HeroBitmap->H
	HeroXMin = 0
	HeroXMax = SCREEN_WIDTH - HeroWidth
	HeroYMin = 0
	HeroYMax = REDUCED_SCREEN_HEIGHT - HeroHeight

	/* set up gun blink stuff */
	GunColor.R = GamePalette(GUN_COLOR).R
	GunColor.G = GamePalette(GUN_COLOR).G
	GunColor.B = GamePalette(GUN_COLOR).B
	GunBlinkCounter = GUN_BLINK_RATE
	GunBlinkState = TRUE

	/* load alien spaceship */
	AlienBitmap = load_bitmap(ExePath & "/dat/gfx/alien.pcx", @GamePalette(0))
	AlienWidth = AlienBitmap->W
	AlienHeight = AlienBitmap->H

	/* load missile */
	MissileBitmap = load_bitmap(ExePath & "/dat/gfx/missile.pcx", @GamePalette(0))
	MissileWidth = MissileBitmap->W
	MissileHeight = MissileBitmap->H

	/* load missile trails */
	MissileTrailUpBitmap = load_bitmap(ExePath & "/dat/gfx/missiletrail.pcx", @GamePalette(0))
	MissileTrailWidth = MissileTrailUpBitmap->W
	MissileTrailHeight = MissileTrailUpBitmap->H

	' Generate and initialize the other trail
	MissileTrailDnBitmap = create_bitmap(MissileTrailWidth, MissileTrailHeight)
	' Clear the bitmap to color 0
	clear_bitmap(MissileTrailDnBitmap)
	' Blit the missiletrailup v inverted
	draw_sprite_v_flip(MissileTrailDnBitmap, MissileTrailUpBitmap, 0, 0)

	/* load explosion bitmaps */
	ExplosionBitmap(0) = load_bitmap(ExePath & "/dat/gfx/explosion1.pcx", @GamePalette(0))
	ExplosionWidth = ExplosionBitmap(0)->W
	ExplosionHeight = ExplosionBitmap(0)->H

	ExplosionBitmap(1) = load_bitmap(ExePath & "/dat/gfx/explosion2.pcx", @GamePalette(0))

	ExplosionBitmap(2) = load_bitmap(ExePath & "/dat/gfx/explosion3.pcx", @GamePalette(0))

	ExplosionBitmap(3) = load_bitmap(ExePath & "/dat/gfx/explosion4.pcx", @GamePalette(0))

	ExplosionBitmap(4) = load_bitmap(ExePath & "/dat/gfx/explosion5.pcx", @GamePalette(0))

	/* initialize Hero SPRITE */
	Hero.Active = TRUE
	Hero.p.x = (HeroXMin + HeroXMax) / 2
	Hero.p.y = (HeroYMin + HeroYMax) / 2
	Hero.bDraw = TRUE

	/* initialize alien sprites */
	For i = 0 To MAX_ALIENS - 1
	Alien(i).Active = FALSE
	Alien(i).bDraw = FALSE
	Next

	/* initialize alien missiles */
	For i = 0 To MAX_ALIEN_MISSILES - 1
	AlienMissile(i).Active = FALSE
	AlienMissile(i).bDraw = FALSE
	Next

	/* initialize hero missiles */
	For i = 0 To MAX_HERO_MISSILES - 1
	HeroMissile(i).Active = FALSE
	HeroMissile(i).bDraw = FALSE
	Next

	/* initialize explosions */
	For i = 0 To MAX_EXPLOSIONS - 1
	Explosion(i).Active = FALSE
	Explosion(i).bDraw = FALSE
	Next
	End Sub


	/*
		Function: FreeSprites
		Description:
			Frees the memory occupied by the sprites.
	*/
	Sub SpritesFinalize()
	Dim i As Integer

	If(HeroBitmap <> NULL) Then
	destroy_bitmap(HeroBitmap)
	HeroBitmap = NULL
	End If

	If(AlienBitmap <> NULL) Then
	destroy_bitmap(AlienBitmap)
	AlienBitmap = NULL
	End If

	If(AlienBitmap <> NULL) Then
	destroy_bitmap(AlienBitmap)
	AlienBitmap = NULL
	End If

	If(MissileBitmap <> NULL) Then
	destroy_bitmap(MissileBitmap)
	MissileBitmap = NULL
	End If

	If(MissileTrailUpBitmap <> NULL) Then
	destroy_bitmap(MissileTrailUpBitmap)
	MissileTrailUpBitmap = NULL
	End If

	If(MissileTrailDnBitmap <> NULL) Then
	destroy_bitmap(MissileTrailDnBitmap)
	MissileTrailDnBitmap = NULL
	End If

	For i = 0 To MAX_EXPLOSION_BITMAPS - 1
	If(ExplosionBitmap(i) < > NULL) Then
	destroy_bitmap(ExplosionBitmap(i))
	ExplosionBitmap(i) = NULL
	End If
	Next
	End Sub


	/*
		Function: GetKeyboardInput
		Description:
			Updates the "UserInput..." variables used by the MoveSprites
			routine from the keyboard input device.
	*/
	Function InputPoll(ByRef UserInputUp As Boolean, ByRef UserInputDown As Boolean, ByRef UserInputLeft As Boolean, ByRef UserInputRight As Boolean, ByRef UserInputFire As Boolean) As Boolean
	Dim As Integer MouseMickeyX, MouseMickeyY

	poll_mouse()
	poll_keyboard()
	get_mouse_mickeys(@MouseMickeyX, @MouseMickeyY)

	UserInputLeft = (MouseMickeyX < 0) Or key(KEY_LEFT)
	UserInputUp = (MouseMickeyY < 0) Or key(KEY_UP)
	UserInputRight = (MouseMickeyX > 0) Or key(KEY_RIGHT)
	UserInputDown = (MouseMickeyY > 0) Or key(KEY_DOWN)
	UserInputFire = (mouse_b < > 0) Or key(KEY_SPACE) Or key(KEY_LCONTROL) Or key(KEY_RCONTROL) Or key(KEY_ALT) Or key(KEY_ALTGR)

	Return key(KEY_ESC)
	End Function


	/*
		Function: CreateHeroMissile
		Description:
			Finds a non-active hero missile in the HeroMissile
			array and initializes it.
	*/
	Function HeroMissileCreate(ByVal x As Integer, ByVal y As Integer) As Boolean
	Dim i As Integer

	For i = 0 To MAX_HERO_MISSILES - 1
	If(Not HeroMissile(i).Active) Then
	HeroMissile(i).Active = TRUE
	HeroMissile(i).p.x = x
	HeroMissile(i).p.y = y
	HeroMissile(i).vx = 0
	HeroMissile(i).vy = -HERO_MISSILE_VELOCITY
	HeroMissile(i).bDraw = TRUE
	/* initialize bounding rect */
	BoundingRectCompute(HeroMissile(i).p.x, HeroMissile(i).p.y, MissileWidth, MissileHeight, HeroMissile(i).Bounds)
	Return TRUE
	End If
	Next

	Return FALSE
	End Function


	/*
		Function: CreateAlien
		Description:
			Finds a free alien in the Alien array and initializes it.
	*/
	Sub AlienCreate()
	Dim i As Integer

	For i = 0 To MAX_ALIENS - 1
	If(Not Alien(i).Active) Then
	Alien(i).Active = TRUE
	Alien(i).p.x = Rnd * (SCREEN_WIDTH - AlienWidth)
	Alien(i).p.y = -AlienHeight							' we want the aliens off-screen
	Alien(i).vx = Rnd * ALIEN_X_VELOCITY + 1
	Alien(i).vy = Rnd * ALIEN_Y_VELOCITY + 1
	Alien(i).bDraw = TRUE
	Alien(i).ObjectSpec1 = ALIEN_MOVE_TIME_BASE + Rnd * ALIEN_MOVE_TIME_VAR
	Alien(i).ObjectSpec2 = 0 /* ability to fire immediately */
	/* initialize alien bounding rect */
	BoundingRectCompute(Alien(i).p.x, Alien(i).p.y, AlienWidth, AlienHeight, Alien(i).Bounds)
	Exit For
	End If
	Next
	End Sub


	/*
		Function: CreateAlienMissile
		Description:
			Finds a free alien missile in the AlienMissile array and
			initializes it.  The x and y positions of the missile
			are set from the x and y parameters which will place
			them somewhere near an alien gun.
	*/
	Sub AlienMissileCreate(ByVal x As Integer, ByVal y As Integer)
	Dim i As Integer

	For i = 0 To MAX_ALIEN_MISSILES - 1
	If(Not AlienMissile(i).Active) Then
	AlienMissile(i).Active = TRUE
	AlienMissile(i).p.x = x
	AlienMissile(i).p.y = y
	AlienMissile(i).vx = 0
	AlienMissile(i).vy = ALIEN_MISSILE_VELOCITY
	AlienMissile(i).bDraw = TRUE
	/* initialize bounding rect */
	BoundingRectCompute(AlienMissile(i).p.x, AlienMissile(i).p.y, MissileWidth, MissileHeight, AlienMissile(i).Bounds)
	Exit For
	End If
	Next
	End Sub


	/*
		Function: CreateExplosion
		Description:
			Starts an explosion occuring at the appropriate x and y
			coordinates.
	*/
	Sub ExplosionCreate(ByVal x As Integer, ByVal y As Integer)
	Dim i As Integer

	For i = 0 To MAX_EXPLOSIONS - 1
	If(Not Explosion(i).Active) Then
	Explosion(i).Active = TRUE
	Explosion(i).p.x = x
	Explosion(i).p.y = y
	Explosion(i).bDraw = TRUE
	Explosion(i).ObjectSpec1 = 0	 /* current explosion bitmap */
	Explosion(i).ObjectSpec2 = EXPLOSION_FRAME_REPEAT_COUNT
	Exit For
	End If
	Next
	End Sub


	/*
		Function: DrawStatus
		Description:
			Draws the status area at the bottom of the screen
			showing the player's current score and shield strength.
	*/
	Sub HUDDraw()
	Dim ScoreText As String * 6
	Dim i As Integer = HUDBitmap->h * 2			' Our HUD was originally for 320 x 240; so we gotta stretch it
	Dim j As Integer = SCORE_NUMBERS_LEFT
	Dim As Integer k, w, h

	' First draw the HUD panel onto the frame buffer
	stretch_sprite(FrameBuffer, HUDBitmap, 0, FrameBuffer->h - i, FrameBuffer->w, i)

	' Update the shield status
	rectfill(FrameBuffer, SHIELD_STATUS_LEFT, SHIELD_STATUS_TOP, SHIELD_STATUS_RIGHT, SHIELD_STATUS_BOTTOM, SHIELD_STATUS_INVERT_COLOR)
	rectfill(FrameBuffer, SHIELD_STATUS_LEFT, SHIELD_STATUS_TOP, SHIELD_STATUS_LEFT + HeroShields, SHIELD_STATUS_BOTTOM, SHIELD_STATUS_COLOR)

	ScoreText = Right("000000" & Str(Score), 6)

	' Render the score
	For i = 1 To 6
	k = Asc(ScoreText, i) - &H30		' &h30 is "0"
	w = HUDNumberBitmap(k)->w * 2
	h = HUDNumberBitmap(k)->h * 2
	stretch_sprite(FrameBuffer, HUDNumberBitmap(k), j, SCORE_NUMBERS_TOP, w, h)
	j += w
	Next
	End Sub


	/*
		Function: InitStatus
		Description:
			Draw the background and "Shield" and "Score" bitmaps.
	*/
	Sub HUDInitialize()
	Dim i As Integer

	' Load the HUD bitmap
	HUDBitmap = load_bitmap(ExePath & "/dat/gfx/hud.pcx", @GamePalette(0))

	' Load the numbers bitmaps
	For i = 0 To 9
	HUDNumberBitmap(i) = load_bitmap(ExePath & "/dat/gfx/" & Str(i) & ".pcx", @GamePalette(0))
	Next
	End Sub


	' Destroys the HUD
	Sub HUDFinalize()
	Dim i As Integer

	If(HUDBitmap <> NULL) Then
	destroy_bitmap(HUDBitmap)
	HUDBitmap = NULL
	End If

	For i = 0 To 9
	If(HUDNumberBitmap(i) < > NULL) Then
	destroy_bitmap(HUDNumberBitmap(i))
	HUDNumberBitmap(i) = NULL
	End If
	Next
	End Sub


	' Initialize the actual game FrameBuffer that we will draw to during play
	Sub FrameBufferInitialize()
	' Create a h/w accelerated bitmap framebuffer first
	FrameBuffer = create_system_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT)

	If(FrameBuffer = NULL) Then
	' Create a regular memory bitmap framebuffer
	FrameBuffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT)
	End If
	End Sub


	' Destroys the framebuffer once we are out of the actual game
	Sub FrameBufferFinalize()
	If(FrameBuffer <> NULL) Then
	destroy_bitmap(FrameBuffer)
	FrameBuffer = NULL
	End If
	End Sub


	' Renders the framebuffer contents to the screen
	Sub FrameBufferFlip()
	blit(FrameBuffer, Screen, 0, 0, 0, 0, FrameBuffer->w, FrameBuffer->h)
	End Sub


	/*
		Function: InitMap
		Description:
			Initialize the map with random tiles.
	*/
	Sub MapInitialize()
	Dim As Integer x, y, w, h, c

	' Create the main tile buffer first
	MapBuffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT)

	/* load the background tiles to video memory */
	Tile(0) = load_bitmap(ExePath & "/dat/gfx/stars1.pcx", @GamePalette(0))
	Tile(1) = load_bitmap(ExePath & "/dat/gfx/stars2.pcx", @GamePalette(0))
	Tile(2) = load_bitmap(ExePath & "/dat/gfx/earth.pcx", @GamePalette(0))

	' Set other variables
	MapScrollStep = MAP_SCROLL_STEP_NORMAL
	MapLineCounter = -TILE_HEIGHT

	' Just draw ramdom tiles on the background
	w = (MapBuffer->w / TILE_WIDTH) - 1
	h = (MapBuffer->h / TILE_HEIGHT) - 1

	For y = 0 To h
	For x = 0 To w
	' We just need more stars and less planets
	c = Rnd * 256
	If(c = 128) Then
	c = NUM_TILES - 1
	Else
	c Mod = 2
	End If

	' Blit the random tile
	blit(Tile(c), MapBuffer, 0, 0, x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT)
	Next
	Next
	End Sub


	' Destroys the background tile map stuff
	Sub MapFinalize()
	Dim i As Integer

	If(MapBuffer <> NULL) Then
	destroy_bitmap(MapBuffer)
	MapBuffer = NULL
	End If

	For i = 0 To NUM_TILES - 1
	If(Tile(i) < > NULL) Then
	destroy_bitmap(Tile(i))
	Tile(i) = NULL
	End If
	Next
	End Sub


	/*
		Function: ScrollScreen
		Description:
			Scroll the hidden page up a few lines and draw the next row
			of tiles above, if necessary.  Moves the screen
	*/
	Sub MapScroll()
	Dim As Integer i, c

	' Check all new tiles are completely shown; if so reset it
	If(MapLineCounter > 0) Then MapLineCounter = -TILE_HEIGHT

	' Check if we have to generate a fresh set of tiles at the top of the map
	If(MapLineCounter <= -TILE_HEIGHT) Then
	' Okay. Generate all the new tiles to be draw at the top of the map
	For i = 0 To(SCREEN_WIDTH / TILE_WIDTH) - 1
	' We just need more stars and less planets
	c = Rnd * 256
	If(c = 128) Then
	c = NUM_TILES - 1
	Else
	c Mod = 2
	End If

	MapLine(i) = c
	Next
	End If

	' Shift the entire background down by "scrollstep" pixels
	blit(MapBuffer, MapBuffer, 0, 0, 0, MapScrollStep, MapBuffer->w, MapBuffer->h - MapScrollStep)

	' Move the new tiles down by "scrollstep"
	MapLineCounter += MapScrollStep

	' Draw the new tiles at the top
	For i = 0 To(SCREEN_WIDTH / TILE_WIDTH) - 1
	blit(Tile(MapLine(i)), MapBuffer, 0, 0, i * TILE_WIDTH, MapLineCounter, TILE_WIDTH, TILE_HEIGHT)
	Next
	End Sub


	' Draws the map buffer to the frame buffer
	Sub MapDraw()
	blit(MapBuffer, FrameBuffer, 0, 0, 0, 0, MapBuffer->w, MapBuffer->h)
	End Sub


	' Loads and plays a MIDI file (loops it too)
	Sub MIDIPlay(s As String)
	' Unload music if loaded
	If(GameMusic <> NULL) Then
	destroy_midi(GameMusic)
	GameMusic = NULL
	End If

	' Load the music file
	GameMusic = load_midi(ExePath & "/dat/snd/mus/" & s)

	' Play it if loading was successful
	If(GameMusic <> NULL) Then	play_midi(GameMusic, TRUE)
	End Sub








	/*
		Function: CenterString
		Description:
			Centers a string on the screen.  The function calculates
			the correct starting column position to center the string
			on the screen and then calls DrawString to do the actual
			drawing of the text.
	*/
	Sub StringDrawCenter(s As String, ByVal y As Integer, ByVal c As Byte)
	textout_centre(Screen, font, StrPtr(s), SCREEN_WIDTH / 2, y * text_height(font), c)
	End Sub


	/*
		Function: DisplayHighScores
		Description:
			Displays the HighScore array on the screen.
	*/
	Sub HighScoresDisplay()
	Dim i As Integer

	StringDrawCenter("####===-- High Scores --===####", 4, 14)
	For i = 0 To UBound(HighScore)
	textprintf_centre(Screen, font, SCREEN_WIDTH / 2, 72 + i * text_height(font) * 4, HIGH_SCORE_COLOR, "%2d. %-20s  %5d", i + 1, StrPtr(HighScore(i).Text), HighScore(i).Score)
	Next
	End Sub


	/*
		Function: DisplayHighScoreScreen
		Description:
			Displays the high score screen from the title page.
	*/
	Sub HighScoresScreenDisplay()
	/* clear screen */
	clear_bitmap(Screen)

	/* set everything to black so we can draw without being seen */
	set_palette(@black_palette(0))

	HighScoresDisplay()

	' Fade in
	fade_in(@default_palette(0), 1)

	' Wait until a key is pressed
	clear_keybuf()
	readkey()

	' Fade out
	fade_out(1)
	End Sub


	/*
		Function: NewHighScore
		Description:
			Manipulates the HighScore array to make room for the
			users score and gets the new text.
	*/
	Sub HighScoresNew(ByVal NewScore As Integer)
	Dim As Integer i, Row, Column, sLen, k

	' check to see if it's really a high score
	If(NewScore <= HighScore(9).Score) Then Return

	/* start high score music */
	MIDIPlay("alienend.mid")

	/* move other scores down to make room */
	For i = 8 To 0 Step - 1
	If(NewScore > HighScore(i).Score) Then
	HighScore(i + 1).Text = HighScore(i).Text
	HighScore(i + 1).Score = HighScore(i).Score
	Else
	Exit For
	End If
	Next
	i += 1

	/* blank out text of correct slot */
	HighScore(i).Text = vbNullString
	HighScore(i).Score = NewScore

	/* clear screen */
	clear_bitmap(Screen)

	/* set everything to black so we can draw without being seen */
	set_palette(@black_palette(0))

	HighScoresDisplay()

	' Fade in
	fade_in(@default_palette(0), 1)

	/* get user text string */
	Row = 72 + i * text_height(font) * 4
	Column = 228
	sLen = 0
	clear_keybuf()
	' This section is a mess; got to clear up and comment this someday
	Do
	textout(Screen, font, Chr(127), Column, Row, HIGH_SCORE_COLOR)
	k = LoByte(readkey())
	If(Asc(" ") <= k And k <= 126 And sLen < HIGH_SCORE_TEXT_LEN) Then
	textout(Screen, font, Chr(k), Column, Row, HIGH_SCORE_COLOR)
	HighScore(i).Text[sLen] = Chr(k)
	sLen += 1
	HighScore(i).Text[sLen] = Chr(0)
	Column += text_length(font, "X")
	ElseIf(8 = k And sLen > 0) Then
	textout(Screen, font, " ", Column, Row, HIGH_SCORE_COLOR)
	sLen -= 1
	Column -= text_length(font, "X")
	HighScore(i).Text[sLen] = Chr(0)
	End If
	Loop While(k < > 13)

	/* erase cursor */
	textout(Screen, font, " ", Column, Row, HIGH_SCORE_COLOR)

	/* fade to black... */
	fade_out(1)
	End Sub


	/*
		Function: TitlePage
		Description:
			Displays the Alien Alley title page.
	*/
	Sub TitlePageDisplay()
	Dim Image As BITMAP Pointer

	/* start title music */
	MIDIPlay("alienintro.mid")

	/* clear screen */
	clear_bitmap(Screen)

	/* set everything to black so we can draw without being seen */
	set_palette(@black_palette(0))

	/* first page of stuff */
	Image = load_bitmap(ExePath & "/dat/gfx/title.pcx", @GamePalette(0))

	' Stretch bmp to fill the screen
	If(Image <> NULL) Then
	stretch_blit(Image, Screen, 0, 0, Image->w, Image->h, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)
	destroy_bitmap(Image)
	End If

	' Fade in
	fade_in(@GamePalette(0), 1)
	End Sub


	/*
		Function: IntroCredits
		Description:
			Displays the introduction credits.
	*/
	Sub IntroCreditsDisplay()
	' Clear the screen
	clear_bitmap(Screen)

	/* set everything to black so we can draw without being seen */
	set_palette(@black_palette(0))

	/* first page of stuff */
	StringDrawCenter("Coriolis Group Books", 28, INTRO_TEXT_COLOR)
	StringDrawCenter("Presents", 31, INTRO_TEXT_COLOR)
	fade_in(@default_palette(0), 1)
	fade_out(1)

	' Clear the screen
	clear_bitmap(Screen)

	/* second page of stuff */
	StringDrawCenter("A", 27, INTRO_TEXT_COLOR)
	StringDrawCenter("Dave Roberts", 29, INTRO_TEXT_COLOR)
	StringDrawCenter("Production", 31, INTRO_TEXT_COLOR)
	fade_in(@default_palette(0), 1)
	fade_out(1)
	End Sub


	/*
		Function: DefaultHighScores
		Description:
			Fills in the HighScore array with some defaults.
			Have fun with this.
	*/
	Sub HighScoresDefault()
	HighScore(0).Text = "Gill Bates"
	HighScore(0).Score = 1000

	HighScore(1).Text = "Sam Stone"
	HighScore(1).Score = 900

	HighScore(2).Text = "Mad Mike"
	HighScore(2).Score = 800

	HighScore(3).Text = "Fanatic Joe"
	HighScore(3).Score = 700

	HighScore(4).Text = "Joe Dirt"
	HighScore(4).Score = 600

	HighScore(5).Text = "Sonic Hedgehog"
	HighScore(5).Score = 500

	HighScore(6).Text = "Donald Duck"
	HighScore(6).Score = 400

	HighScore(7).Text = "Popeye Sailor"
	HighScore(7).Score = 300

	HighScore(8).Text = "Flash Gordon"
	HighScore(8).Score = 200

	HighScore(9).Text = "John Blade"
	HighScore(9).Score = 100
	End Sub


	/*
		Function: LoadHighScores
		Description:
			Loads the high-score file from disk.  If a high-score file
			cannot be found or cannot be read, a default list of
			high-score entries is created.
	*/
	Sub HighScoresLoad()
	Dim As Integer HighScoreFile, i

	' Load default highscores if there is no highscore file
	If(Not FileExists(HIGH_SCORE_FILENAME)) Then
	HighScoresDefault()
	Return
	End If

	' Open the highscore file; if there is a problem load defaults
	HighScoreFile = FreeFile
	If(Open(HIGH_SCORE_FILENAME For Binary As HighScoreFile) < > 0) Then
	HighScoresDefault()
	Return
	End If

	' Read the name and the scores
	For i = 0 To UBound(HighScore)
	Get #HighScoreFile, , HighScore(i).Text
	Get #HighScoreFile, , HighScore(i).Score
	Next

	' Close file
	Close HighScoreFile
	End Sub


	/*
		Function: SaveHighScores
		Description:
			Writes the HighScore array out to the high-score file.
	*/
	Sub HighScoresSave()
	Dim As Integer HighScoreFile, i

	' Open the file for writing
	HighScoreFile = FreeFile
	If(Open(HIGH_SCORE_FILENAME For Binary As HighScoreFile) < > 0) Then
	' Error; just ignore it
	Return
	End If

	For i = 0 To UBound(HighScore)
	Put #HighScoreFile, , HighScore(i).Text
	Put #HighScoreFile, , HighScore(i).Score
	Next

	Close HighScoreFile
	End Sub


	/*
		Function: MoveSprites
		Description:
			Takes care of moving hero ship and alien sprites, based on
			user input and their behavioral algorithms.  MoveSprites
			is also where missiles are generated and off-screen images
			are removed from play.
	*/
	Sub SpritesMove(ByVal UserInputUp As Boolean, ByVal UserInputDown As Boolean, ByVal UserInputLeft As Boolean, ByVal UserInputRight As Boolean, ByVal UserInputFire As Boolean)
	Dim i As Integer
	Dim AlienFireResult As Integer
	Dim AlienProximity As Integer

	/* first, take care of the hero */
	If(UserInputUp) Then Hero.p.y -= HERO_Y_VELOCITY
	If(UserInputDown) Then Hero.p.y += HERO_Y_VELOCITY
	If(UserInputLeft) Then Hero.p.x -= HERO_X_VELOCITY
	If(UserInputRight) Then Hero.p.x += HERO_X_VELOCITY

	/* limit player movement */
	If(Hero.p.y > HeroYMax) Then Hero.p.y = HeroYMax
	If(Hero.p.y < HeroYMin) Then Hero.p.y = HeroYMin
	If(Hero.p.x > HeroXMax) Then Hero.p.x = HeroXMax
	If(Hero.p.x < HeroXMin) Then Hero.p.x = HeroXMin

	/* update hero bounding rect */
	BoundingRectCompute(Hero.p.x, Hero.p.y, HeroWidth, HeroHeight, Hero.Bounds)

	/* update hero missiles */
	For i = 0 To MAX_HERO_MISSILES - 1
	If(HeroMissile(i).bDraw) Then
	/* update position */
	HeroMissile(i).p.y += HeroMissile(i).vy
	' stop drawing when it's off screen
	If(HeroMissile(i).p.y < -(MissileHeight + MissileTrailHeight)) Then
	HeroMissile(i).bDraw = FALSE
	Else
	/* if still onscreen, update bounding rect */
	BoundingRectCompute(HeroMissile(i).p.x, HeroMissile(i).p.y, MissileWidth, MissileHeight, HeroMissile(i).Bounds)
	End If
	End If
	Next

	/* generate hero missiles */
	If(UserInputFire And HeroAllowFire And Hero.bDraw) Then
	If(HeroMissileCreate(Hero.p.x + HERO_GUN_OFFSET_LEFT, Hero.p.y + HERO_GUN_OFFSET_UP) And HeroMissileCreate(Hero.p.x + HERO_GUN_OFFSET_RIGHT, Hero.p.y + HERO_GUN_OFFSET_UP)) Then
	SfxPlay(LaserSound, Hero.p.x, Hero.p.y, HeroBitmap)
	End If
	HeroAllowFire = FALSE
	End If

	/* update alien missiles */
	For i = 0 To MAX_ALIEN_MISSILES - 1
	If(AlienMissile(i).bDraw) Then
	/* update position */
	AlienMissile(i).p.y += AlienMissile(i).vy
	' stop drawing when it's off screen
	If(AlienMissile(i).p.y > (SCREEN_HEIGHT + MissileHeight + MissileTrailHeight)) Then
	AlienMissile(i).bDraw = FALSE
	Else
	/* if still onscreen, update bounding rect */
	BoundingRectCompute(AlienMissile(i).p.x, AlienMissile(i).p.y, MissileWidth, MissileHeight, AlienMissile(i).Bounds)
	End If
	End If
	Next

	/* move aliens */
	For i = 0 To MAX_ALIENS - 1
	If(Alien(i).bDraw) Then
	If(Alien(i).ObjectSpec1 = 0) Then
	/* pick a new direction */
	Alien(i).vx = IIf(Int(Timer) Mod 2, Rnd * ALIEN_X_VELOCITY, Rnd * -ALIEN_X_VELOCITY)
	Alien(i).ObjectSpec1 = ALIEN_MOVE_TIME_BASE + Rnd * ALIEN_MOVE_TIME_VAR
	Else
	Alien(i).ObjectSpec1 -= 1
	End If
	/* update alien position */
	Alien(i).p.x += Alien(i).vx
	Alien(i).p.y += Alien(i).vy

	/* clip alien movement horizontally */
	If(Alien(i).p.x < 0) Then Alien(i).p.x = 0
	If(Alien(i).p.x > SCREEN_WIDTH - AlienWidth) Then Alien(i).p.x = SCREEN_WIDTH - AlienWidth

	/* move alien to top when it gets to bottom */
	If(Alien(i).p.y > SCREEN_HEIGHT + AlienHeight) Then Alien(i).p.y = -AlienHeight

	/* update alien bouding rect */
	BoundingRectCompute(Alien(i).p.x, Alien(i).p.y, AlienWidth, AlienHeight, Alien(i).Bounds)

	/* generate alien missiles */
	If(Alien(i).ObjectSpec2 = 0) Then
	AlienFireResult = Rnd * 100	/* in percent */
	AlienProximity = Alien(i).p.x - Hero.p.x

	If(AlienProximity < 0) Then AlienProximity = -AlienProximity

	If(((AlienProximity < ALIEN_PROX_THRESHOLD) And(AlienFireResult < ALIEN_FIRE_PROB_HERO)) Or(AlienFireResult < ALIEN_FIRE_PROB_RANDOM)) Then
	AlienMissileCreate(Alien(i).p.x + ALIEN_GUN_OFFSET_LEFT, Alien(i).p.y + ALIEN_GUN_OFFSET_DOWN)
	AlienMissileCreate(Alien(i).p.x + ALIEN_GUN_OFFSET_RIGHT, Alien(i).p.y + ALIEN_GUN_OFFSET_DOWN)
	Alien(i).ObjectSpec2 = ALIEN_FIRE_LOCKOUT
	SfxPlay(LaserSound, Alien(i).p.x, Alien(i).p.y, AlienBitmap)
	End If
	Else
	Alien(i).ObjectSpec2 -= 1
	End If
	End If
	Next

	/* generate aliens */
	If(AlienGenCounter = 0) Then
	/* generate an alien */
	AlienCreate()
	/* reinit generate counter */
	AlienGenCounter = ALIEN_GEN_RATE_BASE + Rnd * ALIEN_GEN_RATE_VAR
	Else
	AlienGenCounter -= 1
	End If

	' update explosions -- note, we don't really "move" them, just make the animation go
	For i = 0 To MAX_EXPLOSIONS - 1
	If(Explosion(i).bDraw) Then
	If(Explosion(i).ObjectSpec2 = 0) Then
	Explosion(i).ObjectSpec1 += 1
	Explosion(i).ObjectSpec2 = EXPLOSION_FRAME_REPEAT_COUNT
	If(Explosion(i).ObjectSpec1 >= MAX_EXPLOSION_BITMAPS) Then Explosion(i).bDraw = FALSE
	Else
	Explosion(i).ObjectSpec2 -= 1
	End If
	End If
	Next

	' Check at what speed the map needs to be scrolled
	MapScrollStep = IIf(UserInputUp, MAP_SCROLL_STEP_FAST, MAP_SCROLL_STEP_NORMAL)
	End Sub


	/*
		Function: CheckCollisions
		Description:
			Check for collisions between various objects and start
			explosions if they collide.  Collision detection is
			performed between:
				* aliens and hero
				* aliens and hero missiles
				* hero and alien missiles
			Note that all tests are performed between objects that are
			currently being drawn, not just active objects.
	*/
	Sub CollisionsCheck()
	Dim As Integer i, j

	/* check between hero and aliens */
	For i = 0 To MAX_ALIENS - 1
	/* Use C short circuit boolean evaluation in a big way. */
	/* Make sure both hero and alien are still being drawn */
	/* (they may still be active but have been removed */
	/* from the screen and are just being erased). */
	/* If they are still onscreen, then perform a rectangle test. */
	/* If the rectangle collision indicates a possible hit, then */
	/* perform a bitmap test. */
	If(Hero.bDraw AndAlso Alien(i).bDraw AndAlso CollisionTest(HeroBitmap, Hero.p, AlienBitmap, Alien(i).p)) Then
	Hero.bDraw = FALSE
	ExplosionCreate(Hero.p.x, Hero.p.y)
	Alien(i).bDraw = FALSE
	ExplosionCreate(Alien(i).p.x, Alien(i).p.y)
	SfxPlay(ExplosionSound, Alien(i).p.x, Alien(i).p.y, ExplosionBitmap(0))
	End If
	Next

	/* check between aliens and hero missiles */
	For i = 0 To MAX_ALIENS - 1
	If(Not Alien(i).bDraw) Then Continue For

	For j = 0 To MAX_HERO_MISSILES - 1
	/* do similiar short circuit, mondo huge test as above */
	If(HeroMissile(j).bDraw AndAlso CollisionTest(AlienBitmap, Alien(i).p, MissileBitmap, HeroMissile(j).p)) Then
	Alien(i).bDraw = FALSE
	HeroMissile(j).bDraw = FALSE
	ExplosionCreate(Alien(i).p.x, Alien(i).p.y)
	Score += POINTS_PER_ALIEN
	SfxPlay(ExplosionSound, Alien(i).p.x, Alien(i).p.y, ExplosionBitmap(0))
	Exit For /* alien is destroyed */
	End If
	Next
	Next

	/* check between hero and alien missiles */
	For i = 0 To MAX_ALIEN_MISSILES - 1
	/* again, rely on short circuiting */
	If(AlienMissile(i).bDraw AndAlso Hero.bDraw AndAlso CollisionTest(MissileBitmap, AlienMissile(i).p, HeroBitmap, Hero.p)) Then
	AlienMissile(i).bDraw = FALSE	/* destroy missile in any case */
	If(HeroShields <= 0) Then
	Hero.bDraw = FALSE
	ExplosionCreate(Hero.p.x, Hero.p.y)
	SfxPlay(ExplosionSound, Hero.p.x, Hero.p.y, ExplosionBitmap(0))
	Exit For	/* hero is destroyed */
	Else
	/* take away a bit of shields */
	HeroShields -= 5
	If(HeroShields < 0) Then HeroShields = 0
	End If
	End If
	Next
	End Sub


	/*
		Function: EraseSprites
		Description:
			Erase all current bitmaps from the hidden screen.  If the
			erasure marks the last time that the object will be erased
			because it is no longer being drawn, deactivate the object.
	*/
	Function SpritesErase() As Boolean
	Dim i As Integer
	Static DeathCounter As UInteger

	/* do player and possibly deactivate */
	If(Hero.Active) Then
	If(Not Hero.bDraw) Then
	Hero.Active = FALSE
	DeathCounter = DEATH_DELAY
	End If
	End If

	/* erase and deactivate hero missiles */
	For i = 0 To MAX_HERO_MISSILES - 1
	' deactivate missile if we aren't going to draw or erase it anymore
	If(Not HeroMissile(i).bDraw) Then
	HeroMissile(i).Active = FALSE
	End If
	Next

	/* erase and deactivate aliens */
	For i = 0 To MAX_ALIENS - 1
	' deactive alien if it's been destroyed
	If(Not Alien(i).bDraw) Then
	Alien(i).Active = FALSE
	End If
	Next

	/* erase and deactivate alien missiles */
	For i = 0 To MAX_ALIEN_MISSILES - 1
	' deactivate missile if we aren't going to draw or erase it anymore
	If(Not AlienMissile(i).bDraw) Then
	AlienMissile(i).Active = FALSE
	End If
	Next

	/* erase and deactivate explosions */
	For i = 0 To MAX_EXPLOSIONS - 1
	/* deactivate if explosion has run its course */
	If(Not Explosion(i).bDraw) Then
	Explosion(i).Active = FALSE
	End If
	Next

	/* hero has died -- signal game over after brief delay */
	If(Not Hero.Active) Then
	If(DeathCounter = 0) Then
	Return TRUE
	Else
	DeathCounter -= 1
	End If
	End If

	Return FALSE
	End Function


	/*
		Function: DrawSprites
		Description:
			Draw all active objects that should be drawn on the
			screen.
	*/
	Sub SpritesDraw()
	Dim i As Integer

	/* do explosions */
	For i = 0 To MAX_EXPLOSIONS - 1
	If(Explosion(i).bDraw) Then
	/* draw explosion */
	draw_sprite(FrameBuffer, ExplosionBitmap(Explosion(i).ObjectSpec1), Explosion(i).p.x, Explosion(i).p.y)
	End If
	Next

	/* draw hero missiles */
	For i = 0 To MAX_HERO_MISSILES - 1
	If(HeroMissile(i).bDraw) Then
	/* draw missile itself */
	draw_sprite(FrameBuffer, MissileBitmap, HeroMissile(i).p.x, HeroMissile(i).p.y)
	/* draw missile trail */
	draw_sprite(FrameBuffer, MissileTrailUpBitmap, HeroMissile(i).p.x, HeroMissile(i).p.y + MissileBitmap->h)
	End If
	Next

	/* draw alien missiles */
	For i = 0 To MAX_ALIEN_MISSILES - 1
	If(AlienMissile(i).bDraw) Then
	/* draw missile itself */
	draw_sprite(FrameBuffer, MissileBitmap, AlienMissile(i).p.x, AlienMissile(i).p.y)
	/* draw missile trail */
	draw_sprite(FrameBuffer, MissileTrailDnBitmap, AlienMissile(i).p.x, AlienMissile(i).p.y - MissileTrailDnBitmap->h)
	End If
	Next

	/* do aliens */
	For i = 0 To MAX_ALIENS - 1
	If(Alien(i).Active And Alien(i).bDraw) Then
	draw_sprite(FrameBuffer, AlienBitmap, Alien(i).p.x, Alien(i).p.y)
	End If
	Next

	/* do player */
	If(Hero.Active And Hero.bDraw) Then
	draw_sprite(FrameBuffer, HeroBitmap, Hero.p.x, Hero.p.y)
	End If

	/* blink the guns */
	If(GunBlinkCounter = 0) Then
	If(GunBlinkState) Then
	set_color(GUN_COLOR, @black_palette(0))
	Else
	set_color(GUN_COLOR, @GunColor)
	End If
	GunBlinkState = Not GunBlinkState /* flip it to other state */
	GunBlinkCounter = GUN_BLINK_RATE
	HeroAllowFire = TRUE
	Else
	GunBlinkCounter -= 1
	End If
	End Sub


	/*
		Function: Play
		Description:
			Play the game!
	*/
	Sub Play()
	Dim As Boolean UserInputUp, UserInputDown, UserInputLeft, UserInputRight, UserInputFire
	Dim As Boolean GameOver, GameOverInput, GameOverDeath

	/* initialize all counters, etc. */
	Score = 0
	AlienGenCounter = ALIEN_GEN_RATE_BASE
	HeroShields = MAX_HERO_SHIELDS

	FrameBufferInitialize()
	MapInitialize()
	HUDInitialize()
	SpritesInitialize()

	' Load game palette
	set_palette(@GamePalette(0))

	' Play the in-game music
	MIDIPlay("alienmain.mid")

	' Install our timer interrupt
	install_int_ex(@Ticker, BPS_TO_TIMER(UPDATES_PER_SECOND))

	/* Initialize some variables and enter main animation loop */
	Ticks = 0
	GameOver = FALSE

	Do
	If(Ticks > 0) Then
	/* get user input */
	GameOverInput = InputPoll(UserInputUp, UserInputDown, UserInputLeft, UserInputRight, UserInputFire)

	/* move sprites */
	SpritesMove(UserInputUp, UserInputDown, UserInputLeft, UserInputRight, UserInputFire)

	/* check for collisions */
	CollisionsCheck()

	/* erase */
	GameOverDeath = SpritesErase()

	/* scroll screen */
	MapScroll()

	' Draw map
	MapDraw()

	/* draw */
	SpritesDraw()

	' Draw HUD
	HUDDraw()

	/* Page flip */
	FrameBufferFlip()

	Ticks -= 1
	Else
	rest(0)
	End If

	/* player either aborts or dies */
	GameOver = GameOverInput Or GameOverDeath
	Loop While(Not GameOver)

	' Remove timer interrupt
	remove_int(@Ticker)

	SpritesFinalize()
	HUDFinalize()
	MapFinalize()
	FrameBufferFinalize()

	/* fade to black... */
	fade_out(1)
	End Sub

	//
	// MAIN
	//
	Function Main() As Integer
	Dim Quit As Boolean = FALSE
	Dim DrawTitle As Boolean = TRUE
	Dim k As UByte

	' Initialize everything we need
	ProgramInitialize()

	' Display the into credits screen
	IntroCreditsDisplay()

	' Main menu loop
	While(Not Quit)
	' Draw title page (only if required)
	If(DrawTitle) Then
	TitlePageDisplay()
	DrawTitle = FALSE
	End If

	' Read the key and mask the scancode
	clear_keybuf()
	k = LoByte(readkey())

	' Check what key was press and action it
	Select Case k
	Case 27, Asc("q"), Asc("Q")
	Quit = TRUE
	Case Asc("k"), Asc("K"), Asc("m"), Asc("M"), Asc("j"), Asc("J"), 13
	Play()
	HighScoresNew(Score)
	DrawTitle = TRUE
	Case Asc("s"), Asc("S")
	HighScoresScreenDisplay()
	DrawTitle = TRUE
	Case Else
	DrawTitle = FALSE
	End Select
	Wend

	' Fade out
	fade_out(1)

	' Release all resources
	ProgramFinalize()

	Return 0
	End Function


#endif