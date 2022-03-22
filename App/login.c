#include "login.h"
#include "../config.c"

void loginFunctionality(GameScreen *currentScreen)
{
    Rectangle loginButtonArea = {screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight};
    Vector2 mousePoint = GetMousePosition(); //get the current mouse position
    if (CheckCollisionPointRec(mousePoint, loginButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        *currentScreen = MENU;
    }
}

void loginVisual()
{
    // TODO: Draw LOGIN screen here!
    DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
    DrawText("LOGIN SCREEN", 20, 20, 40, DARKGREEN);
    DrawText("Enter your username to login", 120, 220, 20, DARKGREEN);
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight, VIOLET);
    DrawText("Login", screenWidth / 2 - 57, screenHeight / 2 - 20, 30, WHITE);
}