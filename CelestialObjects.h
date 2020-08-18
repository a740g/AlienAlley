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

#pragma once

#include "AlienAlley.h"

class CelestialObjects
{
private:

	int objectCount;				// total number of objects on the screen
	int bufferWidth;
	int bufferHeight;
	int maxSpriteHeight;

	// Type of objects
	const enum
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
	const float OBJECT_SPEED[OBJECT_TYPE_COUNT] = { 0.5f, 0.2f, 0.2f, 0.2f, 0.2f, 0.8f, 0.8f, 0.8f, 0.8f, 1.1f, 1.1f };

	// This will hold the sprite bitmap and information
	struct
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

	}* object;

public:

	CelestialObjects();				// initialize celestial objects
	~CelestialObjects();			// cleanup
	void update(bool isWarping);	// update positions
	void draw();					// draw objects
};