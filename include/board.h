#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#define WHITE 1
#define BLACK 0

typedef enum Piece
{
    Empty,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
} Piece;

typedef enum GameState
{
    CheckOnBlack,
    CheckOnWhite,
    Stalemate,
    Draw
} GameState;

typedef uint64_t U64;
typedef uint8_t Side;

typedef struct Board
{
    U64 wPawns;
    U64 wKnights;
    U64 wBishops;
    U64 wRooks;
    U64 wQueens;
    U64 wKing;

    U64 bPawns;
    U64 bKnights;
    U64 bBishops;
    U64 bRooks;
    U64 bQueens;
    U64 bKing;

    U64 wPieces;
    U64 bPieces;

    int gameState;
    int wCastling;
    int bCastling;

} Board;

void init_board(Board *board);
void print_board(const Board *board);
void print_u64(const U64 board);

#endif