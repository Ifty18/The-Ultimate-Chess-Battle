#include "menu.h"
#include "../config.c"

void menuFunctionality(GameScreen *currentScreen, int *framesCounter)
{
    // if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    // {
    //     *currentScreen = EXIT;
    // }
    *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    if (*framesCounter > 30)
    {   
        //initializing the buttons area
        Rectangle findGameButtonArea = {screenWidth / 2 - 100, screenHeight / 2 - 200, buttonWidth, buttonHeight};
        Rectangle optionsButtonArea = {screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight};
        Rectangle exitButtonArea = {screenWidth / 2 - 100, screenHeight / 2 + 120, buttonWidth, buttonHeight};

        //geting the mouse position
        Vector2 mousePoint = GetMousePosition(); //get the current mouse position
        if (CheckCollisionPointRec(mousePoint, findGameButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            *currentScreen = LOBBY;
            *framesCounter = 0;
            //aici se deschide clientul pe partea de retele
        }
        if (CheckCollisionPointRec(mousePoint, optionsButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            *currentScreen = OPTIONS;
            *framesCounter = 0;
        }
        if (CheckCollisionPointRec(mousePoint, exitButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            *currentScreen = EXIT;
            *framesCounter = 0;
        }
    }
}

void menuVisual()
{
    //coloring the menu page
    DrawRectangle(0, 0, screenWidth, screenHeight, DARKPURPLE);
    DrawText("MENU SCREEN", 20, 20, 40, MAROON);

    //find a game button
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 200, buttonWidth, buttonHeight, ORANGE);
    DrawText("Find a game", screenWidth / 2 - 87, screenHeight / 2 - 170, 30, MAROON);

    //options button
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight, ORANGE);
    DrawText("Options", screenWidth / 2 - 57, screenHeight / 2 - 20, 30, MAROON);

    //exit button
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 + 120, buttonWidth, buttonHeight, ORANGE);
    DrawText("Exit", screenWidth / 2 - 27, screenHeight / 2 + 150, 30, MAROON);
}