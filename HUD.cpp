#include "HUD.h"


HUD::HUD()
{
    // Initialize everything
    score = 0;
    displayScore = 0;
    lives = LIVES_MAX;
    shield = SHIELD_MAX;
    displayShield = SHIELD_MAX;

    // Initialize the buffer width and height
    bufferWidth = al_get_display_width(al_get_current_display());
    bufferHeight = al_get_display_height(al_get_current_display());

    // Load the game font
    font = al_create_builtin_font();
    InitializeCheck(font, "font");

    // Load game assets
    digitSpriteSheet = al_load_bitmap("dat/gfx/digits_ss.png");
    InitializeCheck(digitSpriteSheet, "dat/gfx/digits_ss.png");
    digitSpriteWidth = al_get_bitmap_width(digitSpriteSheet) / 10;          // for 10 digits
    digitSpriteHeight = al_get_bitmap_height(digitSpriteSheet);

    HUDBitmap = al_load_bitmap("dat/gfx/hud.png");
    InitializeCheck(HUDBitmap, "dat/gfx/hud.png");
    HUDStartX = bufferWidth / 2 - al_get_bitmap_width(HUDBitmap) / 2;       // center horizontally
    HUDStartY = bufferHeight - al_get_bitmap_height(HUDBitmap);             // align to the bottom

    lifeBitmap = al_load_bitmap("dat/gfx/life.png");
    InitializeCheck(lifeBitmap, "dat/gfx/life.png");
    lifeBitmapWidth = al_get_bitmap_width(lifeBitmap);
    lifeBitmapHeight = al_get_bitmap_height(lifeBitmap);
}

HUD::~HUD()
{
    al_destroy_font(font);
    al_destroy_bitmap(digitSpriteSheet);
    al_destroy_bitmap(HUDBitmap);
    al_destroy_bitmap(lifeBitmap);
}

// Updates all game HUD elements
void HUD::update(int gameScore, int gameLives, int gameShield)
{
    // First we save the game values and also do a sanity check
    score = (gameScore > SCORE_MAX ? SCORE_MAX : gameScore);
    lives = (gameLives > LIVES_MAX ? LIVES_MAX : gameLives);
    shield = (gameShield > SHIELD_MAX ? SHIELD_MAX : gameShield);

    // Now increment or decrement display* unil it reaches the real values
    // First Score
    if (displayScore < score)
    {
        displayScore++;
    }
    else if (displayScore > score)
    {
        displayScore--;
    }
    // Finally shield
    if (displayShield < shield)
    {
        displayShield++;
    }
    else if (displayShield > shield)
    {
        displayShield--;
    }
}


// Draws the game HUD on the framebuffer
void HUD::draw()
{
    int spacing, digit, i, j;

    // First draw the HUD panel
    al_draw_bitmap(HUDBitmap, HUDStartX, HUDStartY, 0);

    // Now draw the shield
    // draw full green bar
    //al_draw_filled_rectangle(SHIELD_STATUS_LEFT + HUDStartX, SHIELD_STATUS_TOP + HUDStartY, SHIELD_STATUS_RIGHT + HUDStartX, SHIELD_STATUS_BOTTOM + HUDStartY, al_map_rgb(0, 255, 0));
    // now overlay red bar
    al_draw_filled_rectangle(SHIELD_STATUS_LEFT + HUDStartX, SHIELD_STATUS_TOP + HUDStartY, SHIELD_STATUS_LEFT + HUDStartX + displayShield, SHIELD_STATUS_BOTTOM + HUDStartY, al_map_rgb(255, 255, 255));

    // Now draw the lives
    spacing = lifeBitmapWidth + 2;
    for (i = 0; i < lives; i++)
        al_draw_bitmap(lifeBitmap, 2 + SHIELD_STATUS_LEFT + HUDStartX + i * spacing, 2 + SHIELD_STATUS_TOP + HUDStartY, 0);

    // Now draw the score
    spacing = digitSpriteWidth;
    j = 0;
    for (i = DIGITS_MAX - 1; i >= 0 ; i--)
    {
        digit = displayScore / NUMBER_TABLE[i] % 10;
        al_draw_bitmap_region(digitSpriteSheet, digit * digitSpriteWidth, 0, digitSpriteWidth, digitSpriteHeight, SCORE_NUMBERS_LEFT + HUDStartX + j * digitSpriteWidth, SCORE_NUMBERS_TOP + HUDStartY, 0);
        j++;
    }

    // Finally draw game over message is no lives are left
    if (lives <= 0)
        al_draw_text(font, al_map_rgb_f(1, 1, 1), bufferWidth / 2, bufferHeight / 2, ALLEGRO_ALIGN_CENTER, "G A M E  O V E R");
}
