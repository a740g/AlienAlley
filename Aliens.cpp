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

#include "Aliens.h"

Aliens::Aliens()
{
	// Clear the alien array and create sprite objects
	for (int i = 0; i < ALIENS_N; i++)
	{
		alien[i].sprite = new Sprite();
		alien[i].used = false;
	}

	// Initialize the buffer width and height
	bufferWidth = al_get_display_width(al_get_current_display());
	bufferHeight = al_get_display_height(al_get_current_display());

	// Load the missile sprite sheets
	spriteSheet[BUG] = al_load_bitmap("dat/gfx/alien_bug.png");
	Game::checkInitialized(spriteSheet[BUG], "dat/gfx/alien_bug.png");

	spriteSheet[ARROW] = al_load_bitmap("dat/gfx/alien_arrow.png");
	Game::checkInitialized(spriteSheet[ARROW], "dat/gfx/alien_arrow.png");

	spriteSheet[VIPER] = al_load_bitmap("dat/gfx/alien_viper.png");
	Game::checkInitialized(spriteSheet[VIPER], "dat/gfx/alien_viper.png");

	spriteSheet[GIM72] = al_load_bitmap("dat/gfx/alien_gim72.png");
	Game::checkInitialized(spriteSheet[GIM72], "dat/gfx/alien_gim72.png");

	spriteSheet[HOMER] = al_load_bitmap("dat/gfx/alien_homer.png");
	Game::checkInitialized(spriteSheet[HOMER], "dat/gfx/alien_homer.png");

	spriteSheet[BOP] = al_load_bitmap("dat/gfx/alien_bop.png");
	Game::checkInitialized(spriteSheet[BOP], "dat/gfx/alien_bop.png");

	spriteSheet[EMAGO] = al_load_bitmap("dat/gfx/alien_emago.png");
	Game::checkInitialized(spriteSheet[EMAGO], "dat/gfx/alien_emago.png");

	spriteSheet[XTC] = al_load_bitmap("dat/gfx/alien_xtc.png");
	Game::checkInitialized(spriteSheet[XTC], "dat/gfx/alien_xtc.png");

	spriteSheet[THICCBOI] = al_load_bitmap("dat/gfx/alien_thiccboi.png");
	Game::checkInitialized(spriteSheet[THICCBOI], "dat/gfx/alien_thiccboi.png");
}

Aliens::~Aliens()
{
	// Free all bitmaps
	for (int i = 0; i < ALIEN_TYPE_COUNT; i++)
	{
		al_destroy_bitmap(spriteSheet[i]);
	}

	for (int i = 0; i < ALIENS_N; i++)
	{
		delete alien[i].sprite;
	}
}

// Run alien AI, update location & sprite
void Aliens::update(unsigned long frameCounter, HUD& hm, Missiles& mm, Effects& fm)
{
	int new_quota =	(frameCounter % 120) ? 0 : Game::between(2, 4);

	for (int i = 0; i < ALIENS_N; i++)
	{
		if (!alien[i].used)
		{
			// if this alien is unused, should it spawn?
			if (new_quota > 0)
			{
				// What is the alien type?
				alien[i].type = Game::between(BUG, ALIEN_TYPE_COUNT - 1);
				alien[i].sprite->setBitmap(spriteSheet[alien[i].type], spriteSheetSize[alien[i].type][0], spriteSheetSize[alien[i].type][1]);
				alien[i].sprite->position.x = Game::between(-spriteSheetSize[alien[i].type][0], bufferWidth);
				alien[i].sprite->position.y = -spriteSheetSize[alien[i].type][1];
				
				alien[i].shotTimer = Game::between(1, 99);
				alien[i].blinkTimer = 0;
				alien[i].used = true;

				// How resilient are these aliens to attacks?
				switch (alien[i].type)
				{
				case BUG:
					alien[i].life = 1;
					break;

				case ARROW:
					alien[i].life = 2;
					break;

				case VIPER:
					alien[i].life = 2;
					break;

				case GIM72:
					alien[i].life = 4;
					break;

				case HOMER:
					alien[i].life = 4;
					break;

				case BOP:
					alien[i].life = 6;
					break;

				case EMAGO:
					alien[i].life = 8;
					break;

				case XTC:
					alien[i].life = 10;
					break;

				case THICCBOI:
					alien[i].life = 12;
					break;
				}

				new_quota--;
			}
			continue;
		}

		// Check direction movement and set direction
		if (alien[i].directionTimer > 0)
			alien[i].directionTimer--;
		else
		{
			alien[i].directionTimer = 20 + rand() % 51;
			alien[i].dx = Game::between(-1, 1);
		}

		// Alien specific movement behaviors
		switch (alien[i].type)
		{
		case BUG:
			if (frameCounter % 3)
			{
				alien[i].sprite->position.y++;
				alien[i].sprite->position.x += alien[i].dx;
			}
			break;

		case ARROW:
			alien[i].sprite->position.y++;
			alien[i].sprite->position.x += alien[i].dx;

			break;

		case VIPER:
			alien[i].sprite->position.y += 2;
			alien[i].sprite->position.x += alien[i].dx * 2;
			break;

		case GIM72:
			alien[i].sprite->position.y++;
			alien[i].sprite->position.x += alien[i].dx;
			break;

		case HOMER:
			if (frameCounter % 2)
			{
				alien[i].sprite->position.y++;
				alien[i].sprite->position.x += alien[i].dx;
			}
			break;

		case BOP:
			alien[i].sprite->position.y++;
			alien[i].sprite->position.x += alien[i].dx;
			break;

		case EMAGO:
			alien[i].sprite->position.y++;
			alien[i].sprite->position.x += alien[i].dx;
			break;

		case XTC:
			alien[i].sprite->position.y++;
			alien[i].sprite->position.x += alien[i].dx;
			break;

		case THICCBOI:
			if (!(frameCounter % 4))
			{
				alien[i].sprite->position.y++;
				alien[i].sprite->position.x += alien[i].dx;
			}
			break;
		}

		// Clip alien movement horizontally
		if (alien[i].sprite->position.x < -spriteSheetSize[alien[i].type][0]) alien[i].sprite->position.x = -spriteSheetSize[alien[i].type][0];
		if (alien[i].sprite->position.x > bufferWidth) alien[i].sprite->position.x = bufferWidth;

		// Remove the alien if it crosses the bottom of the sceeen
		if (alien[i].sprite->position.y >= bufferWidth)
		{
			alien[i].used = false;
			continue;
		}

		if (alien[i].blinkTimer)
			alien[i].blinkTimer--;

		// NOTE: Moved collision detection to Game class

		// Calculate midpoints of alien sprites
		int cx = alien[i].sprite->position.x + (spriteSheetSize[alien[i].type][0] / 2);
		int cy = alien[i].sprite->position.y + (spriteSheetSize[alien[i].type][1] / 2);

		// Queue explosion if alien is completely destroyed
		if (alien[i].life <= 0)
		{
			switch (alien[i].type)
			{
			case BUG:
				hm.score += 20;
				fm.add(fm.EXPLOSION_TINY, cx, cy);
				break;

			case ARROW:
				hm.score += 15;
				fm.add(fm.EXPLOSION_TINY, cx, cy);
				break;

			case VIPER:
				hm.score += 25;
				fm.add(fm.EXPLOSION_TINY, cx, cy);
				break;

			case GIM72:
				hm.score += 35;
				fm.add(fm.EXPLOSION_SMALL, cx, cy);
				break;

			case HOMER:
				hm.score += 30;
				fm.add(fm.EXPLOSION_SMALL, cx, cy);
				break;

			case BOP:
				hm.score += 40;
				fm.add(fm.EXPLOSION_SMALL, cx, cy);
				break;

			case EMAGO:
				hm.score += 45;
				fm.add(fm.EXPLOSION_SMALL, cx, cy);
				break;

			case XTC:
				hm.score += 50;
				fm.add(fm.EXPLOSION_SMALL, cx, cy);
				break;

			case THICCBOI:
				hm.score += 80;
				fm.add(fm.EXPLOSION_BIG, cx, cy);
				break;
			}

			alien[i].used = false;
			continue;
		}

		// Check if the alien can shoot and then handle shooting per alien type
		alien[i].shotTimer--;
		if (alien[i].shotTimer == 0)
		{
			switch (alien[i].type)
			{
			case BUG:
				mm.add(mm.ALIEN, false, cx, cy);
				alien[i].shotTimer = 150;
				break;

			case ARROW:
				mm.add(mm.ALIEN, true, cx, alien[i].sprite->position.y);
				alien[i].shotTimer = 80;
				break;

			case VIPER:
				mm.add(mm.ALIEN, true, cx, alien[i].sprite->position.y);
				alien[i].shotTimer = 80;
				break;

			case GIM72:
				mm.add(mm.ALIEN, true, cx - 5, cy);
				mm.add(mm.ALIEN, true, cx + 5, cy);
				alien[i].shotTimer = 150;
				break;

			case HOMER:
				mm.add(mm.ALIEN, true, cx - 5, cy);
				mm.add(mm.ALIEN, true, cx + 5, cy);
				alien[i].shotTimer = 100;
				break;

			case BOP:
				mm.add(mm.ALIEN, true, cx - 5, cy);
				mm.add(mm.ALIEN, true, cx + 5, cy);
				alien[i].shotTimer = 100;
				break;

			case EMAGO:
				mm.add(mm.ALIEN, true, cx - 5, cy);
				mm.add(mm.ALIEN, true, cx + 5, cy);
				alien[i].shotTimer = 100;
				break;

			case XTC:
				mm.add(mm.ALIEN, true, cx - 5, cy);
				mm.add(mm.ALIEN, true, cx + 5, cy);
				alien[i].shotTimer = 100;
				break;

			case THICCBOI:
				mm.add(mm.ALIEN, true, cx - 5, cy);
				mm.add(mm.ALIEN, true, cx + 5, cy);
				mm.add(mm.ALIEN, true, cx - 5, cy + 8);
				mm.add(mm.ALIEN, true, cx + 5, cy + 8);
				alien[i].shotTimer = 200;
				break;
			}
		}
	}
}

// This is called by the collision detector if the alien collided with something
void Aliens::hit(int n, bool critical)
{
	// Sanity checks
	if (n < 0 || n >= ALIENS_N)
		Game::checkInitialized(false, __FUNCTION__": Alien index out of range");
	if (!alien[n].used)
		Game::checkInitialized(false, __FUNCTION__": Tried to used an inactive alien slot");

	if (critical)
		alien[n].life = 0;
	else
		alien[n].life--;
	
	alien[n].blinkTimer = 4;
}

// Draw aliens
void Aliens::draw()
{
	for (int i = 0; i < ALIENS_N; i++)
	{
		if (!alien[i].used)
			continue;
		if (alien[i].blinkTimer > 2)	// TODO: ???
			continue;

		alien[i].sprite->draw();
	}
}
