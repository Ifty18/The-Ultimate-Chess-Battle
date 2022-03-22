#include "exit.h"
#include "../config.c"

void exitFunctionality(GameScreen *currentScreen, int *framesCounter)
{
    // TODO: Update EXIT screen variables here!

    // Press enter to return to LOGIN screen
    *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    if (*framesCounter > 30)
    {   
        //initializing the buttons area
        Rectangle yesButtonArea = {screenWidth / 2 - 100, screenHeight / 2 - 200, buttonWidth, buttonHeight};
        Rectangle noButtonArea = {screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight};

        //geting the mouse position
        Vector2 mousePoint = GetMousePosition(); //get the current mouse position
        if (CheckCollisionPointRec(mousePoint, yesButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            // *currentScreen = EXIT;
            // *framesCounter = 0;
            EndDrawing();
            CloseWindow();
        }
        if (CheckCollisionPointRec(mousePoint, noButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            *currentScreen = MENU;
            *framesCounter = 0;
        }
    }
}

void exitVisual()
{
    // TODO: Draw EXIT screen here!
    DrawRectangle(0, 0, screenWidth, screenHeight, DARKPURPLE);
    DrawText("Are you sure you want to exit?", 20, 20, 40, MAROON);

    //yes
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 200, buttonWidth, buttonHeight, ORANGE);
    DrawText("yes", screenWidth / 2 - 87, screenHeight / 2 - 170, 30, MAROON);

    //no
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight, ORANGE);
    DrawText("no", screenWidth / 2 - 57, screenHeight / 2 - 20, 30, MAROON);
}
