#pragma once

#include "AlienAlley.h"

class CelestialObjects
{
public:

	CelestialObjects();				// initialize celestial objects
	~CelestialObjects();			// cleanup
	void update(bool isWarping);	// update positions
	void draw();					// draw objects

private:

	int objectCount = 0;	// total number of objects on the screen
	int bufferWidth = 0;
	int bufferHeight = 0;
	int maxSpriteHeight = 0;

	// Type of objects
	enum
	{
		STAR = 0,
		GALAXY_1,
		GALAXY_2,
		GALAXY_3,
		GALAXY_4,
		PLANET_1,
		PLANET_2,
		PLANET_3,
		PLANET_4,
		MOON_1,
		MOON_2,
		OBJECT_TYPE_COUNT
	};

	// This matches with the object types above
	const float objectSpeed[OBJECT_TYPE_COUNT] = {0.5f, 0.2f, 0.2f, 0.2f, 0.2f, 0.8f, 0.8f, 0.8f, 0.8f, 1.1f, 1.1f};

	// This will hold the sprite bitmap and information
	struct ObjectSprite
	{
		ALLEGRO_BITMAP* bitmap;
		int width;
		int height;
	} objectSprite[OBJECT_TYPE_COUNT];

	// This will hold information of the celestial object
	struct Object
	{
		float y;
		float speed;
		int objectType;

	}* object = NULL;
};