#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

#include "game.h"
typedef enum GUIPiece
{
    wPAWN,
    wKNIGHT,
    wBISHOP,
    wROOK,
    wQUEEN,
    wKING,
    bPAWN,
    bKNIGHT,
    bBISHOP,
    bROOK,
    bQUEEN,
    bKING,
    EMPTY
} GUIPiece;

typedef struct WindowData
{
    int mouse_x;
    int mouse_y;
    int pieceHeld;
    int heldPieceType;
    int heldPieceIndex;
    SDL_Rect pawn_promotion_menu;
    U64 validMoves;
} WindowData;

int render_game(Game *game, Side side);

#endif