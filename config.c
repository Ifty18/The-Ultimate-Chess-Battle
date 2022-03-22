#pragma once
#include "raylib.h"

typedef enum GameScreen
{
    LOGO,
    LOGIN,
    MENU,
    OPTIONS,
    LOBBY,
    GAME,
    WWIN,
    BWIN,
    EXIT
} GameScreen;

typedef struct table
{
    int x[65], y[65], pieceNumber[65], state[65], validMove[65];
} squares;


#define screenWidth 1366
#define screenHeight 768
#define buttonWidth 200
#define buttonHeight 80
