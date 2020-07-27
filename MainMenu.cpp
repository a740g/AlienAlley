#include "MainMenu.h"

MainMenu::MainMenu(int screenWidth, int screenHeight)
{
	bufferWidth = screenWidth;
	bufferHeight = screenHeight;
	bitmap = NULL;
	bitmapWidth = bitmapHeight = 0;
}

MainMenu::~MainMenu()
{
	if (bitmap != NULL) al_destroy_bitmap(bitmap);
}

// Displays the introduction credits
void MainMenu::drawIntroCreditsScreen()
{
	if (bitmap != NULL)
	{
		al_destroy_bitmap(bitmap);
		bitmap = NULL;
		bitmapWidth = bitmapHeight = 0;
	}

	// Clear the screen
	al_clear_to_color(al_map_rgb(0, 0, 0));

	// First page of stuff
	bitmap = al_load_bitmap("dat/gfx/intro_screen1.png");
	InitializeCheck(bitmap, "dat/gfx/intro_screen1.png");
	bitmapWidth = al_get_bitmap_width(bitmap);
	bitmapHeight = al_get_bitmap_height(bitmap);

	// TODO: draw without being seen

	// Stretch bitmap to fill the screen
	al_draw_scaled_bitmap(bitmap, 0, 0, bitmapWidth, bitmapHeight, 0, 0, bufferWidth, bufferHeight, 0);

	al_destroy_bitmap(bitmap);
	bitmap = NULL;
	bitmapWidth = bitmapHeight = 0;

	FadeIn();
	FadeOut();

	// Clear the screen
	al_clear_to_color(al_map_rgb(0, 0, 0));

	// Second page of stuff
	bitmap = al_load_bitmap("dat/gfx/intro_screen2.png");
	InitializeCheck(bitmap, "dat/gfx/intro_screen2.png");
	bitmapWidth = al_get_bitmap_width(bitmap);
	bitmapHeight = al_get_bitmap_height(bitmap);

	// TODO: draw without being seen

	// Stretch bitmap to fill the screen
	al_draw_scaled_bitmap(bitmap, 0, 0, bitmapWidth, bitmapHeight, 0, 0, bufferWidth, bufferHeight, 0);

	al_destroy_bitmap(bitmap);
	bitmap = NULL;
	bitmapWidth = bitmapHeight = 0;

	FadeIn();
	FadeOut();
}

// Displays the Alien Alley title page
void MainMenu::drawTitleScreen()
{
	// Clear the screen
	al_clear_to_color(al_map_rgb(0, 0, 0));

	bitmap = al_load_bitmap("dat/gfx/title_screen.png");
	InitializeCheck(bitmap, "dat/gfx/title_screen.png");
	bitmapWidth = al_get_bitmap_width(bitmap);
	bitmapHeight = al_get_bitmap_height(bitmap);

	// TODO: draw without being seen

	// Stretch bitmap to fill the screen
	al_draw_scaled_bitmap(bitmap, 0, 0, bitmapWidth, bitmapHeight, 0, 0, bufferWidth, bufferHeight, 0);

	al_destroy_bitmap(bitmap);
	bitmap = NULL;
	bitmapWidth = bitmapHeight = 0;

	FadeIn();
}


// Returns the user input on the title screen
int MainMenu::userInput()
{
    // TODO: Add your implementation code here.
    return 0;
}


void MainMenu::FadeIn()
{
	// TODO: Add your implementation code here.
}


void MainMenu::FadeOut()
{
	// TODO: Add your implementation code here.
}
