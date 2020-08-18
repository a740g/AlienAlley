///////////////////////////////////////////////////////////////////////
//     _    _ _                 _    _ _
//    / \  | (_) ___ _ __      / \  | | | ___ _   _ 
//   / _ \ | | |/ _ \ '_ \    / _ \ | | |/ _ \ | | |
//  / ___ \| | |  __/ | | |  / ___ \| | |  __/ |_| |
// /_/   \_\_|_|\___|_| |_| /_/   \_\_|_|\___|\__, |
//                                            |___/
//
//  Conversion/port copyright � Samuel Gomes & Neil Gomes, 1998-2020.
//
///////////////////////////////////////////////////////////////////////

#include "MainMenu.h"

MainMenu::MainMenu()
{
	display = al_get_current_display();
	bufferWidth = al_get_display_width(display);
	bufferHeight = al_get_display_height(display);
}

// Displays the Alien Alley title page
void MainMenu::drawTitleScreen()
{
	// Clear the screen
	al_clear_to_color(al_map_rgb(0, 0, 0));

	ALLEGRO_BITMAP* bitmap = al_load_bitmap("dat/gfx/title_screen.png");
	InitializeCheck(bitmap, "dat/gfx/title_screen.png");
	int bitmapWidth = al_get_bitmap_width(bitmap);
	int bitmapHeight = al_get_bitmap_height(bitmap);

	// Stretch bitmap to fill the screen
	// We scale the pictures keeping aspect ratio intact and nothing is cropped
	float factor_x = (float)bufferWidth / (float)bitmapWidth;
	float factor_y = (float)bufferHeight / (float)bitmapHeight;
	float factor = (factor_y < factor_x) ? factor_y : factor_x;
	float w = (float)bitmapWidth * factor;
	float h = (float)bitmapHeight * factor;
	float x = ((float)bufferWidth / 2.0f) - (w / 2.0f);
	float y = ((float)bufferHeight / 2.0f) - (h / 2.0f);
	al_draw_scaled_bitmap(bitmap, 0, 0, bitmapWidth, bitmapHeight, x, y, w, h, 0);

	al_destroy_bitmap(bitmap);

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
	InitializeCheck(tempBitmap, __FUNCTION__": failed to get video memory bitmap");

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
	InitializeCheck(tempBitmap, __FUNCTION__": failed to get video memory bitmap");

	for (int a = 0; a < 256; a++)
	{
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_draw_tinted_bitmap(tempBitmap, al_map_rgba(255 - a, 255 - a, 255 - a, a), 0, 0, 0);
		al_flip_display();
		al_rest(0.005);
	}

	al_destroy_bitmap(tempBitmap);
}
