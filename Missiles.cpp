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

#include "Missiles.h"

Missiles::Missiles()
{
	// Clear the shot array
	for (int i = 0; i < SHOTS_N; i++)
	{
		shot[i].sprite = new Sprite();
		shot[i].used = false;
		shot[i].dx = shot[i].dy = 0;
		shot[i].type = HERO;
	}

	// Initialize the buffer width and height
	bufferWidth = al_get_display_width(al_get_current_display());
	bufferHeight = al_get_display_height(al_get_current_display());

	// Load the missile sprite sheets
	spriteSheet[HERO] = al_load_bitmap("dat/gfx/missile_hero_ss.png");
	Game::checkInitialized(spriteSheet[HERO], "dat/gfx/missile_hero_ss.png");
	spriteSheet[ALIEN] = al_load_bitmap("dat/gfx/missile_alien_ss.png");
	Game::checkInitialized(spriteSheet[ALIEN], "dat/gfx/missile_alien_ss.png");

	// Load audio effects
	sample[HERO] = al_load_sample("dat/snd/sfx/missile_hero.flac");
	Game::checkInitialized(sample[HERO], "dat/snd/sfx/missile_hero.flac");
	sample[ALIEN] = al_load_sample("dat/snd/sfx/missile_alien.flac");
	Game::checkInitialized(sample[ALIEN], "dat/snd/sfx/missile_alien.flac");
}

Missiles::~Missiles()
{
	// Free all bitmaps and samples
	for (int i = 0; i < MISSILES_TYPE_COUNT; i++)
	{
		al_destroy_sample(sample[i]);
		al_destroy_bitmap(spriteSheet[i]);
	}

	for (int i = 0; i < SHOTS_N; i++)
		delete shot[i].sprite;
}

// Update all missile locations, sprites etc.
void Missiles::update()
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shot[i].used)
			continue;

		// Update the sprite first
		shot[i].sprite->update();

		switch (shot[i].type)
		{
		case HERO:
			//shot[i].sprite->position.x += shot[i].dx;		// not required. hero always shoots straight up
			shot[i].sprite->position.y += shot[i].dy;		// here dy should be negative

			// Remove the missile if it goes off screen
			if (shot[i].sprite->position.y < -(shot[i].sprite->size.cy))
			{
				shot[i].used = false;
			}

			break;

		case ALIEN:
			// Move missils in dx, dy directions
			shot[i].sprite->position.x += shot[i].dx;
			shot[i].sprite->position.y += shot[i].dy;

			// Remove the missile if it goes off screen
			if ((shot[i].sprite->position.x < -(shot[i].sprite->size.cx)) || (shot[i].sprite->position.x > bufferWidth) || (shot[i].sprite->position.y < -(shot[i].sprite->size.cy)) || (shot[i].sprite->position.y > bufferHeight))
			{
				shot[i].used = false;
			}

			break;

		default:
			Game::checkInitialized(false, __FUNCTION__": case not handled");
		}
	}
}

// Draw all missiles
void Missiles::draw()
{
	for (int i = 0; i < SHOTS_N; i++)
	{
		if (!shot[i].used)
			continue;

		shot[i].sprite->draw();
	}
}

// Add a missile from an object on the screen
bool Missiles::add(unsigned int type, bool straight, int x, int y)
{
	int slot;

	// Sanity check
	if (type >= MISSILES_TYPE_COUNT) return false;

	// Search for an unused effects slot
	for (slot = 0; slot < SHOTS_N; slot++)
		if (!shot[slot].used) break;

	// Check if we are out of missile slots
	if (slot >= SHOTS_N) return false;

	// Assign the sprite bitmap based on type
	shot[slot].sprite->setBitmap(spriteSheet[type], spriteSheetSize[type][0], spriteSheetSize[type][1], 3);		// skip 3 frames

	if (type == HERO)
	{
		shot[slot].sprite->position.x = x - (spriteSheetSize[type][0] / 2);		// center the missle
		shot[slot].sprite->position.y = y;

		shot[slot].dx = 0;												// straight up
		shot[slot].dy = -5;												// missile will travel 5 pixels to the top of the screen
	}
	else // alien
	{
		shot[slot].sprite->position.x = x - (spriteSheetSize[type][0] / 2);		// center the missle
		shot[slot].sprite->position.y = y - (spriteSheetSize[type][1] / 2);		// center the missie

		if (straight)
		{
			shot[slot].dx = 0;
			shot[slot].dy = 2;
		}
		else
		{

			shot[slot].dx = Game::between(-2, 2);
			shot[slot].dy = Game::between(-2, 2);
		}

		// if the shot has no speed, don't bother
		if (!shot[slot].dx && !shot[slot].dy)
			return true;
	}

	shot[slot].type = type;
	shot[slot].used = true;

	// Pan and play the effects sound. TODO: Centering must account sprite size here and also elsewhere
	al_play_sample(sample[type], 1, (2.0f * (float)x / (float)(bufferWidth - 1)) - 1.0f, 1, ALLEGRO_PLAYMODE_ONCE, nullptr);

	return true;
}
