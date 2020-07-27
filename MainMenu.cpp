#include "MainMenu.h"

MainMenu::MainMenu()
{
	display = al_get_current_display();
	bufferWidth = al_get_display_width(display);
	bufferHeight = al_get_display_height(display);
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

	// Stretch bitmap to fill the screen
	drawScaledBitmap();

	al_destroy_bitmap(bitmap);
	bitmap = NULL;
	bitmapWidth = bitmapHeight = 0;

	fadeIn();
	fadeOut();

	// Clear the screen
	al_clear_to_color(al_map_rgb(0, 0, 0));

	// Second page of stuff
	bitmap = al_load_bitmap("dat/gfx/intro_screen2.png");
	InitializeCheck(bitmap, "dat/gfx/intro_screen2.png");
	bitmapWidth = al_get_bitmap_width(bitmap);
	bitmapHeight = al_get_bitmap_height(bitmap);

	// Stretch bitmap to fill the screen
	drawScaledBitmap();

	al_destroy_bitmap(bitmap);
	bitmap = NULL;
	bitmapWidth = bitmapHeight = 0;

	fadeIn();
	fadeOut();
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

	// Stretch bitmap to fill the screen
	drawScaledBitmap();

	al_destroy_bitmap(bitmap);
	bitmap = NULL;
	bitmapWidth = bitmapHeight = 0;

	fadeIn();
}


// Returns the user input on the title screen
int MainMenu::userInput()
{
    // TODO: Add your implementation code here.
    return 0;
}


void MainMenu::fadeIn()
{
	ALLEGRO_BITMAP* tempBitmap = al_clone_bitmap(al_get_backbuffer(display));

	for (int a = 0; a < 256; a++)
	{
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_tinted_bitmap(tempBitmap, al_map_rgba(a, a, a, a), 0, 0, 0);
		al_flip_display();
		al_rest(0.005);
	}

	al_destroy_bitmap(tempBitmap);
}


void MainMenu::fadeOut()
{
	ALLEGRO_BITMAP* tempBitmap = al_clone_bitmap(al_get_backbuffer(display));

	for (int a = 0; a < 256; a++)
	{
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_tinted_bitmap(tempBitmap, al_map_rgba(255 - a, 255 - a, 255 - a, a), 0, 0, 0);
		al_flip_display();
		al_rest(0.005);
	}

	al_destroy_bitmap(tempBitmap);
}

// We scale the pictures keeping aspect ratio intact
void MainMenu::drawScaledBitmap()
{
	float factor_x = (float)bufferWidth / (float)bitmapWidth;
	float factor_y = (float)bufferHeight / (float)bitmapHeight;
	float factor = (factor_y < factor_x) ? factor_y : factor_x;
	float w = (float)bitmapWidth * factor;
	float h = (float)bitmapHeight * factor;
	float x = ((float)bufferWidth / 2.0f) - (w / 2.0f);
	float y = ((float)bufferHeight / 2.0f) - (h / 2.0f);
	al_draw_scaled_bitmap(bitmap, 0, 0, bitmapWidth, bitmapHeight, x, y, w, h, 0);
}
