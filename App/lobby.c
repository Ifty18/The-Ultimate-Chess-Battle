#include "lobby.h"
#include "../config.c"
#include "main.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int sd;
extern struct sockaddr_in client;
extern char msg[100];
extern int playerNumber;
extern int myTurn;

void lobbyFunctionality(GameScreen *currentScreen, int *framesCounter)
{
    // *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    // if (*framesCounter > 30)
    // {
    //     //initializing the buttons area
    //     Rectangle startButtonArea = {screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight};

    //     //geting the mouse position
    //     Vector2 mousePoint = GetMousePosition(); //get the current mouse position
    //     if (CheckCollisionPointRec(mousePoint, startButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    //     {
    //         *currentScreen = GAME;
    //         *framesCounter = 0;
    //     }
    // }

    if (connect(sd, (struct sockaddr *)&client, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        // return errno;
        // EndDrawing();
        // CloseWindow();
    }
    else
    {
        printf("ne-am conectat cu succes!!!! \n");
    }

    if (read(sd, msg, 100) < 0)
    {
        perror("[client]Eroare la read() de la server.\n");
        // return errno;
        // EndDrawing();
        // CloseWindow();
    }
    /* afisam mesajul primit */
    printf("[client]Acesta este jucatorul numarul %s \n", msg);

    if (atoi(msg) == 1 || atoi(msg) == 2)
    {
        playerNumber = atoi(msg);
        if(playerNumber == 1){
             myTurn = 1;
        } else if (playerNumber == 2){
             myTurn = 0;
        }
        *currentScreen = GAME;
        *framesCounter = 0;
        printf("am intrat in iful unde se schimba currentScreen = GAME \n");
    }
    printf("se trece de msg == 1 || msg == 2 \n");
}

void lobbyVisual()
{
    //coloring the menu page
    DrawRectangle(0, 0, screenWidth, screenHeight, DARKPURPLE);
    DrawText("Lobby SCREEN", 20, 20, 40, MAROON);
    DrawText("Waiting for other players..", 290, 220, 60, MAROON);
    // DrawText("The Ultimate Chess Battle", 290, 320, 60, LIGHTGRAY);

    //start button
    DrawRectangle(screenWidth / 2 - 100, screenHeight / 2 - 50, buttonWidth, buttonHeight, ORANGE);
    DrawText("Start", screenWidth / 2 - 57, screenHeight / 2 - 20, 30, MAROON);
}