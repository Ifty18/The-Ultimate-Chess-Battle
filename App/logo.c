#include "logo.h"
#include "../config.c"

void logoFunctionality(GameScreen *currentScreen, int *framesCounter)
{
    *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    if (*framesCounter > 180)
    {
        *currentScreen = MENU;
        *framesCounter = 0;
    }
}

void logoVisual()
{
    // TODO: Draw LOGO screen here!
    DrawText("The Ultimate Chess Battle", 290, 320, 60, LIGHTGRAY);
}
