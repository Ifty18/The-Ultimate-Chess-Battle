#include "wwin.h"
#include "../config.c"

void wwinFunctionality(GameScreen *currentScreen, int *framesCounter)
{
    *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    if (*framesCounter > 200)
    {
        *currentScreen = MENU;
        *framesCounter = 0;
    }
}

void wwinVisual()
{
    // TODO: Draw LOGO screen here!
    DrawText("WHITE WON!", 490, 320, 70, LIGHTGRAY);
}
