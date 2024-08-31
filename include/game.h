#ifndef GAME_H
#define GAME_H

#include "board.h"

typedef struct Game
{
    Board board;
    int turn;
    int side;
    int pawn_promotion;
} Game;

#endif