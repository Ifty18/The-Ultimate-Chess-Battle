#include <stdio.h>
#include "../config.c"
#include "start.h"
#include "login.h"
#include "logo.h"
#include "menu.h"
#include "lobby.h"
#include "game.h"
#include "wwin.h"
#include "bwin.h"
#include "options.h"
#include "exit.h"

void initialiseTable(squares * square){
     int i, j, k=1;
    for ( j = 0 ; j < 8 ; j++ ){
        for ( i= 0 ; i < 8 ; i++ ){
            square->x[k] = 624 + i * 87;
            square->y[k] = 36 + j * 87;
            square->state[k] = 0;
            square->validMove[k] = 0;
            if ( (k >= 9) && ( k <= 16 ) ){ //black pawn
                square->pieceNumber[k] = 7;
            } else if ( (k == 1) || (k == 8) ){ // black rook
                square->pieceNumber[k] = 8;
            } else if ( (k == 2) || (k == 7) ){ // black knight
                square->pieceNumber[k] = 10;
            } else if ( (k == 3) || (k == 6) ){ // black bishop
                square->pieceNumber[k] = 9;
            } else if ( k == 4 ){ // black queen
                square->pieceNumber[k] = 11;
            } else if ( k == 5 ){ // black king
                square->pieceNumber[k] = 12;
            } else if ( (k >= 49) && ( k <= 56 ) ){ // white pawn
                square->pieceNumber[k] = 1;
            } else if ( (k == 57) || (k == 64) ){ // white rook
                square->pieceNumber[k] = 2;
            } else if ( (k == 58) || (k == 63) ){ // white knight
                square->pieceNumber[k] = 4;
            } else if ( (k == 59) || (k == 62) ){ // white bishop
                square->pieceNumber[k] = 3;
            } else if ( k == 60 ){ // white queen 
                square->pieceNumber[k] = 5;
            } else if ( k == 61 ){ // white king
                square->pieceNumber[k] = 6;
            } else { // empty cells
                square->pieceNumber[k] = -1;
            }
            k++;
        }
    }
}

void startupScreen()
{

    GameScreen currentScreen = LOGO; //setting the game starting point at the LOGO page
    InitWindow(screenWidth, screenHeight, "The ultimate chess battle"); //opening the game window
    SetTargetFPS(60); // set our game to run at 60 frames-per-second
    int framesCounter = 0;
    bool selected = false;
    int otherCellIsSelected = -1;
    int playerTurn = 1; //1 means white's turn, 2 means black's turn
    Texture2D table = LoadTexture("../Assets/tabla.png");
    Texture2D blackPawn = LoadTexture("../Assets/blackPawn.png");
    Texture2D whitePawn = LoadTexture("../Assets/whitePawn.png");
    Texture2D blackRook = LoadTexture("../Assets/blackRook.png");
    Texture2D whiteRook = LoadTexture("../Assets/whiteRook.png");
    Texture2D blackBishop = LoadTexture("../Assets/blackBishop.png");
    Texture2D whiteBishop = LoadTexture("../Assets/whiteBishop.png");
    Texture2D blackKnight = LoadTexture("../Assets/blackKnight.png");
    Texture2D whiteKnight = LoadTexture("../Assets/whiteKnight.png");
    Texture2D blackKing = LoadTexture("../Assets/blackKing.png");
    Texture2D whiteKing = LoadTexture("../Assets/whiteKing.png");
    Texture2D blackQueen = LoadTexture("../Assets/blackQueen.png");
    Texture2D whiteQueen = LoadTexture("../Assets/whiteQueen.png");

    squares square;
    initialiseTable(&square);

    // Main game loop
    while (!WindowShouldClose()) // detects window close button or ESC key
    {
        // here is the functionality part of every screen
        switch (currentScreen)
        {
        case LOGO:
        {
            logoFunctionality(&currentScreen, &framesCounter);
        }
        // break;
        // case LOGIN:
        // {
        //     loginFunctionality(&currentScreen);
        // }
        break;
        case MENU:
        {
            menuFunctionality(&currentScreen, &framesCounter);
        }
        break;
        case LOBBY:
        {
            lobbyFunctionality(&currentScreen, &framesCounter);
        }
        break;
        case GAME:
        {
            gameFunctionality(&currentScreen, &framesCounter, &square, &selected, &otherCellIsSelected);
        }
        break;
        case WWIN:
        {
            wwinFunctionality(&currentScreen, &framesCounter);
        }
        break;
        case BWIN:
        {
            bwinFunctionality(&currentScreen, &framesCounter);
        }
        break;
        case OPTIONS:
        {
            optionsFunctionality(&currentScreen, &framesCounter);
        }
        break;
        case EXIT:
        {
            exitFunctionality(&currentScreen, &framesCounter);
        }
        break;
        default:
            break;
        }

        // here are the visual functions of every screen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case LOGO:
        {
            logoVisual();
        }
        // break;
        // case LOGIN:
        // {
        //     loginVisual();
        // }
        break;
        case MENU:
        {
            menuVisual();
        }
        break;
        case LOBBY:
        {
            lobbyVisual();
        }
        break;
        case GAME:
        {
            gameVisual(&table, &blackPawn, &blackRook, &blackKnight, &blackBishop, &blackQueen, &blackKing, &whitePawn, &whiteRook, &whiteBishop, &whiteKnight, &whiteQueen, &whiteKing, &square, &selected);
        }
        break;
        case WWIN:
        {
            wwinVisual();
        }
        break;
        case BWIN:
        {
            bwinVisual();
        }
        break;
        case OPTIONS:
        {
            optionsVisual();
        }
        break;
        case EXIT:
        {
            exitVisual();
        }
        break;
        default:
            break;
        }
        EndDrawing();
    }
    CloseWindow(); // Close window and OpenGL context
}