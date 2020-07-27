#pragma once

#include "AlienAlley.h"

class CelestialObjects
{
public:

	CelestialObjects(int screenWidth, int screenHeight);	// initialize celestial objects
	~CelestialObjects();									// cleanup
	void update();											// update positions
	void draw();											// draw objects

private:

	int objectCount = 0;	// total number of objects on the screen
	int bufferWidth = 0;
	int bufferHeight = 0;
	int maxSpriteHeight = 0;

	// Type of objects
	enum ObjectTypes
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