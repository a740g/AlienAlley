#include "CelestialObjects.h"


CelestialObjects::CelestialObjects(int screenWidth, int screenHeight)
{
	// Initialize the buffer width and height
	bufferWidth = screenWidth;
	bufferHeight = screenHeight;

	// How many objects do we want on the screen
	objectCount = bufferWidth / 2;
	
	// Allocate space for the objects in memory
	object = new Object[objectCount];

	// Load game assets

	// First one is a star and just a pixel that is rendered using primitives
	objectSprite[STAR].bitmap = NULL;
	objectSprite[STAR].width = objectSprite[STAR].height = 1;

	// Load the remaining sprites
	objectSprite[GALAXY_1].bitmap = al_load_bitmap("dat/gfx/galaxy1.png");
	InitializeCheck(objectSprite[GALAXY_1].bitmap, "dat/gfx/galaxy1.png");
	objectSprite[GALAXY_1].width = al_get_bitmap_width(objectSprite[GALAXY_1].bitmap);
	objectSprite[GALAXY_1].height = al_get_bitmap_height(objectSprite[GALAXY_1].bitmap);

	objectSprite[GALAXY_2].bitmap = al_load_bitmap("dat/gfx/galaxy2.png");
	InitializeCheck(objectSprite[GALAXY_2].bitmap, "dat/gfx/galaxy2.png");
	objectSprite[GALAXY_2].width = al_get_bitmap_width(objectSprite[GALAXY_2].bitmap);
	objectSprite[GALAXY_2].height = al_get_bitmap_height(objectSprite[GALAXY_2].bitmap);

	objectSprite[GALAXY_3].bitmap = al_load_bitmap("dat/gfx/galaxy3.png");
	InitializeCheck(objectSprite[GALAXY_3].bitmap, "dat/gfx/galaxy3.png");
	objectSprite[GALAXY_3].width = al_get_bitmap_width(objectSprite[GALAXY_3].bitmap);
	objectSprite[GALAXY_3].height = al_get_bitmap_height(objectSprite[GALAXY_3].bitmap);

	objectSprite[GALAXY_4].bitmap = al_load_bitmap("dat/gfx/galaxy4.png");
	InitializeCheck(objectSprite[GALAXY_4].bitmap, "dat/gfx/galaxy4.png");
	objectSprite[GALAXY_4].width = al_get_bitmap_width(objectSprite[GALAXY_4].bitmap);
	objectSprite[GALAXY_4].height = al_get_bitmap_height(objectSprite[GALAXY_4].bitmap);

	objectSprite[MOON_1].bitmap = al_load_bitmap("dat/gfx/moon1.png");
	InitializeCheck(objectSprite[MOON_1].bitmap, "dat/gfx/moon1.png");
	objectSprite[MOON_1].width = al_get_bitmap_width(objectSprite[MOON_1].bitmap);
	objectSprite[MOON_1].height = al_get_bitmap_height(objectSprite[MOON_1].bitmap);

	objectSprite[MOON_2].bitmap = al_load_bitmap("dat/gfx/moon2.png");
	InitializeCheck(objectSprite[MOON_2].bitmap, "dat/gfx/moon2.png");
	objectSprite[MOON_2].width = al_get_bitmap_width(objectSprite[MOON_2].bitmap);
	objectSprite[MOON_2].height = al_get_bitmap_height(objectSprite[MOON_2].bitmap);

	objectSprite[PLANET_1].bitmap = al_load_bitmap("dat/gfx/planet1.png");
	InitializeCheck(objectSprite[PLANET_1].bitmap, "dat/gfx/planet1.png");
	objectSprite[PLANET_1].width = al_get_bitmap_width(objectSprite[PLANET_1].bitmap);
	objectSprite[PLANET_1].height = al_get_bitmap_height(objectSprite[PLANET_1].bitmap);

	objectSprite[PLANET_2].bitmap = al_load_bitmap("dat/gfx/planet2.png");
	InitializeCheck(objectSprite[PLANET_2].bitmap, "dat/gfx/planet2.png");
	objectSprite[PLANET_2].width = al_get_bitmap_width(objectSprite[PLANET_2].bitmap);
	objectSprite[PLANET_2].height = al_get_bitmap_height(objectSprite[PLANET_2].bitmap);

	objectSprite[PLANET_3].bitmap = al_load_bitmap("dat/gfx/planet3.png");
	InitializeCheck(objectSprite[PLANET_3].bitmap, "dat/gfx/planet3.png");
	objectSprite[PLANET_3].width = al_get_bitmap_width(objectSprite[PLANET_3].bitmap);
	objectSprite[PLANET_3].height = al_get_bitmap_height(objectSprite[PLANET_3].bitmap);

	objectSprite[PLANET_4].bitmap = al_load_bitmap("dat/gfx/planet4.png");
	InitializeCheck(objectSprite[PLANET_4].bitmap, "dat/gfx/planet4.png");
	objectSprite[PLANET_4].width = al_get_bitmap_width(objectSprite[PLANET_4].bitmap);
	objectSprite[PLANET_4].height = al_get_bitmap_height(objectSprite[PLANET_4].bitmap);

	// Find the maximum object sprite height. This will help us calculate how much offscreen do we need to accout for the sprite to "properly" come in & out of view
	for (int i = 0; i < OBJECT_TYPE_COUNT; i++)
	{
		if (maxSpriteHeight < objectSprite[i].height) maxSpriteHeight = objectSprite[i].height;
	}

	// Initialize all objects
	for (int i = 0; i < objectCount; i++)
	{
		object[i].y = between(-(float)maxSpriteHeight, (float)bufferHeight + (float)maxSpriteHeight);
		// Force the object to be a star more times than not
		if (rand() % UCHAR_MAX == 0) {
			object[i].objectType = between(STAR, OBJECT_TYPE_COUNT - 1);
			object[i].speed = 0.05f * (float)object[i].objectType;
		}
		else
		{
			object[i].speed = between(0.1f, 1.0f);
			object[i].objectType = STAR;
		}
	}
}

CelestialObjects::~CelestialObjects()
{
	// Free all the sprites
	for (int i = 0; i < OBJECT_TYPE_COUNT; i++)
	{
		if (objectSprite[i].bitmap != NULL) al_destroy_bitmap(objectSprite[i].bitmap);
	}

	// Free the object array
	delete [] object;
}

void CelestialObjects::update()
{
	for (int i = 0; i < objectCount; i++)
	{
		object[i].y += object[i].speed;
		if (object[i].y >= bufferHeight + maxSpriteHeight)
		{
			object[i].y = -maxSpriteHeight;
			// Force the object to be a star more times than not
			if (rand() % UCHAR_MAX == 0) {
				object[i].objectType = between(STAR, OBJECT_TYPE_COUNT - 1);
				object[i].speed = object[i].speed = 0.05f * (float)object[i].objectType;;
			}
			else
			{
				object[i].speed = between(0.1f, 1.0f);
				object[i].objectType = STAR;
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
			al_draw_bitmap(objectSprite[object[i].objectType].bitmap, objectX - objectSprite[object[i].objectType].width / 2, object[i].y, 0);
		}
		else
		{
			float l = object[i].speed * 0.8;
			if (object[i].y >= 0 && object[i].y <= bufferHeight) al_draw_pixel(objectX, object[i].y, al_map_rgb_f(l, l, l));
		}
		objectX += 2;
	}
}
