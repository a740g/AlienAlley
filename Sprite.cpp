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

// Return true if s collides (pixel perfect) with *this
bool Sprite::collidesWith(const Sprite& s)
{
	// Sanity check
	if (frames <= 0 || s.frames <= 0 || frameList[currentFrame] == nullptr || s.frameList[s.currentFrame] == nullptr) return false;

	// Create bounding rectangle for *this bitmap
	CRect r1(position, size);
	// Create bounding rectangle for s bitmap
	CRect r2(s.position, s.size);

	// Find the intersecting rectangle
	CRect r = r1 & r2;
	// Leave if the rectangles do not collide
	if (r.IsRectNull()) return false;

	// Rectangles collide; do a detailed check

	// No collision
	return false;
#if 0
' Collision testing routine (works only for 8-bit bitmaps at the moment!)
Function CollisionTest(ByVal bmp1 As BITMAP Pointer, bmp1pt As POINT_T, ByVal bmp2 As BITMAP Pointer, bmp2pt As POINT_T) As Boolean
	Dim As RECT_T r1, r2, r
	Dim As Integer x1, y1, x2, y2, x1s, x2s, w, h, i, j

	' Create 'bmp1' bounding rectangle
	BoundingRectCompute(bmp1pt.x, bmp1pt.y, bmp1->w, bmp1->h, r1)

	' Create 'bmp2' bounding rectangle
	BoundingRectCompute(bmp2pt.x, bmp2pt.y, bmp2->w, bmp2->h, r2)

	' Leave if the rectangle do not collide
	If(r1.a.x > r2.b.x Or r2.a.x > r1.b.x Or r1.a.y > r2.b.y Or r2.a.y > r1.b.y) Then
		Return FALSE
	End If

	' Rectangle collision; do a detailed check
	' Find the intersecting rectangle
	r.a.x = IIf(r1.a.x > r2.a.x, r1.a.x, r2.a.x)
	r.a.y = IIf(r1.a.y > r2.a.y, r1.a.y, r2.a.y)
	r.b.x = IIf(r1.b.x < r2.b.x, r1.b.x, r2.b.x)
	r.b.y = IIf(r1.b.y < r2.b.y, r1.b.y, r2.b.y)

	' Find the width and height of the intersecting rectangle
	w = 1 + r.b.x - r.a.x
	h = 1 + r.b.y - r.a.y

	' Find the starting x & y positions of intersection in the bitmaps
	y1 = r.a.y - bmp1pt.y
	y2 = r.a.y - bmp2pt.y
	x1s = r.a.x - bmp1pt.x
	x2s = r.a.x - bmp2pt.x

	' Check both bitmap for collission
	For j = 0 To h - 1
		x1 = x1s
		x2 = x2s
	
		For i = 0 To w - 1
			If(_getpixel(bmp1, x1, y1) < > 0 AndAlso _getpixel(bmp2, x2, y2) < > 0) Then
				Return TRUE
			End If

			x1 += 1
			x2 += 1
		Next

		y1 += 1
		y2 += 1
	Next

	Return FALSE
End Function
#endif
}

void Sprite::draw(int flags)
{
	if (frames <= 0 || frameList[currentFrame] == nullptr) return;

	al_draw_bitmap(frameList[currentFrame], position.x, position.y, flags);
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
	if (frames <= 0) return;

	for (int i = 0; i < frames; i++)
	{
		if (frameList[i] != nullptr)
		{
			al_destroy_bitmap(frameList[i]);
			frameList[i] = nullptr;
		}
	}

	frameList.clear();
	frames = 0;
	currentFrame = 0;
	animationDirection = 0;
}
