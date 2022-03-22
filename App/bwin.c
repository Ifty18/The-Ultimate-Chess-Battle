#include "bwin.h"
#include "../config.c"

void bwinFunctionality(GameScreen *currentScreen, int *framesCounter)
{
    *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    if (*framesCounter > 200)
    {
        *currentScreen = MENU;
        *framesCounter = 0;
    }
}

void bwinVisual()
{
    //coloring the menu page
    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);

    // TODO: Draw LOGO screen here!
    DrawText("BLACK WON!", 490, 320, 70, DARKGRAY);
}
