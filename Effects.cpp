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

#include "Effects.h"

Effects::Effects()
{
	// Clear the effects array
	for (int i = 0; i < FX_N; i++)
	{
		fx[i].sprite = new Sprite();
		fx[i].used = false;
		fx[i].type = SPARKS;
	}

	// Initialize the buffer width and height
	bufferSize.SetSize(al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display()));

	// Load the effects sprite sheets
	spriteSheet[SPARKS] = al_load_bitmap("dat/gfx/sparks_ss.png");
	Game::checkInitialized(spriteSheet[SPARKS], __FUNCTION__": failed to load dat/gfx/sparks_ss.png");
	spriteSheet[EXPLOSION_TINY] = al_load_bitmap("dat/gfx/explosion_tiny_ss.png");
	Game::checkInitialized(spriteSheet[EXPLOSION_TINY], __FUNCTION__": failed to load dat/gfx/explosion_tiny_ss.png");
	spriteSheet[EXPLOSION_SMALL] = al_load_bitmap("dat/gfx/explosion_small_ss.png");
	Game::checkInitialized(spriteSheet[EXPLOSION_SMALL], __FUNCTION__": failed to load dat/gfx/explosion_small_ss.png");
	spriteSheet[EXPLOSION_BIG] = al_load_bitmap("dat/gfx/explosion_big_ss.png");
	Game::checkInitialized(spriteSheet[EXPLOSION_BIG], __FUNCTION__": failed to load dat/gfx/explosion_big_ss.png");

	// Load audio effects
	sample[SPARKS] = al_load_sample("dat/snd/sfx/hit.flac");
	Game::checkInitialized(sample[SPARKS], __FUNCTION__": failed to load dat/snd/sfx/hit.flac");
	sample[EXPLOSION_TINY] = al_load_sample("dat/snd/sfx/explosion_tiny.flac");
	Game::checkInitialized(sample[EXPLOSION_TINY], __FUNCTION__": failed to load dat/snd/sfx/explosion_tiny.flac");
	sample[EXPLOSION_SMALL] = al_load_sample("dat/snd/sfx/explosion_small.flac");
	Game::checkInitialized(sample[EXPLOSION_SMALL], __FUNCTION__": failed to load dat/snd/sfx/explosion_small.flac");
	sample[EXPLOSION_BIG] = al_load_sample("dat/snd/sfx/explosion_big.flac");
	Game::checkInitialized(sample[EXPLOSION_BIG], __FUNCTION__": failed to load dat/snd/sfx/explosion_big.flac");
}

Effects::~Effects()
{
	// Free all bitmaps and samples
	for (int i = 0; i < EFFECTS_TYPE_COUNT; i++)
	{
		al_destroy_sample(sample[i]);
		al_destroy_bitmap(spriteSheet[i]);
	}

	for (int i = 0; i < FX_N; i++)
		delete fx[i].sprite;
}

bool Effects::add(unsigned int type, int x, int y)
{
	int slot;

	// Sanity check
	if (type >= EFFECTS_TYPE_COUNT) return false;

	// Search for an unused effects slot
	for (slot = 0; slot < FX_N; slot++)
		if (!fx[slot].used) break;

	// Check if we are out of effects slots
	if (slot >= FX_N) return false;

	// Pan and play the effects sound. TODO: Special handling for SPARKS > lower gain
	al_play_sample(sample[type], 1, (2.0f * (float)x / (float)(bufferSize.cx - 1)) - 1.0f, 1, ALLEGRO_PLAYMODE_ONCE, nullptr);

	fx[slot].type = type;
	fx[slot].used = true;
	// Special handling for spark
	if (type == SPARKS)
		fx[slot].sprite->setBitmap(spriteSheet[type], spriteSheetSize[type][0], spriteSheetSize[type][1], 5);	// skip two frames for update
	else
		fx[slot].sprite->setBitmap(spriteSheet[type], spriteSheetSize[type][0], spriteSheetSize[type][1]);
	// Center the sprite
	fx[slot].sprite->position.x = x - spriteSheetSize[type][0] / 2;
	fx[slot].sprite->position.y = y - spriteSheetSize[type][1] / 2;

	return true;
}

void Effects::update()
{
	for (int i = 0; i < FX_N; i++)
	{
		if (!fx[i].used)
			continue;

		fx[i].sprite->update();

		if (fx[i].sprite->playCount > 0)
			fx[i].used = false;
	}
}

void Effects::draw()
{
	for (int i = 0; i < FX_N; i++)
	{
		if (!fx[i].used)
			continue;

		fx[i].sprite->draw();
	}
}
