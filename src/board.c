#include <stdio.h>
#include <stdlib.h>
#include "board.h"

void init_board(Board *board)
{

    board->wPawns = 0x000000000000FF00ULL;
    board->wKnights = 0x0000000000000042ULL;
    board->wBishops = 0x0000000000000024ULL;
    board->wRooks = 0x0000000000000081ULL;
    board->wQueens = 0x0000000000000008ULL;
    board->wKing = 0x0000000000000010ULL;

    board->bPawns = 0x00FF000000000000ULL;
    board->bKnights = 0x4200000000000000ULL;
    board->bBishops = 0x2400000000000000ULL;
    board->bRooks = 0x8100000000000000ULL;
    board->bQueens = 0x0800000000000000ULL;
    board->bKing = 0x1000000000000000ULL;

    board->wPieces = board->wPawns | board->wKnights | board->wBishops | board->wRooks | board->wQueens | board->wKing;
    board->bPieces = board->bPawns | board->bKnights | board->bBishops | board->bRooks | board->bQueens | board->bKing;
}

void print_board(const Board *board)
{
    const char *pieces[] = {"P", "N", "B", "R", "Q", "K", "p", "n", "b", "r", "q", "k"};
    U64 bitboards[] = {
        board->wPawns, board->wKnights, board->wBishops,
        board->wRooks, board->wQueens, board->wKing,
        board->bPawns, board->bKnights, board->bBishops,
        board->bRooks, board->bQueens, board->bKing};

    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            char piece = '.';
            for (int i = 0; i < 12; i++)
            {
                if (bitboards[i] & (1ULL << square))
                {
                    piece = pieces[i][0];
                    break;
                }
            }
            fprintf(stdout, "%c ", piece);
        }
        fprintf(stdout, "\n");
    }
}

void print_u64(const U64 board)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (board & (1ULL << square))
            {
                printf("X ");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }
}