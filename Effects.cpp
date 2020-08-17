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

// --- fx ---

FX_T fx[FX_N];

void fx_init()
{
	for (int i = 0; i < FX_N; i++)
		fx[i].used = false;
}

void fx_add(bool spark, int x, int y)
{
	if (!spark)
		al_play_sample(sample_explode[between(0, 2)], 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, nullptr);

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
