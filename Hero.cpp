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
	InitializeCheck(tmp_bmp, "dat/gfx/hero.png");
	// Since we have 32x32 bitmaps and the sheet is just 1 row
	sprite.setBitmap(tmp_bmp, al_get_bitmap_height(tmp_bmp), al_get_bitmap_height(tmp_bmp), 3);
}

Hero::~Hero()
{
	al_destroy_bitmap(sprite.spriteSheet);
}

void Hero::update(int gameLives, bool moveLeft, bool moveRight, bool moveUp, bool moveDown, bool shoot)
{
	
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

	if (invincibleTimer)
		invincibleTimer--;
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

	if (ship.shot_timer)
		ship.shot_timer--;
	else if (key[ALLEGRO_KEY_LCTRL] || key[ALLEGRO_KEY_SPACE] || key[ALLEGRO_KEY_RCTRL])
	{
		int x = ship.sprite.position.x + (ship.sprite.size.cx / 2);
		if (shots_add(true, false, x, ship.sprite.position.y))
			ship.shot_timer = 5;
	}
}

void Hero::draw()
{
	if (ship.lives <= 0)
		return;
	if (ship.respawn_timer)
		return;
	if (((ship.invincible_timer / 2) % 3) == 1)
		return;

	ship.sprite.draw();
}
