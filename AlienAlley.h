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
#include "MainMenu.h"
#include "HUD.h"

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
	//ALIEN_TYPE_BOP,
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

void InitializeCheck(bool test, const char* description);
int between(int lo, int hi);
float between(float lo, float hi);
