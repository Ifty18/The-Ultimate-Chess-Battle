#include "game.h"
#include "../config.c"
#include <stdio.h>
#include "start.h"
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
extern int oldCell;
extern int newCell;
extern int auxiliarPiece;

//tabla are 8 * 87 latimea si lungimea == 696
//marginile sunt de 2

// -1 : no piece
// 1 : white pawn
// 2 : white rook
// 3 : white bishop
// 4 : white knight
// 5 : white queen
// 6 : white king
// 7 : black pawn
// 8 : black rook
// 9 : black bishop
// 10 : black knight
// 11 : black queen
// 12 : black king

//takes the selected cell as an imput and marks the valid cells
void checkValidMoves(int cellNumber, int pieceNumber, squares *square)
{
    // square->validMove[cellNumber] = 1;

    if (pieceNumber == 1)
    { // white pawn
        if ((cellNumber >= 49) && (cellNumber <= 56))
        { //in case it is the first move
            if ((square->pieceNumber[cellNumber - 8] == -1))
            { // no enemy in the first cell
                square->validMove[cellNumber - 8] = 1;
                if ((square->pieceNumber[cellNumber - 16] == -1))
                { // no enemy piece even 2 cells in front
                    square->validMove[cellNumber - 16] = 1;
                }
            }
            if ((square->pieceNumber[cellNumber - 7] >= 7) && (square->pieceNumber[cellNumber - 7] <= 12))
            {
                square->validMove[cellNumber - 7] = 1;
            }
            if ((square->pieceNumber[cellNumber - 9] >= 7) && (square->pieceNumber[cellNumber - 9] <= 12))
            {
                square->validMove[cellNumber - 9] = 1;
            }
        }
        else
        { //normal case
            if (square->pieceNumber[cellNumber - 8] == -1)
            { //if there is no piece in front of the pawn
                square->validMove[cellNumber - 8] = 1;
            }
            if ((square->pieceNumber[cellNumber - 7] >= 7) && (square->pieceNumber[cellNumber - 7] <= 12))
            { //if there is a piece in the top right corner
                square->validMove[cellNumber - 7] = 1;
            }
            if ((square->pieceNumber[cellNumber - 9] >= 7) && (square->pieceNumber[cellNumber - 9] <= 12))
            { //if there is a piece in the top left corner
                square->validMove[cellNumber - 9] = 1;
            }
        }
    }
    else if (pieceNumber == 7)
    { // black pawn
        if ((cellNumber >= 9) && (cellNumber <= 16))
        { //in case it is the first move
            if ((square->pieceNumber[cellNumber + 8] == -1))
            { // no enemy in the first cell
                square->validMove[cellNumber + 8] = 1;
                if ((square->pieceNumber[cellNumber + 16] == -1))
                { // no enemy piece even 2 cells in front
                    square->validMove[cellNumber + 16] = 1;
                }
            }
            if ((square->pieceNumber[cellNumber + 7] >= 1) && (square->pieceNumber[cellNumber + 7] <= 6))
            {
                square->validMove[cellNumber + 7] = 1;
            }
            if ((square->pieceNumber[cellNumber + 9] >= 1) && (square->pieceNumber[cellNumber + 9] <= 6))
            {
                square->validMove[cellNumber + 9] = 1;
            }
        }
        else
        { //normal case
            if (square->pieceNumber[cellNumber + 8] == -1)
            { //if there is no piece in front of the pawn
                square->validMove[cellNumber + 8] = 1;
            }
            if ((square->pieceNumber[cellNumber + 7] >= 1) && (square->pieceNumber[cellNumber + 7] <= 6))
            { //if there is a piece in the top right corner
                square->validMove[cellNumber + 7] = 1;
            }
            if ((square->pieceNumber[cellNumber + 9] >= 1) && (square->pieceNumber[cellNumber + 9] <= 6))
            { //if there is a piece in the top left corner
                square->validMove[cellNumber + 9] = 1;
            }
        }
    }
    else if (pieceNumber == 2)
    { // white rook
        int aux = cellNumber;

        while ((aux >= 9) && ((square->pieceNumber[aux - 8] == -1) || (square->pieceNumber[aux - 8] >= 7) && (square->pieceNumber[aux - 8] <= 12)))
        { //creating the path if the rook goes up the table
            square->validMove[aux - 8] = 1;
            if ((square->pieceNumber[aux - 8] >= 7) && (square->pieceNumber[aux - 8] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux - 8;
        }

        aux = cellNumber; //resetting the value of aux to the initial position

        while ((aux <= 56) && ((square->pieceNumber[aux + 8] == -1) || (square->pieceNumber[aux + 8] >= 7) && (square->pieceNumber[aux + 8] <= 12)))
        { //creating the path if the rook goes down the table
            square->validMove[aux + 8] = 1;
            if ((square->pieceNumber[aux + 8] >= 7) && (square->pieceNumber[aux + 8] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux + 8;
        }

        aux = cellNumber; //resetting the value of aux to the initial position

        while ((aux % 8 > 0) && ((square->pieceNumber[aux + 1] == -1) || (square->pieceNumber[aux + 1] >= 7) && (square->pieceNumber[aux + 1] <= 12)))
        { //creating the path if the rook goes right
            square->validMove[aux + 1] = 1;
            if ((square->pieceNumber[aux + 1] >= 7) && (square->pieceNumber[aux + 1] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux + 1;
        }

        aux = cellNumber; //resetting the value of aux to the initial position

        while (((aux % 8 > 1) || (aux % 8 == 0)) && ((square->pieceNumber[aux - 1] == -1) || (square->pieceNumber[aux - 1] >= 7) && (square->pieceNumber[aux - 1] <= 12)))
        { //creating the path if the rook goes left
            square->validMove[aux - 1] = 1;
            if ((square->pieceNumber[aux - 1] >= 7) && (square->pieceNumber[aux - 1] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux - 1;
        }
    }
    else if (pieceNumber == 8)
    { // black rook
        int aux = cellNumber;

        while ((aux >= 9) && ((square->pieceNumber[aux - 8] == -1) || (square->pieceNumber[aux - 8] >= 1) && (square->pieceNumber[aux - 8] <= 6)))
        { //creating the path if the rook goes up the table
            square->validMove[aux - 8] = 1;
            if ((square->pieceNumber[aux - 8] >= 1) && (square->pieceNumber[aux - 8] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux - 8;
        }

        aux = cellNumber; //resetting the value of aux to the initial position

        while ((aux <= 56) && ((square->pieceNumber[aux + 8] == -1) || (square->pieceNumber[aux + 8] >= 1) && (square->pieceNumber[aux + 8] <= 6)))
        { //creating the path if the rook goes down the table
            square->validMove[aux + 8] = 1;
            if ((square->pieceNumber[aux + 8] >= 1) && (square->pieceNumber[aux + 8] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux + 8;
        }

        aux = cellNumber; //resetting the value of aux to the initial position

        while ((aux % 8 > 0) && ((square->pieceNumber[aux + 1] == -1) || (square->pieceNumber[aux + 1] >= 1) && (square->pieceNumber[aux + 1] <= 6)))
        { //creating the path if the rook goes right
            square->validMove[aux + 1] = 1;
            if ((square->pieceNumber[aux + 1] >= 1) && (square->pieceNumber[aux + 1] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux + 1;
        }

        aux = cellNumber; //resetting the value of aux to the initial position

        while (((aux % 8 > 1) || (aux % 8 == 0)) && ((square->pieceNumber[aux - 1] == -1) || (square->pieceNumber[aux - 1] >= 1) && (square->pieceNumber[aux - 1] <= 6)))
        { //creating the path if the rook goes left
            square->validMove[aux - 1] = 1;
            if ((square->pieceNumber[aux - 1] >= 1) && (square->pieceNumber[aux - 1] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux = aux - 1;
        }
    }
    else if (pieceNumber == 3)
    { // white bishop
        int aux2 = cellNumber;

        while (((aux2 >= 9) && (aux2 % 8 != 0)) && ((square->pieceNumber[aux2 - 7] == -1) || ((square->pieceNumber[aux2 - 7] >= 7) && (square->pieceNumber[aux2 - 7] <= 12))))
        { //creating the path if the bishop goes right up
            square->validMove[aux2 - 7] = 1;
            if ((square->pieceNumber[aux2 - 7] >= 7) && (square->pieceNumber[aux2 - 7] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux2 = aux2 - 7;
        }

        aux2 = cellNumber;

        while (((aux2 >= 9) && (aux2 % 8 != 1)) && ((square->pieceNumber[aux2 - 9] == -1) || ((square->pieceNumber[aux2 - 9] >= 7) && (square->pieceNumber[aux2 - 9] <= 12))))
        { //creating the path if the bishop goes left up
            square->validMove[aux2 - 9] = 1;
            if ((square->pieceNumber[aux2 - 9] >= 7) && (square->pieceNumber[aux2 - 9] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux2 = aux2 - 9;
        }

        aux2 = cellNumber;

        while (((aux2 <= 56) && (aux2 % 8 != 1)) && ((square->pieceNumber[aux2 + 7] == -1) || ((square->pieceNumber[aux2 + 7] >= 7) && (square->pieceNumber[aux2 + 7] <= 12))))
        { //creating the path if the bishop goes left down
            square->validMove[aux2 + 7] = 1;
            if ((square->pieceNumber[aux2 + 7] >= 7) && (square->pieceNumber[aux2 + 7] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux2 = aux2 + 7;
        }

        aux2 = cellNumber;

        while (((aux2 <= 56) && (aux2 % 8 != 0)) && ((square->pieceNumber[aux2 + 9] == -1) || ((square->pieceNumber[aux2 + 9] >= 7) && (square->pieceNumber[aux2 + 9] <= 12))))
        { //creating the path if the bishop goes right down
            square->validMove[aux2 + 9] = 1;
            if ((square->pieceNumber[aux2 + 9] >= 7) && (square->pieceNumber[aux2 + 9] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux2 = aux2 + 9;
        }
    }
    else if (pieceNumber == 9)
    { // black bishop
        int aux3 = cellNumber;

        while (((aux3 >= 9) && (aux3 % 8 != 0)) && ((square->pieceNumber[aux3 - 7] == -1) || ((square->pieceNumber[aux3 - 7] >= 1) && (square->pieceNumber[aux3 - 7] <= 6))))
        { //creating the path if the bishop goes right up
            square->validMove[aux3 - 7] = 1;
            if ((square->pieceNumber[aux3 - 7] >= 1) && (square->pieceNumber[aux3 - 7] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux3 = aux3 - 7;
        }

        aux3 = cellNumber;

        while (((aux3 >= 9) && (aux3 % 8 != 1)) && ((square->pieceNumber[aux3 - 9] == -1) || ((square->pieceNumber[aux3 - 9] >= 1) && (square->pieceNumber[aux3 - 9] <= 6))))
        { //creating the path if the bishop goes left up
            square->validMove[aux3 - 9] = 1;
            if ((square->pieceNumber[aux3 - 9] >= 1) && (square->pieceNumber[aux3 - 9] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux3 = aux3 - 9;
        }

        aux3 = cellNumber;

        while (((aux3 <= 56) && (aux3 % 8 != 1)) && ((square->pieceNumber[aux3 + 7] == -1) || ((square->pieceNumber[aux3 + 7] >= 1) && (square->pieceNumber[aux3 + 7] <= 6))))
        { //creating the path if the bishop goes left down
            square->validMove[aux3 + 7] = 1;
            if ((square->pieceNumber[aux3 + 7] >= 1) && (square->pieceNumber[aux3 + 7] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux3 = aux3 + 7;
        }

        aux3 = cellNumber;

        while (((aux3 <= 56) && (aux3 % 8 != 0)) && ((square->pieceNumber[aux3 + 9] == -1) || ((square->pieceNumber[aux3 + 9] >= 1) && (square->pieceNumber[aux3 + 9] <= 6))))
        { //creating the path if the bishop goes right down
            square->validMove[aux3 + 9] = 1;
            if ((square->pieceNumber[aux3 + 9] >= 1) && (square->pieceNumber[aux3 + 9] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux3 = aux3 + 9;
        }
    }
    else if (pieceNumber == 5)
    { // white queen
        int aux4 = cellNumber;

        while (((aux4 >= 9) && (aux4 % 8 != 0)) && ((square->pieceNumber[aux4 - 7] == -1) || ((square->pieceNumber[aux4 - 7] >= 7) && (square->pieceNumber[aux4 - 7] <= 12))))
        { //creating the path if the queen goes right up
            square->validMove[aux4 - 7] = 1;
            if ((square->pieceNumber[aux4 - 7] >= 7) && (square->pieceNumber[aux4 - 7] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 7;
        }

        aux4 = cellNumber;

        while (((aux4 >= 9) && (aux4 % 8 != 1)) && ((square->pieceNumber[aux4 - 9] == -1) || ((square->pieceNumber[aux4 - 9] >= 7) && (square->pieceNumber[aux4 - 9] <= 12))))
        { //creating the path if the queen goes left up
            square->validMove[aux4 - 9] = 1;
            if ((square->pieceNumber[aux4 - 9] >= 7) && (square->pieceNumber[aux4 - 9] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 9;
        }

        aux4 = cellNumber;

        while (((aux4 <= 56) && (aux4 % 8 != 1)) && ((square->pieceNumber[aux4 + 7] == -1) || ((square->pieceNumber[aux4 + 7] >= 7) && (square->pieceNumber[aux4 + 7] <= 12))))
        { //creating the path if the queen goes left down
            square->validMove[aux4 + 7] = 1;
            if ((square->pieceNumber[aux4 + 7] >= 7) && (square->pieceNumber[aux4 + 7] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 7;
        }

        aux4 = cellNumber;

        while (((aux4 <= 56) && (aux4 % 8 != 0)) && ((square->pieceNumber[aux4 + 9] == -1) || ((square->pieceNumber[aux4 + 9] >= 7) && (square->pieceNumber[aux4 + 9] <= 12))))
        { //creating the path if the queen goes right down
            square->validMove[aux4 + 9] = 1;
            if ((square->pieceNumber[aux4 + 9] >= 7) && (square->pieceNumber[aux4 + 9] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 9;
        }

        aux4 = cellNumber;

        while ((aux4 >= 9) && ((square->pieceNumber[aux4 - 8] == -1) || (square->pieceNumber[aux4 - 8] >= 7) && (square->pieceNumber[aux4 - 8] <= 12)))
        { //creating the path if the queen goes up the table
            square->validMove[aux4 - 8] = 1;
            if ((square->pieceNumber[aux4 - 8] >= 7) && (square->pieceNumber[aux4 - 8] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 8;
        }

        aux4 = cellNumber; //resetting the value of aux to the initial position

        while ((aux4 <= 56) && ((square->pieceNumber[aux4 + 8] == -1) || (square->pieceNumber[aux4 + 8] >= 7) && (square->pieceNumber[aux4 + 8] <= 12)))
        { //creating the path if the queen goes down the table
            square->validMove[aux4 + 8] = 1;
            if ((square->pieceNumber[aux4 + 8] >= 7) && (square->pieceNumber[aux4 + 8] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 8;
        }

        aux4 = cellNumber; //resetting the value of aux to the initial position

        while ((aux4 % 8 > 0) && ((square->pieceNumber[aux4 + 1] == -1) || (square->pieceNumber[aux4 + 1] >= 7) && (square->pieceNumber[aux4 + 1] <= 12)))
        { //creating the path if the queen goes right
            square->validMove[aux4 + 1] = 1;
            if ((square->pieceNumber[aux4 + 1] >= 7) && (square->pieceNumber[aux4 + 1] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 1;
        }

        aux4 = cellNumber; //resetting the value of aux to the initial position

        while (((aux4 % 8 > 1) || (aux4 % 8 == 0)) && ((square->pieceNumber[aux4 - 1] == -1) || (square->pieceNumber[aux4 - 1] >= 7) && (square->pieceNumber[aux4 - 1] <= 12)))
        { //creating the path if the queen goes left
            square->validMove[aux4 - 1] = 1;
            if ((square->pieceNumber[aux4 - 1] >= 7) && (square->pieceNumber[aux4 - 1] <= 12))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 1;
        }
    }
    else if (pieceNumber == 11)
    { // black queen
        int aux4 = cellNumber;

        while (((aux4 >= 9) && (aux4 % 8 != 0)) && ((square->pieceNumber[aux4 - 7] == -1) || ((square->pieceNumber[aux4 - 7] >= 1) && (square->pieceNumber[aux4 - 7] <= 6))))
        { //creating the path if the queen goes right up
            square->validMove[aux4 - 7] = 1;
            if ((square->pieceNumber[aux4 - 7] >= 1) && (square->pieceNumber[aux4 - 7] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 7;
        }

        aux4 = cellNumber;

        while (((aux4 >= 9) && (aux4 % 8 != 1)) && ((square->pieceNumber[aux4 - 9] == -1) || ((square->pieceNumber[aux4 - 9] >= 1) && (square->pieceNumber[aux4 - 9] <= 6))))
        { //creating the path if the queen goes left up
            square->validMove[aux4 - 9] = 1;
            if ((square->pieceNumber[aux4 - 9] >= 1) && (square->pieceNumber[aux4 - 9] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 9;
        }

        aux4 = cellNumber;

        while (((aux4 <= 56) && (aux4 % 8 != 1)) && ((square->pieceNumber[aux4 + 7] == -1) || ((square->pieceNumber[aux4 + 7] >= 1) && (square->pieceNumber[aux4 + 7] <= 6))))
        { //creating the path if the queen goes left down
            square->validMove[aux4 + 7] = 1;
            if ((square->pieceNumber[aux4 + 7] >= 1) && (square->pieceNumber[aux4 + 7] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 7;
        }

        aux4 = cellNumber;

        while (((aux4 <= 56) && (aux4 % 8 != 0)) && ((square->pieceNumber[aux4 + 9] == -1) || ((square->pieceNumber[aux4 + 9] >= 1) && (square->pieceNumber[aux4 + 9] <= 6))))
        { //creating the path if the queen goes right down
            square->validMove[aux4 + 9] = 1;
            if ((square->pieceNumber[aux4 + 9] >= 1) && (square->pieceNumber[aux4 + 9] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 9;
        }

        aux4 = cellNumber;

        while ((aux4 >= 9) && ((square->pieceNumber[aux4 - 8] == -1) || (square->pieceNumber[aux4 - 8] >= 1) && (square->pieceNumber[aux4 - 8] <= 6)))
        { //creating the path if the queen goes up the table
            square->validMove[aux4 - 8] = 1;
            if ((square->pieceNumber[aux4 - 8] >= 1) && (square->pieceNumber[aux4 - 8] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 8;
        }

        aux4 = cellNumber; //resetting the value of aux to the initial position

        while ((aux4 <= 56) && ((square->pieceNumber[aux4 + 8] == -1) || (square->pieceNumber[aux4 + 8] >= 1) && (square->pieceNumber[aux4 + 8] <= 6)))
        { //creating the path if the queen goes down the table
            square->validMove[aux4 + 8] = 1;
            if ((square->pieceNumber[aux4 + 8] >= 1) && (square->pieceNumber[aux4 + 8] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 8;
        }

        aux4 = cellNumber; //resetting the value of aux to the initial position

        while ((aux4 % 8 > 0) && ((square->pieceNumber[aux4 + 1] == -1) || (square->pieceNumber[aux4 + 1] >= 1) && (square->pieceNumber[aux4 + 1] <= 6)))
        { //creating the path if the queen goes right
            square->validMove[aux4 + 1] = 1;
            if ((square->pieceNumber[aux4 + 1] >= 1) && (square->pieceNumber[aux4 + 1] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 + 1;
        }

        aux4 = cellNumber; //resetting the value of aux to the initial position

        while (((aux4 % 8 > 1) || (aux4 % 8 == 0)) && ((square->pieceNumber[aux4 - 1] == -1) || (square->pieceNumber[aux4 - 1] >= 1) && (square->pieceNumber[aux4 - 1] <= 6)))
        { //creating the path if the queen goes left
            square->validMove[aux4 - 1] = 1;
            if ((square->pieceNumber[aux4 - 1] >= 1) && (square->pieceNumber[aux4 - 1] <= 6))
            { //if we have an enemy piece in the way
                break;
            }
            aux4 = aux4 - 1;
        }
    }
    if (pieceNumber == 6)
    { //white king
        if ((cellNumber > 8) && ((square->pieceNumber[cellNumber - 8] == -1) || ((square->pieceNumber[cellNumber - 8] >= 7) && (square->pieceNumber[cellNumber - 8] <= 12))))
        { //if the king wants to go one square up
            square->validMove[cellNumber - 8] = 1;
        }
        if ((cellNumber <= 56) && ((square->pieceNumber[cellNumber + 8] == -1) || ((square->pieceNumber[cellNumber + 8] >= 7) && (square->pieceNumber[cellNumber + 8] <= 12))))
        { //if the king wants to go one square down
            square->validMove[cellNumber + 8] = 1;
        }
        if ((cellNumber % 8 != 1) && ((square->pieceNumber[cellNumber - 1] == -1) || ((square->pieceNumber[cellNumber - 1] >= 7) && (square->pieceNumber[cellNumber - 1] <= 12))))
        { //if the king wants to go one square left
            square->validMove[cellNumber - 1] = 1;
        }
        if ((cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber + 1] == -1) || ((square->pieceNumber[cellNumber + 1] >= 7) && (square->pieceNumber[cellNumber + 1] <= 12))))
        { //if the king wants to go one square right
            square->validMove[cellNumber + 1] = 1;
        }

        if ((cellNumber % 8 != 1) && (cellNumber > 8) && ((square->pieceNumber[cellNumber - 9] == -1) || ((square->pieceNumber[cellNumber - 9] >= 7) && (square->pieceNumber[cellNumber - 9] <= 12))))
        { //if the king wants to go one square up left
            square->validMove[cellNumber - 9] = 1;
        }
        if ((cellNumber % 8 != 1) && (cellNumber <= 56) && ((square->pieceNumber[cellNumber + 7] == -1) || ((square->pieceNumber[cellNumber + 7] >= 7) && (square->pieceNumber[cellNumber + 7] <= 12))))
        { //if the king wants to go one square down left
            square->validMove[cellNumber + 7] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber - 7] == -1) || ((square->pieceNumber[cellNumber - 7] >= 7) && (square->pieceNumber[cellNumber - 7] <= 12))))
        { //if the king wants to go one square up right
            square->validMove[cellNumber - 7] = 1;
        }
        if ((cellNumber <= 56) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber + 9] == -1) || ((square->pieceNumber[cellNumber + 9] >= 7) && (square->pieceNumber[cellNumber + 9] <= 12))))
        { //if the king wants to go one square down right
            square->validMove[cellNumber + 9] = 1;
        }
    }
    if (pieceNumber == 12)
    { //black king
        if ((cellNumber > 8) && ((square->pieceNumber[cellNumber - 8] == -1) || ((square->pieceNumber[cellNumber - 8] >= 1) && (square->pieceNumber[cellNumber - 8] <= 6))))
        { //if the king wants to go one square up
            square->validMove[cellNumber - 8] = 1;
        }
        if ((cellNumber <= 56) && ((square->pieceNumber[cellNumber + 8] == -1) || ((square->pieceNumber[cellNumber + 8] >= 1) && (square->pieceNumber[cellNumber + 8] <= 6))))
        { //if the king wants to go one square down
            square->validMove[cellNumber + 8] = 1;
        }
        if ((cellNumber % 8 != 1) && ((square->pieceNumber[cellNumber - 1] == -1) || ((square->pieceNumber[cellNumber - 1] >= 1) && (square->pieceNumber[cellNumber - 1] <= 6))))
        { //if the king wants to go one square left
            square->validMove[cellNumber - 1] = 1;
        }
        if ((cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber + 1] == -1) || ((square->pieceNumber[cellNumber + 1] >= 1) && (square->pieceNumber[cellNumber + 1] <= 6))))
        { //if the king wants to go one square right
            square->validMove[cellNumber + 1] = 1;
        }

        if ((cellNumber % 8 != 1) && (cellNumber > 8) && ((square->pieceNumber[cellNumber - 9] == -1) || ((square->pieceNumber[cellNumber - 9] >= 1) && (square->pieceNumber[cellNumber - 9] <= 6))))
        { //if the king wants to go one square up left
            square->validMove[cellNumber - 9] = 1;
        }
        if ((cellNumber % 8 != 1) && (cellNumber <= 56) && ((square->pieceNumber[cellNumber + 7] == -1) || ((square->pieceNumber[cellNumber + 7] >= 1) && (square->pieceNumber[cellNumber + 7] <= 6))))
        { //if the king wants to go one square down left
            square->validMove[cellNumber + 7] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber - 7] == -1) || ((square->pieceNumber[cellNumber - 7] >= 1) && (square->pieceNumber[cellNumber - 7] <= 6))))
        { //if the king wants to go one square up right
            square->validMove[cellNumber - 7] = 1;
        }
        if ((cellNumber <= 56) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber + 9] == -1) || ((square->pieceNumber[cellNumber + 9] >= 1) && (square->pieceNumber[cellNumber + 9] <= 6))))
        { //if the king wants to go one square down right
            square->validMove[cellNumber + 9] = 1;
        }
    }
    else if (pieceNumber == 4)
    { // white knight
        if ((cellNumber > 16) && (cellNumber % 8 != 1) && ((square->pieceNumber[cellNumber - 17] == -1) || ((square->pieceNumber[cellNumber - 17] >= 7) && (square->pieceNumber[cellNumber - 17] <= 12))))
        { //the knight goes up - left
            square->validMove[cellNumber - 17] = 1;
        }
        if ((cellNumber > 16) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber - 15] == -1) || ((square->pieceNumber[cellNumber - 15] >= 7) && (square->pieceNumber[cellNumber - 15] <= 12))))
        { //the knight goes up - right
            square->validMove[cellNumber - 15] = 1;
        }
        if ((cellNumber < 49) && (cellNumber % 8 != 1) && ((square->pieceNumber[cellNumber + 15] == -1) || ((square->pieceNumber[cellNumber + 15] >= 7) && (square->pieceNumber[cellNumber + 15] <= 12))))
        { //the knight goes down - left
            square->validMove[cellNumber + 15] = 1;
        }
        if ((cellNumber < 49) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber + 17] == -1) || ((square->pieceNumber[cellNumber + 17] >= 7) && (square->pieceNumber[cellNumber + 17] <= 12))))
        { //the knight goes down - right
            square->validMove[cellNumber + 17] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 1) && (cellNumber % 8 != 2) && ((square->pieceNumber[cellNumber - 10] == -1) || ((square->pieceNumber[cellNumber - 10] >= 7) && (square->pieceNumber[cellNumber - 10] <= 12))))
        { //the knight goes left - up
            square->validMove[cellNumber - 10] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 0) && (cellNumber % 8 != 7) && ((square->pieceNumber[cellNumber - 6] == -1) || ((square->pieceNumber[cellNumber - 6] >= 7) && (square->pieceNumber[cellNumber - 6] <= 12))))
        { //the knight goes right - up
            square->validMove[cellNumber - 6] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 0) && (cellNumber % 8 != 7) && ((square->pieceNumber[cellNumber - 6] == -1) || ((square->pieceNumber[cellNumber - 6] >= 7) && (square->pieceNumber[cellNumber - 6] <= 12))))
        { //the knight goes right - up
            square->validMove[cellNumber - 6] = 1;
        }
        if ((cellNumber < 57) && (cellNumber % 8 != 1) && (cellNumber % 8 != 2) && ((square->pieceNumber[cellNumber + 6] == -1) || ((square->pieceNumber[cellNumber + 6] >= 7) && (square->pieceNumber[cellNumber + 6] <= 12))))
        { //the knight goes left - down
            square->validMove[cellNumber + 6] = 1;
        }
        if ((cellNumber < 57) && (cellNumber % 8 != 0) && (cellNumber % 8 != 7) && ((square->pieceNumber[cellNumber + 10] == -1) || ((square->pieceNumber[cellNumber + 10] >= 7) && (square->pieceNumber[cellNumber + 10] <= 12))))
        { //the knight goes right - down
            square->validMove[cellNumber + 10] = 1;
        }
    }
    else if (pieceNumber == 10)
    { // black knight
        if ((cellNumber > 16) && (cellNumber % 8 != 1) && ((square->pieceNumber[cellNumber - 17] == -1) || ((square->pieceNumber[cellNumber - 17] >= 1) && (square->pieceNumber[cellNumber - 17] <= 6))))
        { //the knight goes up - left
            square->validMove[cellNumber - 17] = 1;
        }
        if ((cellNumber > 16) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber - 15] == -1) || ((square->pieceNumber[cellNumber - 15] >= 1) && (square->pieceNumber[cellNumber - 15] <= 6))))
        { //the knight goes up - right
            square->validMove[cellNumber - 15] = 1;
        }
        if ((cellNumber < 49) && (cellNumber % 8 != 1) && ((square->pieceNumber[cellNumber + 15] == -1) || ((square->pieceNumber[cellNumber + 15] >= 1) && (square->pieceNumber[cellNumber + 15] <= 6))))
        { //the knight goes down - left
            square->validMove[cellNumber + 15] = 1;
        }
        if ((cellNumber < 49) && (cellNumber % 8 != 0) && ((square->pieceNumber[cellNumber + 17] == -1) || ((square->pieceNumber[cellNumber + 17] >= 1) && (square->pieceNumber[cellNumber + 17] <= 6))))
        { //the knight goes down - right
            square->validMove[cellNumber + 17] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 1) && (cellNumber % 8 != 2) && ((square->pieceNumber[cellNumber - 10] == -1) || ((square->pieceNumber[cellNumber - 10] >= 1) && (square->pieceNumber[cellNumber - 10] <= 6))))
        { //the knight goes left - up
            square->validMove[cellNumber - 10] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 0) && (cellNumber % 8 != 7) && ((square->pieceNumber[cellNumber - 6] == -1) || ((square->pieceNumber[cellNumber - 6] >= 1) && (square->pieceNumber[cellNumber - 6] <= 6))))
        { //the knight goes right - up
            square->validMove[cellNumber - 6] = 1;
        }
        if ((cellNumber > 8) && (cellNumber % 8 != 0) && (cellNumber % 8 != 7) && ((square->pieceNumber[cellNumber - 6] == -1) || ((square->pieceNumber[cellNumber - 6] >= 1) && (square->pieceNumber[cellNumber - 6] <= 6))))
        { //the knight goes right - up
            square->validMove[cellNumber - 6] = 1;
        }
        if ((cellNumber < 57) && (cellNumber % 8 != 1) && (cellNumber % 8 != 2) && ((square->pieceNumber[cellNumber + 6] == -1) || ((square->pieceNumber[cellNumber + 6] >= 1) && (square->pieceNumber[cellNumber + 6] <= 6))))
        { //the knight goes left - down
            square->validMove[cellNumber + 6] = 1;
        }
        if ((cellNumber < 57) && (cellNumber % 8 != 0) && (cellNumber % 8 != 7) && ((square->pieceNumber[cellNumber + 10] == -1) || ((square->pieceNumber[cellNumber + 10] >= 1) && (square->pieceNumber[cellNumber + 10] <= 6))))
        { //the knight goes right - down
            square->validMove[cellNumber + 10] = 1;
        }
    }
}

//resets the cells "validMove" to 0
void clearValidMoves(squares *square)
{
    for (int i = 1; i < 65; i++)
    {
        square->validMove[i] = 0;
    }
}

void gameFunctionality(GameScreen *currentScreen, int *framesCounter, squares *square, bool *selected, int *otherCellIsSelected)
{

    if (myTurn == 0) //if it's not our turn
    {

        if (read(sd, &oldCell, sizeof(oldCell)) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
            // return errno;
        }
        if (read(sd, &newCell, sizeof(newCell)) < 0)
        {
            perror("[client]Eroare la read() de la server.\n");
            // return errno;
        }
        printf("[client] Oponentul a mutat piesa de pe %d pe %d \n", oldCell, newCell);
        if (oldCell == 65 || newCell == 65)
        {
            // break;
            printf(" !!! se doreste un exit !!! \n");
        }
        //update the table

        if(square->pieceNumber[newCell] == 6){
            printf("black won \n");
            initialiseTable(square);
            // playerNumber == 1;
            *framesCounter = 0;
            *currentScreen = BWIN;
            close(sd);
        } else if (square->pieceNumber[newCell] == 12){
            printf("white won \n");
            initialiseTable(square);
            // playerNumber == 1;
            *framesCounter = 0;
            *currentScreen = WWIN;
            close(sd);
        }

        auxiliarPiece = square->pieceNumber[oldCell];
        square->pieceNumber[oldCell] = -1;
        square->pieceNumber[newCell] = auxiliarPiece;
        myTurn = 1;
    }

    // printf("am intrat in game functionality \n");
    *framesCounter = *framesCounter + 1; //for some reason *framesCounter
    Rectangle cells[65];
    // int xValue, yValue;
    for (int i = 1; i < 65; i++)
    {
        cells[i].x = square->x[i];
        cells[i].y = square->y[i];
        cells[i].height = 87;
        cells[i].width = 87;
    }

    if (*framesCounter > 30)
    {
        //initializing the buttons area
        Rectangle leaveMatchButtonArea = {10, 670, buttonWidth, buttonHeight};

        //geting the mouse position
        Vector2 mousePoint = GetMousePosition(); //get the current mouse position
        if (CheckCollisionPointRec(mousePoint, leaveMatchButtonArea) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            // clearValidMoves(square);
            *otherCellIsSelected = -1;
            initialiseTable(square);
            *currentScreen = MENU;
            *framesCounter = 0;
            playerNumber = 1;
        }
        int ok = 0; //no king was taken
        //parsing the whole table
        for (int i = 1; i < 65; i++)
        {

            if (CheckCollisionPointRec(mousePoint, cells[i]) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (square->state[i] == 0) && (*selected == false) && (*otherCellIsSelected == -1) && (square->pieceNumber[i] != -1) && (((playerNumber == 1) && (square->pieceNumber[i] >= 1) && (square->pieceNumber[i] <= 6)) || ((playerNumber == 2) && (square->pieceNumber[i] >= 7) && (square->pieceNumber[i] <= 12)))) //if we select a new cell with a piece (no cell was previously selected)
            {
                //print the position of the cell and the piece it has
                int value = square->x[i];
                int value2 = square->y[i];
                int value3 = square->pieceNumber[i];
                printf("the cell with x = %d, y = %d and pieceNumber = %d was selected \n", value, value2, value3);
                printf("\n");

                //the square becomes selected
                square->state[i] = 1;
                *selected = true; //the selected variable is used for the clicking functionality
                *otherCellIsSelected = i;
                clearValidMoves(square);
                checkValidMoves(i, value3, square);
            }
            else if (CheckCollisionPointRec(mousePoint, cells[i]) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (square->state[i] == 1) && (*selected == false) && (*otherCellIsSelected != -1) && (square->pieceNumber[i] != -1)) //if a cell is deselected
            {
                printf("the cell with the number %d was deselected \n", i);
                printf("\n");
                square->state[i] = 0;
                *selected = true;
                *otherCellIsSelected = -1;
                clearValidMoves(square);
            }
            else if (CheckCollisionPointRec(mousePoint, cells[i]) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (square->state[i] == 0) && (*selected == false) && (*otherCellIsSelected != -1) && (square->pieceNumber[i] != -1) && ((((square->pieceNumber[i] >= 1) && (square->pieceNumber[i] <= 6)) && ((square->pieceNumber[*otherCellIsSelected] >= 1) && (square->pieceNumber[*otherCellIsSelected] <= 6))) || (((square->pieceNumber[i] >= 7) && (square->pieceNumber[i] <= 12)) && ((square->pieceNumber[*otherCellIsSelected] >= 7) && (square->pieceNumber[*otherCellIsSelected] <= 12))))) //if we want to select a friendly cell but another cell is already selected
            {
                int value4 = *otherCellIsSelected;
                int value5 = square->pieceNumber[i];
                square->state[value4] = 0; //we deselect the cell that was previously selected
                printf("the cell with the number %d was selected before \n", value4);
                printf("now we will deselect it and replace it with the cell number %d instead \n", i);
                *otherCellIsSelected = i;
                *selected = true;
                square->state[i] = 1;
                printf("\n");
                clearValidMoves(square);
                checkValidMoves(i, value5, square);
            }
            else if (CheckCollisionPointRec(mousePoint, cells[i]) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (square->state[i] == 0) && (*selected == false) && (*otherCellIsSelected != -1) && (square->pieceNumber[i] != -1) && ((((square->pieceNumber[i] >= 1) && (square->pieceNumber[i] <= 6)) && ((square->pieceNumber[*otherCellIsSelected] >= 7) && (square->pieceNumber[*otherCellIsSelected] <= 12))) || (((square->pieceNumber[i] >= 7) && (square->pieceNumber[i] <= 12)) && ((square->pieceNumber[*otherCellIsSelected] >= 1) && (square->pieceNumber[*otherCellIsSelected] <= 6)))) && (square->validMove[i] == 1)) //if we want to select an enemy cell and take its piece
            {

                int value4 = *otherCellIsSelected;
                if (square->pieceNumber[i] == 6)
                {
                    ok = 2; //black wins
                }
                else if (square->pieceNumber[i] == 12)
                {
                    ok = 1; //white wins
                }
                int value5 = square->pieceNumber[value4];
                square->state[value4] = 0; //we deselect the cell that was previously selected
                square->state[i] = 0;
                printf("a piece was taken! \n");
                *otherCellIsSelected = -1;
                *selected = true;
                square->pieceNumber[value4] = -1;
                square->pieceNumber[i] = value5;
                clearValidMoves(square);
                // if (*playerTurn == 1)
                // {
                //     *playerTurn = 2;
                // }
                // else if (playerNumber == 2)
                // {
                //     playerNumber = 1;
                // }
                // value4 e vechea pozitie
                // i e noua pozitie
                oldCell = value4;
                newCell = i;

                if (write(sd, &oldCell, sizeof(oldCell)) <= 0)
                {
                    perror("[client]Eroare la write() spre server.\n");
                    // return errno;
                }
                if (write(sd, &newCell, sizeof(newCell)) <= 0)
                {
                    perror("[client]Eroare la write() spre server.\n");
                    // return errno;
                }
                if (oldCell == 65 || newCell == 65)
                {
                    // break;
                    printf("se doreste a se iesi din joc \n");
                }
                myTurn = 0;
                //aici vom trimite raspunsul la server
            }
            else if (CheckCollisionPointRec(mousePoint, cells[i]) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (square->state[i] == 0) && (*selected == false) && (*otherCellIsSelected != -1) && (square->pieceNumber[i] == -1) && (square->validMove[i] == 1)) //if we want to move a piece to another cell
            {

                oldCell = *otherCellIsSelected;
                newCell = i;

                square->pieceNumber[i] = square->pieceNumber[*otherCellIsSelected];
                square->pieceNumber[*otherCellIsSelected] = -1;
                square->state[*otherCellIsSelected] = 0;
                square->state[i] = 0;
                *otherCellIsSelected = -1;
                *selected = true;
                printf("a piece was moved \n");
                clearValidMoves(square);
                // if (playerNumber == 1)
                // {
                //     playerNumber = 2;
                // }
                // else if (playerNumber == 2)
                // {
                //     playerNumber = 1;
                // }
                //aici vom trimite raspunsul la server


                 // value4 e vechea pozitie
                // i e noua pozitie
                

                if (write(sd, &oldCell, sizeof(oldCell)) <= 0)
                {
                    perror("[client]Eroare la write() spre server.\n");
                    // return errno;
                }
                if (write(sd, &newCell, sizeof(newCell)) <= 0)
                {
                    perror("[client]Eroare la write() spre server.\n");
                    // return errno;
                }
                if (oldCell == 65 || newCell == 65)
                {
                    // break;
                    printf("se doreste a se iesi din joc \n");
                }
                myTurn = 0;



            }
        }

        //*selected becomes 0 again as soon as the mouse left button is up
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && (*selected == true))
        {
            *selected = false;
        }

        if (ok == 1)
        {
            printf("white won \n");
            initialiseTable(square);
            // playerNumber == 1;
            *framesCounter = 0;
            *currentScreen = WWIN;
            close(sd);
        }
        else if (ok == 2)
        {
            printf("black won \n");
            initialiseTable(square);
            // playerNumber == 1;
            *framesCounter = 0;
            *currentScreen = BWIN;
            close(sd);
        }
    }
}

void gameVisual(Texture2D *table, Texture2D *blackPawn, Texture2D *blackRook, Texture2D *blackKnight, Texture2D *blackBishop, Texture2D *blackQueen, Texture2D *blackKing, Texture2D *whitePawn, Texture2D *whiteRook, Texture2D *whiteBishop, Texture2D *whiteKnight, Texture2D *whiteQueen, Texture2D *whiteKing, squares *square, bool *selected)
{
    DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
    // DrawText("GAME SCREEN", 20, 20, 40, MAROON);

    //leave match button
    DrawRectangle(20, 670, buttonWidth, buttonHeight, YELLOW);
    DrawText("Leave match", 25, 700, 30, MAROON);

    //drawing the table
    DrawTexture(*table, 622, 34, RAYWHITE);
    // DrawRectangle(624, 36, 87, 87, RED);

    if (playerNumber == 1)
    {
        DrawText("You are the first player (white)", 100, 100, 25, MAROON);
    }
    else if (playerNumber == 2)
    {
        DrawText("You are the second player (black)", 100, 100, 25, MAROON);
    }

    //drawing the pieces
    //firstly we draw the black pawns
    for (int i = 1; i < 65; i++)
    {

        if (square->pieceNumber[i] != -1)
        {
            if (square->pieceNumber[i] == 1)
            {
                DrawTexture(*whitePawn, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 2)
            {
                DrawTexture(*whiteRook, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 3)
            {
                DrawTexture(*whiteBishop, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 4)
            {
                DrawTexture(*whiteKnight, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 5)
            {
                DrawTexture(*whiteQueen, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 6)
            {
                DrawTexture(*whiteKing, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 7)
            {
                DrawTexture(*blackPawn, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 8)
            {
                DrawTexture(*blackRook, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 9)
            {
                DrawTexture(*blackBishop, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 10)
            {
                DrawTexture(*blackKnight, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 11)
            {
                DrawTexture(*blackQueen, square->x[i], square->y[i], RAYWHITE);
            }
            else if (square->pieceNumber[i] == 12)
            {
                DrawTexture(*blackKing, square->x[i], square->y[i], RAYWHITE);
            }
        }
        if (square->validMove[i] == 1)
        {
            int aux1 = square->x[i];
            int aux2 = square->y[i];
            DrawRectangle(aux1, aux2, 87, 87, ORANGE);
        }
    }
}