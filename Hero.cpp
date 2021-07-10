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

#include "Hero.h"

Hero::Hero()
{
	// Initialize the buffer width and height
	bufferWidth = al_get_display_width(al_get_current_display());
	bufferHeight = al_get_display_height(al_get_current_display());

	sprite.position.x = (bufferWidth / 2) - (sprite.size.cx / 2);
	sprite.position.y = (bufferHeight / 2) - (sprite.size.cy / 2);
	sprite.boundary.SetRect(0, 0, bufferWidth, bufferHeight);

	shotTimer = 0;
	respawnTimer = 0;
	invincibleTimer = INVINCIBLE_TIMER_DEFAULT;
	speed = SPEED_DEFAULT;
	lives = 0;
	
	ALLEGRO_BITMAP* tmp_bmp = al_load_bitmap("dat/gfx/hero.png");
	Game::checkInitialized(tmp_bmp, "dat/gfx/hero.png");

	// Since we have 32x32 bitmaps and the sheet is just 1 row
	sprite.setBitmap(tmp_bmp, al_get_bitmap_height(tmp_bmp), al_get_bitmap_height(tmp_bmp), 3);
}

Hero::~Hero()
{
	al_destroy_bitmap(sprite.spriteSheet);
}

// Update hero location, add missiles and effects based on input
void Hero::update(int gameLives, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, bool shoot, Missiles& mm, Effects& fm)
{
	// Queue in the final explosion if this is the last life
	if (gameLives < 0 && lives != gameLives)
	{
		int cx = sprite.position.x + (sprite.size.cx / 2);
		int cy = sprite.position.y + (sprite.size.cy / 2);

		fm.add(fm.EXPLOSION_BIG, cx, cy);
	}

	lives = gameLives;

	// No need to update if we are out of lives
	if (lives < 0)
		return;

	// Hero will come to life after some time
	if (respawnTimer)
	{
		respawnTimer--;
		return;
	}

	// Make the hero move based on input
	if (moveLeft)
		sprite.position.x -= speed;
	if (moveRight)
		sprite.position.x += speed;
	if (moveUp)
		sprite.position.y -= speed;
	if (moveDown)
		sprite.position.y += speed;

	// Make the sprite logic go
	sprite.update();

	// NOTE: Moved collision dectection to Game class

	// Check if we can shoot and shoot if we have correct user input
	if (shotTimer)
		shotTimer--;
	else if (shoot)
	{
		int x = sprite.position.x + (sprite.size.cx / 2);
		if (mm.add(mm.HERO, true, x, sprite.position.y))
			shotTimer = 5;
	}
}

void Hero::draw()
{
	if (lives <= 0)
		return;
	if (respawnTimer)
		return;
	if (((invincibleTimer / 2) % 3) == 1)
		return;

	sprite.draw();
}
