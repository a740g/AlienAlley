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
    bufferSize.SetSize(al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display()));

    // Load game assets
    digitSpriteSheet = al_load_bitmap("dat/gfx/digits_ss.png");
    Game::checkInitialized(digitSpriteSheet, __FUNCTION__": failed to load dat/gfx/digits_ss.png");
    digitSpriteSize.SetSize(al_get_bitmap_width(digitSpriteSheet) / 10, al_get_bitmap_height(digitSpriteSheet));          // for 10 digits

    HUDBitmap = al_load_bitmap("dat/gfx/hud.png");
    Game::checkInitialized(HUDBitmap, __FUNCTION__": failed to load dat/gfx/hud.png");
    // Center & align to the bottom
    HUDStartPosition.SetPoint(bufferSize.cx / 2 - al_get_bitmap_width(HUDBitmap) / 2, bufferSize.cy - al_get_bitmap_height(HUDBitmap));

    lifeBitmap = al_load_bitmap("dat/gfx/life.png");
    Game::checkInitialized(lifeBitmap, __FUNCTION__": failed to load dat/gfx/life.png");
    lifeBitmapSize.SetSize(al_get_bitmap_width(lifeBitmap), al_get_bitmap_height(lifeBitmap));
}

HUD::~HUD()
{
    al_destroy_bitmap(digitSpriteSheet);
    al_destroy_bitmap(HUDBitmap);
    al_destroy_bitmap(lifeBitmap);
}

// Updates all game HUD elements
void HUD::update()
{
    // Increment the values first
    //score += gameScore;
    //lives += gameLives;
    //shield += gameShield;

    // Do a sanity check
    score = min(score, SCORE_MAX);
    lives = min(lives, LIVES_MAX);
    shield = min(shield, SHIELD_MAX);

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
    al_draw_bitmap(HUDBitmap, HUDStartPosition.x, HUDStartPosition.y, 0);

    // Now draw the shield
    al_draw_filled_rectangle(SHIELD_STATUS_LEFT + HUDStartPosition.x, SHIELD_STATUS_TOP + HUDStartPosition.y, SHIELD_STATUS_LEFT + HUDStartPosition.x + displayShield, SHIELD_STATUS_BOTTOM + HUDStartPosition.y, al_map_rgb_f(1, (float)displayShield / (float)SHIELD_MAX, (float)lives / (float)LIVES_MAX));

    // Now draw the lives
    spacing = lifeBitmapSize.cx + 2;
    for (i = 0; i < lives; i++)
        al_draw_bitmap(lifeBitmap, 2 + SHIELD_STATUS_LEFT + HUDStartPosition.x + i * spacing, 2 + SHIELD_STATUS_TOP + HUDStartPosition.y, 0);

    // Now draw the score
    spacing = digitSpriteSize.cx;
    j = 0;
    for (i = DIGITS_MAX - 1; i >= 0 ; i--)
    {
        digit = getDigit(displayScore, i);
        al_draw_bitmap_region(digitSpriteSheet, digit * digitSpriteSize.cx, 0, digitSpriteSize.cx, digitSpriteSize.cy, SCORE_NUMBERS_LEFT + HUDStartPosition.x + j * digitSpriteSize.cx, SCORE_NUMBERS_TOP + HUDStartPosition.y, 0);
        j++;
    }

    // Finally draw game over message is no lives are left
    if (lives < 0)
        strcpy_s(Game::message, sizeof(Game::message), "G A M E  O V E R");
}

int HUD::getDigit(int n, int p)
{
	switch (p)
    {
    case 0:
        break;
	case 1:
        n /= 10;
        break;
	case 2:
        n /= 100;
        break;
	case 3:
        n /= 1000;
        break;
	case 4:
        n /= 10000;
        break;
	case 5:
        n /= 100000;
        break;
	case 6:
        n /= 1000000;
        break;
	case 7:
        n /= 10000000;
        break;
	case 8:
        n /= 100000000;
        break;
	case 9:
        n /= 1000000000;
        break;
	default:
        Game::checkInitialized(false, __FUNCTION__": case not handled");
	}
	
    return n % 10;
}
