#include "Sprite.h"

Sprite::Sprite(const char* fname, int width, int height, int rows, int columns)
{
}

Sprite::~Sprite()
{
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
}

bool Sprite::setFrame(int n)
{
	return false;
}

void Sprite::reset()
{
}
