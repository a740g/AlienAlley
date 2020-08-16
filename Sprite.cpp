#include "Sprite.h"

Sprite::Sprite(const char* fname, int width, int height, int rows, int columns)
{
	frames = 0;
	currentFrame = 0;
	animationDirection = 0;

	if (fname == nullptr) return;

	loadBitmap(fname, width, height, rows, columns);
}

Sprite::~Sprite()
{
	reset();
}

bool Sprite::loadBitmap(const char* fname, int width, int height, int rows, int columns)
{
	return false;
}

bool Sprite::collidesWith(const Sprite& s)
{
	return false;
}

void Sprite::draw(int flags)
{
}

void Sprite::nextFrame()
{
	currentFrame += animationDirection;

	if (currentFrame >= frames) currentFrame = 0;
	if (currentFrame < 0) currentFrame = frames - 1;
}

bool Sprite::setFrame(int n)
{
	if (n >= frames || n < 0) return false;

	currentFrame = n;
	return true;
}

void Sprite::reset()
{
}
