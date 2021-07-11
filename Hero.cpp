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

#include "Hero.h"

Hero::Hero(HUD& hm)
{
	
	sprite = new Sprite();

	// Initialize the buffer width and height
	bufferSize.SetSize(al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display()));

	shotTimer = 0;
	respawnTimer = 0;
	invincibleTimer = INVINCIBLE_TIMER_DEFAULT;
	speed = SPEED_DEFAULT;
	
	// Since we are not going to change the hero sprite, we will use the spritesheet member of the sprite class to keep track of the sprite memory
	ALLEGRO_BITMAP* tmp_bmp = al_load_bitmap("dat/gfx/hero_ss.png");
	Game::checkInitialized(tmp_bmp, __FUNCTION__": failed to load dat/gfx/hero_ss.png");

	// Since we have 64x64 bitmap spritesheet and the sheet is just 1 row
	sprite->setBitmap(tmp_bmp, al_get_bitmap_height(tmp_bmp), al_get_bitmap_height(tmp_bmp), 3);

	// Set sprite position and clipping
	sprite->position.x = (bufferSize.cx / 2) - (sprite->size.cx / 2);
	sprite->position.y = (bufferSize.cy / 2) - (sprite->size.cy / 2);
	sprite->boundary.SetRect(0, 0, bufferSize.cx - 1, hm.HUDStartPosition.y - 1);
}

Hero::~Hero()
{
	al_destroy_bitmap(sprite->spriteSheet);

	delete sprite;
}

// Update hero location, add missiles and effects based on input
void Hero::update(bool moveLeft, bool moveRight, bool moveUp, bool moveDown, bool shoot, HUD& hm, Missiles& mm)
{
	// No need to update if we are out of lives
	if (hm.lives < 0)
		return;

	// Hero will come to life after some time
	if (respawnTimer)
	{
		respawnTimer--;
		return;
	}

	// Make the hero move based on input
	if (moveLeft)
		sprite->position.x -= speed;
	if (moveRight)
		sprite->position.x += speed;
	if (moveUp)
		sprite->position.y -= speed;
	if (moveDown)
		sprite->position.y += speed;

	// Make the sprite logic go
	sprite->update();

	if (invincibleTimer)
		invincibleTimer--;

	// Check if we can shoot and shoot if we have correct user input
	if (shotTimer)
		shotTimer--;
	else if (shoot)
	{
		int x = sprite->position.x + (sprite->size.cx / 2);
		if (mm.add(mm.HERO, true, x - 24, sprite->position.y + 21) && mm.add(mm.HERO, true, x + 24, sprite->position.y + 21))
			shotTimer = SHOT_TIMER_DEFAULT;
	}
}

// This is called by the collision detector if the hero collided with something
void Hero::hit(bool critical, HUD& hm, Effects& fm)
{
	// Nothing should happen to our hero is below conditions are met
	if (hm.lives < 0 || respawnTimer || invincibleTimer)
		return;

	if (critical)
		hm.shield = 0;
	else
		hm.shield -= 2;

	if (hm.shield <= 0 && hm.lives >= 0)
	{
		// Queue in the explosion if hero lost a life
		fm.add(fm.EXPLOSION_SMALL, sprite->position.x + (sprite->size.cx / 2), sprite->position.y + (sprite->size.cy / 2));

		hm.lives--;
		if (hm.lives >= 0)
			hm.shield = HUD::SHIELD_MAX;

		respawnTimer = RESPAWN_TIMER_DEFAULT;
		invincibleTimer = INVINCIBLE_TIMER_DEFAULT;
	}
}

void Hero::draw(HUD& hm)
{
	// Don't draw if below conditions are not met
	if (hm.lives < 0 || respawnTimer || (((invincibleTimer / 2) % 3) == 1))
		return;

	sprite->draw();
}
