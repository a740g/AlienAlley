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

#include "CelestialObjects.h"

CelestialObjects::CelestialObjects()
{
	// Initialize the buffer width and height
	bufferSize.SetSize(al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display()));

	// How many objects do we want on the screen
	objectCount = bufferSize.cx / 2;
	
	// Allocate space for the objects in memory
	object = new Object[objectCount];

	// Load game assets

	// First one is a star and just a pixel that is rendered using primitives
	objectSprite[STAR].bitmap = nullptr;
	objectSprite[STAR].size.cx = 1;
	objectSprite[STAR].size.cy = 1;

	// Load the remaining sprites
	objectSprite[GALAXY_1].bitmap = al_load_bitmap("dat/gfx/galaxy1.png");
	Game::checkInitialized(objectSprite[GALAXY_1].bitmap, __FUNCTION__": failed to load dat/gfx/galaxy1.png");
	objectSprite[GALAXY_1].size.cx = al_get_bitmap_width(objectSprite[GALAXY_1].bitmap);
	objectSprite[GALAXY_1].size.cy = al_get_bitmap_height(objectSprite[GALAXY_1].bitmap);

	objectSprite[GALAXY_2].bitmap = al_load_bitmap("dat/gfx/galaxy2.png");
	Game::checkInitialized(objectSprite[GALAXY_2].bitmap, __FUNCTION__": failed to load dat/gfx/galaxy2.png");
	objectSprite[GALAXY_2].size.cx = al_get_bitmap_width(objectSprite[GALAXY_2].bitmap);
	objectSprite[GALAXY_2].size.cy = al_get_bitmap_height(objectSprite[GALAXY_2].bitmap);

	objectSprite[GALAXY_3].bitmap = al_load_bitmap("dat/gfx/galaxy3.png");
	Game::checkInitialized(objectSprite[GALAXY_3].bitmap, __FUNCTION__": failed to load dat/gfx/galaxy3.png");
	objectSprite[GALAXY_3].size.cx = al_get_bitmap_width(objectSprite[GALAXY_3].bitmap);
	objectSprite[GALAXY_3].size.cy = al_get_bitmap_height(objectSprite[GALAXY_3].bitmap);

	objectSprite[GALAXY_4].bitmap = al_load_bitmap("dat/gfx/galaxy4.png");
	Game::checkInitialized(objectSprite[GALAXY_4].bitmap, __FUNCTION__": failed to load dat/gfx/galaxy4.png");
	objectSprite[GALAXY_4].size.cx = al_get_bitmap_width(objectSprite[GALAXY_4].bitmap);
	objectSprite[GALAXY_4].size.cy = al_get_bitmap_height(objectSprite[GALAXY_4].bitmap);

	objectSprite[MOON_1].bitmap = al_load_bitmap("dat/gfx/moon1.png");
	Game::checkInitialized(objectSprite[MOON_1].bitmap, __FUNCTION__": failed to load dat/gfx/moon1.png");
	objectSprite[MOON_1].size.cx = al_get_bitmap_width(objectSprite[MOON_1].bitmap);
	objectSprite[MOON_1].size.cy = al_get_bitmap_height(objectSprite[MOON_1].bitmap);

	objectSprite[MOON_2].bitmap = al_load_bitmap("dat/gfx/moon2.png");
	Game::checkInitialized(objectSprite[MOON_2].bitmap, __FUNCTION__": failed to load dat/gfx/moon2.png");
	objectSprite[MOON_2].size.cx = al_get_bitmap_width(objectSprite[MOON_2].bitmap);
	objectSprite[MOON_2].size.cy = al_get_bitmap_height(objectSprite[MOON_2].bitmap);

	objectSprite[PLANET_1].bitmap = al_load_bitmap("dat/gfx/planet1.png");
	Game::checkInitialized(objectSprite[PLANET_1].bitmap, __FUNCTION__": failed to load dat/gfx/planet1.png");
	objectSprite[PLANET_1].size.cx = al_get_bitmap_width(objectSprite[PLANET_1].bitmap);
	objectSprite[PLANET_1].size.cy = al_get_bitmap_height(objectSprite[PLANET_1].bitmap);

	objectSprite[PLANET_2].bitmap = al_load_bitmap("dat/gfx/planet2.png");
	Game::checkInitialized(objectSprite[PLANET_2].bitmap, __FUNCTION__": failed to load dat/gfx/planet2.png");
	objectSprite[PLANET_2].size.cx = al_get_bitmap_width(objectSprite[PLANET_2].bitmap);
	objectSprite[PLANET_2].size.cy = al_get_bitmap_height(objectSprite[PLANET_2].bitmap);

	objectSprite[PLANET_3].bitmap = al_load_bitmap("dat/gfx/planet3.png");
	Game::checkInitialized(objectSprite[PLANET_3].bitmap, __FUNCTION__": failed to load dat/gfx/planet3.png");
	objectSprite[PLANET_3].size.cx = al_get_bitmap_width(objectSprite[PLANET_3].bitmap);
	objectSprite[PLANET_3].size.cy = al_get_bitmap_height(objectSprite[PLANET_3].bitmap);

	objectSprite[PLANET_4].bitmap = al_load_bitmap("dat/gfx/planet4.png");
	Game::checkInitialized(objectSprite[PLANET_4].bitmap, __FUNCTION__": failed to load dat/gfx/planet4.png");
	objectSprite[PLANET_4].size.cx = al_get_bitmap_width(objectSprite[PLANET_4].bitmap);
	objectSprite[PLANET_4].size.cy = al_get_bitmap_height(objectSprite[PLANET_4].bitmap);

	// Find the maximum object sprite height. This will help us calculate how much offscreen do we need to accout for the sprite to "properly" come in & out of view
	maxSpriteHeight = 0;
	for (int i = 0; i < OBJECT_TYPE_COUNT; i++)
	{
		if (maxSpriteHeight < objectSprite[i].size.cy) maxSpriteHeight = objectSprite[i].size.cy;
	}

	// Initialize all objects
	for (int i = 0; i < objectCount; i++)
	{
		object[i].y = Game::between(-(float)maxSpriteHeight, (float)bufferSize.cy + (float)maxSpriteHeight);
		// Force the object to be a star more times than not
		if (rand() % UCHAR_MAX == 0) {
			object[i].objectType = Game::between(STAR + 1, OBJECT_TYPE_COUNT - 1);
			object[i].speed = Game::between(OBJECT_SPEED[object[i].objectType] - 0.1f, OBJECT_SPEED[object[i].objectType] + 0.1f);
		}
		else
		{
			object[i].objectType = STAR;
			object[i].speed = Game::between(OBJECT_SPEED[STAR] - 0.1f, OBJECT_SPEED[STAR] + 0.1f);
		}
	}
}

CelestialObjects::~CelestialObjects()
{
	// Free all the sprites
	for (int i = 0; i < OBJECT_TYPE_COUNT; i++)
	{
		al_destroy_bitmap(objectSprite[i].bitmap);
	}

	// Free the object array
	delete [] object;
}

void CelestialObjects::update(bool isWarping)
{
	for (int i = 0; i < objectCount; i++)
	{
		object[i].y += object[i].speed * (isWarping ? 2 : 1);
		if (object[i].y >= bufferSize.cy + maxSpriteHeight)
		{
			object[i].y = -maxSpriteHeight;
			// Force the object to be a star more times than not
			if (rand() % UCHAR_MAX == 0) {
				object[i].objectType = Game::between(STAR + 1, OBJECT_TYPE_COUNT - 1);
				object[i].speed = Game::between(OBJECT_SPEED[object[i].objectType] - 0.1f, OBJECT_SPEED[object[i].objectType] + 0.1f);
			}
			else
			{
				object[i].objectType = STAR;
				object[i].speed = Game::between(OBJECT_SPEED[STAR] - 0.1f, OBJECT_SPEED[STAR] + 0.1f);
			}
		}
	}
}

void CelestialObjects::draw()
{
	float objectX = 1.5f;

	for (int i = 0; i < objectCount; i++)
	{
		if (object[i].objectType > STAR && object[i].objectType < OBJECT_TYPE_COUNT)
		{
			al_draw_bitmap(objectSprite[object[i].objectType].bitmap, objectX - objectSprite[object[i].objectType].size.cx / 2, object[i].y, 0);
		}
		else
		{
			// scale the object speed: b0 + (b1 - b0) * ((a-a0)/(a1-a0))
			float l = 0.1f + (0.7f - 0.1f) * ((object[i].speed - 0.4f) / (0.6f - 0.4f));
			if (object[i].y >= 0 && object[i].y <= bufferSize.cy) al_draw_pixel(objectX, object[i].y, al_map_rgb_f(l + Game::between(0.0f, 0.3f), l + Game::between(0.0f, 0.3f), l + Game::between(0.0f, 0.3f)));
		}
		objectX += 2;
	}
}
