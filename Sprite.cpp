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

#include "Sprite.h"

Sprite::Sprite(ALLEGRO_BITMAP* ssBitmap, int width, int height, int frameSkip)
{
	setBitmap(ssBitmap, width, height, frameSkip);
}

void Sprite::reset()
{
	spriteSheet = nullptr;
	size.SetSize(0, 0);
	frames.SetSize(0, 0);
	currentFrame = 0;
	animationDirection = 0;
	playCount = 0;
	frameSkipMax = 0;
	frameSkipCount = 0;
	boundary.SetRectEmpty();
}

bool Sprite::setBitmap(ALLEGRO_BITMAP* ssBitmap, int width, int height, int frameSkip)
{
	if (ssBitmap == nullptr) return false;

	reset();											// reset all sprite properties
	spriteSheet = ssBitmap;

	// Check if we have a sprite sheet
	if (width <= 0 && height <= 0)
	{
		size.SetSize(al_get_bitmap_width(spriteSheet), al_get_bitmap_height(spriteSheet));
		frames.SetSize(1, 1);							// single image
		animationDirection = 0;							// no animation
	}
	else
	{
		if (width <= 0) width = height;
		if (height <= 0) height = width;
		size.SetSize(width, height);
		frames.SetSize(al_get_bitmap_width(spriteSheet) / width, al_get_bitmap_height(spriteSheet) / height);
		animationDirection = 1;							// forward animation
	}

	frameSkipMax = frameSkip;

	return true;
}

// Return true if s collides with *this
bool Sprite::collidesWith(const Sprite& s)
{
	// Sanity check
	if (spriteSheet == nullptr || s.spriteSheet == nullptr) return false;

	// Create bounding rectangle for *this bitmap
	CRect r1(position, size);
	// Create bounding rectangle for s bitmap
	CRect r2(s.position, s.size);

	// Find the intersecting rectangle and leave if the rectangles do not collide
	if ((r1 & r2).IsRectNull()) return false;

	// We have a coliision
	return true;
}

void Sprite::draw(int flags)
{
	if (spriteSheet == nullptr) return;

	al_draw_bitmap_region(spriteSheet, (currentFrame % frames.cx) * size.cx, (currentFrame / frames.cx) * size.cy, size.cx, size.cy, position.x, position.y, flags);
}

void Sprite::update()
{
	// Adjust position to bounding box if we have one
	if (!boundary.IsRectEmpty())
	{
		position.x = min(max(position.x, boundary.left), boundary.right - size.cx);
		position.y = min(max(position.y, boundary.top), boundary.bottom - size.cy);
	}

	// Skip frames if user has set frameSkip
	frameSkipCount++;
	if (frameSkipCount < frameSkipMax) return;

	// Else, we process the rest of the update cycle
	frameSkipCount = 0;
	currentFrame += animationDirection;

	if (currentFrame >= frames.cx * frames.cy)
	{
		currentFrame = 0;
		playCount++;
	}
	if (currentFrame < 0)
	{
		currentFrame = frames.cx * frames.cy - 1;
		playCount++;
	}
}

bool Sprite::setFrame(int n)
{
	if (n >= frames.cx * frames.cy || n < 0) return false;

	currentFrame = n;
	return true;
}
