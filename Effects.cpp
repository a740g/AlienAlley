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
	bufferWidth = al_get_display_width(al_get_current_display());

	// Load the effects sprite sheets
	spriteSheet[SPARKS] = al_load_bitmap("dat/gfx/sparks_ss.png");
	InitializeCheck(spriteSheet[SPARKS], "dat/gfx/sparks_ss.png");
	spriteSheet[EXPLOSION_TINY] = al_load_bitmap("dat/gfx/explosion_tiny_ss.png");
	InitializeCheck(spriteSheet[EXPLOSION_TINY], "dat/gfx/explosion_tiny_ss.png");
	spriteSheet[EXPLOSION_SMALL] = al_load_bitmap("dat/gfx/explosion_small_ss.png");
	InitializeCheck(spriteSheet[EXPLOSION_SMALL], "dat/gfx/explosion_small_ss.png");
	spriteSheet[EXPLOSION_BIG] = al_load_bitmap("dat/gfx/explosion_big_ss.png");
	InitializeCheck(spriteSheet[EXPLOSION_BIG], "dat/gfx/explosion_big_ss.png");

	// Load audio effects
	sample[SPARKS] = al_load_sample("dat/snd/sfx/hit.flac");
	InitializeCheck(sample[SPARKS], "dat/snd/sfx/hit.flac");
	sample[EXPLOSION_TINY] = al_load_sample("dat/snd/sfx/explosion_tiny.flac");
	InitializeCheck(sample[EXPLOSION_TINY], "dat/snd/sfx/explosion_tiny.flac");
	sample[EXPLOSION_SMALL] = al_load_sample("dat/snd/sfx/explosion_small.flac");
	InitializeCheck(sample[EXPLOSION_SMALL], "dat/snd/sfx/explosion_small.flac");
	sample[EXPLOSION_BIG] = al_load_sample("dat/snd/sfx/explosion_big.flac");
	InitializeCheck(sample[EXPLOSION_BIG], "dat/snd/sfx/explosion_big.flac");
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

	// Pan and play the effects sound
	al_play_sample(sample[type], 1, (2.0f * (float)x / (float)(bufferWidth - 1)) - 1.0f, 1, ALLEGRO_PLAYMODE_ONCE, nullptr);

	fx[slot].type = type;
	fx[slot].used = true;
	// Special handling for spark
	if (type == SPARKS)
		fx[slot].sprite->setBitmap(spriteSheet[type], spriteSheetSize[type][0], spriteSheetSize[type][1], 1);	// skip one frame for update
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
