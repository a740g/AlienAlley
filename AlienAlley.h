#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include "CelestialObjects.h"

constexpr auto INTRO_TEXT_COLOR = 15;
constexpr auto MAX_ALIENS = 4;
constexpr auto MAX_ALIEN_MISSILES = 20;
constexpr auto MAX_HERO_MISSILES = 10;
constexpr auto MAX_EXPLOSIONS = MAX_ALIENS + 1;			// +1 for hero
constexpr auto MAX_EXPLOSION_BITMAPS = 5;
constexpr auto GUN_COLOR = 8;
constexpr auto GUN_BLINK_RATE = 20;
constexpr auto HERO_X_VELOCITY = 3;
constexpr auto HERO_Y_VELOCITY = 3;
constexpr auto ALIEN_X_VELOCITY = 3;
constexpr auto ALIEN_Y_VELOCITY = 2;
constexpr auto HERO_MISSILE_VELOCITY = 5;
constexpr auto ALIEN_MISSILE_VELOCITY = 4;
constexpr auto MAX_MOVE_STEP = 8;						// max sprite movement (hero missile)
constexpr auto ALIEN_MOVE_TIME_VAR = 50;
constexpr auto ALIEN_MOVE_TIME_BASE = 20;
constexpr auto ALIEN_GEN_RATE_BASE = 40;
constexpr auto ALIEN_GEN_RATE_VAR = 40;
constexpr auto ALIEN_FIRE_LOCKOUT = 60;
constexpr auto ALIEN_FIRE_PROB_HERO = 20;
constexpr auto ALIEN_FIRE_PROB_RANDOM = 10;
constexpr auto ALIEN_PROX_THRESHOLD = 20;
constexpr auto HERO_GUN_OFFSET_LEFT = 3;
constexpr auto HERO_GUN_OFFSET_RIGHT = 26;
constexpr auto HERO_GUN_OFFSET_UP = 10;
constexpr auto ALIEN_GUN_OFFSET_LEFT = 4;
constexpr auto ALIEN_GUN_OFFSET_RIGHT = 25;
constexpr auto ALIEN_GUN_OFFSET_DOWN = 20;
constexpr auto DEATH_DELAY = 60;						// 1 sec delay after player death
constexpr auto POINTS_PER_ALIEN = 10;
constexpr auto SHIELD_STATUS_WIDTH = 80;
constexpr auto MAX_HERO_SHIELDS = SHIELD_STATUS_WIDTH - 1;
constexpr auto SHIELD_STATUS_HEIGHT = 20;
constexpr auto SHIELD_STATUS_LEFT = 192;
constexpr auto SHIELD_STATUS_TOP = 440;
constexpr auto SHIELD_STATUS_RIGHT = SHIELD_STATUS_LEFT + SHIELD_STATUS_WIDTH - 1;
constexpr auto SHIELD_STATUS_BOTTOM = SHIELD_STATUS_TOP + SHIELD_STATUS_HEIGHT - 1;
constexpr auto SHIELD_STATUS_COLOR = 47;
constexpr auto SHIELD_STATUS_INVERT_COLOR = 173;
constexpr auto STATUS_BACKGROUND_COLOR = 27;
constexpr auto SCORE_NUMBERS_LEFT = 474;
constexpr auto SCORE_NUMBERS_TOP = 443;
constexpr auto EXPLOSION_FRAME_REPEAT_COUNT = 3;
constexpr auto HIGH_SCORE_TEXT_LEN = 20;
constexpr auto HIGH_SCORE_FILENAME = "highscore.dat";
constexpr auto HIGH_SCORE_COLOR = 2;
constexpr auto TILE_WIDTH = 32;							// in pixels
constexpr auto TILE_HEIGHT = 32;
constexpr auto NUM_TILES = 3;
constexpr auto UPDATES_PER_SECOND = 60;
constexpr auto STATUS_HEIGHT = 60;						// our hud is 60 pixels now 30 * 2 in 640x480 mode
constexpr auto MAP_SCROLL_STEP_NORMAL = 1;				// scrolling parameters
constexpr auto MAP_SCROLL_STEP_FAST = 2;
constexpr auto KEY_SEEN = 1;							// Key see and released flags
constexpr auto KEY_RELEASED = 2;
constexpr auto EXPLOSION_FRAMES = 5;
constexpr auto SPARKS_FRAMES = 3;
constexpr auto FX_N = 128;
constexpr auto SHOTS_N = 128;
constexpr auto SHIP_SPEED = 3;
constexpr auto ALIENS_N = 16;

// Types of aliens
enum ALIEN_TYPE_T
{
	ALIEN_TYPE_BUG = 0,
	ALIEN_TYPE_ARROW,
	ALIEN_TYPE_THICCBOI,
	ALIEN_TYPE_N
};

// Absolute X, Y
struct POINT_T
{
	int x;
	int y;
};

// Rectangle
struct RECT_T
{
	POINT_T a;
	POINT_T b;
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

	ALLEGRO_BITMAP* life;
	DIM_T life_d;

	ALLEGRO_BITMAP* alien[ALIEN_TYPE_N];
	DIM_T alien_d[ALIEN_TYPE_N];

	ALLEGRO_BITMAP* alien_shot;
	DIM_T alien_shot_d;

	ALLEGRO_BITMAP* explosion;
	ALLEGRO_BITMAP* sparks;

	ALLEGRO_BITMAP* powerup;
};

struct FX_T
{
	int x, y;
	int frame;
	bool spark;
	bool used;
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

void InitializeCheck(bool test, const char* description);
int between(int lo, int hi);
float between(float lo, float hi);
