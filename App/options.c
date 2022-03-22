#include "options.h"
#include "../config.c"

void optionsFunctionality(GameScreen *currentScreen, int *framesCounter)
{
    
    *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    if (*framesCounter > 30)
    {   
        //initializing the buttons area
        Rectangle backButtonArea = {screenWidth / 2 - 100, screenHeight / 2 - 200, buttonWidth, buttonHeight};

        //geting the mouse position
        Vector2 mousePoint = GetMousePosition(); //get the current mouse position
        if (CheckCollisionPointRec(mousePoint, backButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            *currentScreen = MENU;
            *framesCounter = 0;
        }
    }
}

void optionsVisual()
{
    //coloring the menu page
    DrawRectangle(0, 0, screenWidth, screenHeight, DARKPURPLE);
    DrawText("OPTIONS SCREEN", 20, 20, 40, BLACK);

    //find a game button
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 200, buttonWidth, buttonHeight, ORANGE);
    DrawText("back", screenWidth / 2 - 87, screenHeight / 2 - 170, 30, MAROON);

}