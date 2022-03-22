#pragma once
#include <stdio.h>
#include "../config.c"
#include "start.h"

void checkValidMoves(int cellNumber , int pieceNumber, squares *square);
void clearValidMoves(squares *square);
void gameFunctionality(GameScreen * currentScreen, int * framesCounter, squares * square, bool * selected, int *otherCellIsSelected);

void gameVisual(Texture2D *table, Texture2D *blackPawn, Texture2D *blackRook, Texture2D *blackKnight, Texture2D *blackBishop, Texture2D *blackQueen, Texture2D *blackKing, Texture2D *whitePawn, Texture2D *whiteRook, Texture2D *whiteBishop, Texture2D *whiteKnight, Texture2D *whiteQueen, Texture2D *whiteKing, squares *square, bool *selected);