#include "gui/main_window.h"
#include "moves.h"

#include <stdio.h>

#define BOARD_SIZE 8
#define TILE_SIZE 42

void bitboard_to_row_col(U64 bitboard, int *row, int *col)
{
    if (bitboard == 0)
        return; // No pieces

    int index = __builtin_ffsll(bitboard) - 1; // Get the index of the least significant bit
    *row = index / BOARD_SIZE;
    *col = index % BOARD_SIZE;
}

Piece convert_gui_piece_to_piece(GUIPiece piece)
{
    switch (piece)
    {
    case wPAWN:
        return 1;
    case wKNIGHT:
        return 2;
    case wBISHOP:
        return 3;
    case wROOK:
        return 4;
    case wQUEEN:
        return 5;
    case wKING:
        return 6;
    case bPAWN:
        return 1;
    case bKNIGHT:
        return 2;
    case bBISHOP:
        return 3;
    case bROOK:
        return 4;
    case bQUEEN:
        return 5;
    case bKING:
        return 6;
    default:
        return 0;
    }

    return 0;
}

void handle_mouse_events(SDL_Event *event, Game *game, WindowData *wdata, Mix_Chunk **sounds)
{
    switch (event->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (game->pawn_promotion)
            {
                if (wdata->mouse_y >= wdata->pawn_promotion_menu.y && wdata->mouse_y < wdata->pawn_promotion_menu.y + wdata->pawn_promotion_menu.h)
                {
                    if (event->motion.x >= wdata->pawn_promotion_menu.x && event->motion.x < wdata->pawn_promotion_menu.x + TILE_SIZE)
                    {
                        promote_pawn(game, Knight);
                    }
                    else if (event->motion.x >= wdata->pawn_promotion_menu.x + TILE_SIZE && event->motion.x < wdata->pawn_promotion_menu.x + TILE_SIZE * 2)
                    {
                        promote_pawn(game, Bishop);
                    }
                    else if (event->motion.x >= wdata->pawn_promotion_menu.x + TILE_SIZE * 2 && event->motion.x < wdata->pawn_promotion_menu.x + TILE_SIZE * 3)
                    {
                        promote_pawn(game, Rook);
                    }
                    else if (event->motion.x >= wdata->pawn_promotion_menu.x + TILE_SIZE * 3 && event->motion.x < wdata->pawn_promotion_menu.x + TILE_SIZE * 4)
                    {
                        promote_pawn(game, Queen);
                    }
                }
                game->turn = (game->turn == WHITE) ? BLACK : WHITE;
                return;
            }
            wdata->mouse_x = event->motion.x;
            wdata->mouse_y = event->motion.y;

            int x = event->button.x / TILE_SIZE;
            int y = event->button.y / TILE_SIZE;

            if (game->side == WHITE)
            {
                y = BOARD_SIZE - 1 - y;
                x = BOARD_SIZE - 1 - x;
            }

            int index = y * BOARD_SIZE + (BOARD_SIZE - 1 - x);

            U64 mask = 1ULL << index;
            U64 pieceMask = 0ULL;
            // Check if a piece is at the clicked position and belongs to the current player
            for (int pieceType = !game->turn * 6; pieceType < !game->turn * 6 + 6; pieceType++)
            {
                switch (pieceType)
                {
                case wPAWN:
                    pieceMask = game->board.wPawns;
                    break;
                case wKNIGHT:
                    pieceMask = game->board.wKnights;
                    break;
                case wBISHOP:
                    pieceMask = game->board.wBishops;
                    break;
                case wROOK:
                    pieceMask = game->board.wRooks;
                    break;
                case wQUEEN:
                    pieceMask = game->board.wQueens;
                    break;
                case wKING:
                    pieceMask = game->board.wKing;
                    break;
                case bPAWN:
                    pieceMask = game->board.bPawns;
                    break;
                case bKNIGHT:
                    pieceMask = game->board.bKnights;
                    break;
                case bBISHOP:
                    pieceMask = game->board.bBishops;
                    break;
                case bROOK:
                    pieceMask = game->board.bRooks;
                    break;
                case bQUEEN:
                    pieceMask = game->board.bQueens;
                    break;
                case bKING:
                    pieceMask = game->board.bKing;
                    break;
                default:
                    break;
                }

                // printf("---NEW-----\n");
                // print_u64(pieceMask);
                // printf("--------\n");
                // print_u64(mask);
                if (pieceMask & mask)
                {
                    wdata->validMoves = get_valid_moves(mask, &game->board, game->turn);
                    wdata->pieceHeld = 1;
                    wdata->heldPieceType = pieceType;
                    wdata->heldPieceIndex = index;
                    break;
                }
            }
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if (event->button.button == SDL_BUTTON_LEFT && wdata->pieceHeld)
        {

            int x = event->button.x / TILE_SIZE;
            int y = event->button.y / TILE_SIZE;

            if (game->turn == WHITE)
            {
            }
            y = BOARD_SIZE - 1 - y;
            int index = y * BOARD_SIZE + x;

            U64 mask = 1ULL << index;

            if ((wdata->validMoves & mask) > 0)
            {
                int result = move(game, 1ULL << wdata->heldPieceIndex, mask, convert_gui_piece_to_piece(wdata->heldPieceType));
                if (result == 1)
                {
                    Mix_PlayChannel(-1, sounds[1], 0);
                }
                else if (result == 0)
                {
                    Mix_PlayChannel(-1, sounds[0], 0);
                }
            }

            wdata->pieceHeld = 0;
            wdata->heldPieceIndex = -1;
            wdata->heldPieceType = -1;
            wdata->validMoves = 0;
        }
        break;

    case SDL_MOUSEMOTION:
        if (wdata->pieceHeld)
        {
            wdata->mouse_x = event->motion.x;
            wdata->mouse_y = event->motion.y;
        }
        break;
    }
}

void render_grid(SDL_Renderer *renderer)
{
    SDL_Rect tile;
    SDL_Color color1 = {154, 79, 36, 255}; // White color
    SDL_Color color2 = {80, 42, 21, 255};  // Black color

    // Loop through each row and column
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            // Determine the color of the current tile
            SDL_Color tileColor = ((row + col) % 2 == 0) ? color1 : color2;

            // Set the render draw color
            SDL_SetRenderDrawColor(renderer, tileColor.r, tileColor.g, tileColor.b, tileColor.a);

            // Define the tile rectangle
            tile.x = col * TILE_SIZE;
            tile.y = row * TILE_SIZE;
            tile.w = TILE_SIZE;
            tile.h = TILE_SIZE;

            // Render the tile
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

void render_pieces(SDL_Renderer *renderer, SDL_Texture *pieces, Game *game, Side side, WindowData *wData)
{
    SDL_Rect destRect;
    SDL_Rect srcRect;

    for (int pieceType = 0; pieceType < 12; pieceType++)
    {
        U64 bitboard;

        // TODO: Replace this switch
        switch (pieceType)
        {
        case wPAWN:
            bitboard = game->board.wPawns;
            break;
        case wKNIGHT:
            bitboard = game->board.wKnights;
            break;
        case wBISHOP:
            bitboard = game->board.wBishops;
            break;
        case wROOK:
            bitboard = game->board.wRooks;
            break;
        case wQUEEN:
            bitboard = game->board.wQueens;
            break;
        case wKING:
            bitboard = game->board.wKing;
            break;
        case bPAWN:
            bitboard = game->board.bPawns;
            break;
        case bKNIGHT:
            bitboard = game->board.bKnights;
            break;
        case bBISHOP:
            bitboard = game->board.bBishops;
            break;
        case bROOK:
            bitboard = game->board.bRooks;
            break;
        case bQUEEN:
            bitboard = game->board.bQueens;
            break;
        case bKING:
            bitboard = game->board.bKing;
            break;
        default:
            break;
        }

        // Calculate source rectangle position in the texture atlas
        srcRect.x = (pieceType % 6) * TILE_SIZE;
        srcRect.y = (pieceType / 6) * TILE_SIZE;
        srcRect.w = TILE_SIZE;
        srcRect.h = TILE_SIZE;
        while (bitboard)
        {

            int index = __builtin_ffsll(bitboard) - 1; // Get the index of the least significant bit
            int row = index / BOARD_SIZE;
            int col = index % BOARD_SIZE;

            if (side == WHITE)
            {
                row = BOARD_SIZE - 1 - row;
            }
            else
            {
                col = BOARD_SIZE - 1 - col;
            }

            destRect.x = col * TILE_SIZE;
            destRect.y = row * TILE_SIZE;

            destRect.w = TILE_SIZE;
            destRect.h = TILE_SIZE;
            // Render the piece
            SDL_RenderCopy(renderer, pieces, &srcRect, &destRect);

            // Clear the bit that was processed
            bitboard &= (bitboard - 1);
        }

        if (wData->pieceHeld)
        {
            // Calculate source rectangle position in the texture atlas for the held piece
            srcRect.x = (wData->heldPieceType % 6) * TILE_SIZE;
            srcRect.y = (wData->heldPieceType / 6) * TILE_SIZE;
            srcRect.w = TILE_SIZE;
            srcRect.h = TILE_SIZE;

            // Set destination rectangle for the held piece
            destRect.x = wData->mouse_x - TILE_SIZE / 2;
            destRect.y = wData->mouse_y - TILE_SIZE / 2;
            destRect.w = TILE_SIZE;
            destRect.h = TILE_SIZE;

            // Render the held piece
            SDL_RenderCopy(renderer, pieces, &srcRect, &destRect);
        }
    }
}

void render_highlights(SDL_Renderer *renderer, SDL_Texture *pieces, Game *game, Side side, WindowData *wData)
{
    SDL_Rect tile;
    SDL_Color targetColor = {162, 183, 39, 140}; // White color
    SDL_Color sourceColor = {80, 91, 21, 255};   // Black color

    // Render source highlight
    int x = wData->heldPieceIndex % BOARD_SIZE;
    int y = 7 - (wData->heldPieceIndex / BOARD_SIZE);

    SDL_Color tileColor = sourceColor;

    SDL_SetRenderDrawColor(renderer, tileColor.r, tileColor.g, tileColor.b, tileColor.a);

    if (side)
    {
        tile.x = x * TILE_SIZE;
        tile.y = y * TILE_SIZE;
    }
    else
    {
        tile.x = (BOARD_SIZE - 1 - x) * TILE_SIZE;
        tile.y = (BOARD_SIZE - 1 - y) * TILE_SIZE;
    }

    tile.w = TILE_SIZE;
    tile.h = TILE_SIZE;

    SDL_RenderFillRect(renderer, &tile);

    // Render target highlights
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, targetColor.r, targetColor.g, targetColor.b, targetColor.a);
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            int index = (7 - row) * BOARD_SIZE + col;
            U64 mask = 1ULL << index;
            if ((wData->validMoves & mask) > 0)
            {
                // Define the tile rectangle
                if (side)
                {
                    tile.x = col * TILE_SIZE;
                    tile.y = row * TILE_SIZE;
                }
                else
                {
                    tile.x = (BOARD_SIZE - 1 - col) * TILE_SIZE;
                    tile.y = (BOARD_SIZE - 1 - row) * TILE_SIZE;
                }

                tile.w = TILE_SIZE;
                tile.h = TILE_SIZE;

                // Render the tile
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
}

void render_pawn_promotion_menu(SDL_Renderer *renderer, SDL_Texture *pieces, Game *game, Side side, WindowData *wData)
{
    if (game->turn == WHITE)
    {
        U64 white_pawn_promotion_mask = 0xFF00000000000000ULL;
        U64 pawn = game->board.wPawns & white_pawn_promotion_mask;

        SDL_Rect destRect;
        SDL_Rect srcRect;

        srcRect.x = TILE_SIZE;
        srcRect.y = 0;
        srcRect.w = TILE_SIZE * 4;
        srcRect.h = TILE_SIZE;

        int index = __builtin_ffsll(pawn) - 1; // Get the index of the least significant bit
        int row = index / BOARD_SIZE;
        int col = index % BOARD_SIZE;
        if (col > 4)
        {
            destRect.x = (col - 3) * TILE_SIZE;
        }
        else
        {
            destRect.x = col * TILE_SIZE;
        }
        destRect.y = (BOARD_SIZE - row - 1) * TILE_SIZE;
        destRect.w = TILE_SIZE * 4;
        destRect.h = TILE_SIZE;
        wData->pawn_promotion_menu = destRect;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &destRect);
        SDL_RenderCopy(renderer, pieces, &srcRect, &destRect);
    }
    else
    {
        U64 black_pawn_promotion_mask = 0x00000000000000FFULL;
        U64 pawn = game->board.bPawns & black_pawn_promotion_mask;

        SDL_Rect destRect;
        SDL_Rect srcRect;

        srcRect.x = TILE_SIZE;
        srcRect.y = TILE_SIZE;
        srcRect.w = TILE_SIZE * 4;
        srcRect.h = TILE_SIZE;

        int index = __builtin_ffsll(pawn) - 1; // Get the index of the least significant bit
        int row = index / BOARD_SIZE;
        int col = index % BOARD_SIZE;
        if (col > 4)
        {
            destRect.x = (col - 3) * TILE_SIZE;
        }
        else
        {
            destRect.x = col * TILE_SIZE;
        }
        destRect.y = (BOARD_SIZE - row - 1) * TILE_SIZE;
        destRect.w = TILE_SIZE * 4;
        destRect.h = TILE_SIZE;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &destRect);
        SDL_RenderCopy(renderer, pieces, &srcRect, &destRect);
    }
}

void render_board(SDL_Renderer *renderer, SDL_Texture *pieces, Game *game, Side side, WindowData *wData)
{
    render_grid(renderer);
    render_highlights(renderer, pieces, game, side, wData);
    render_pieces(renderer, pieces, game, side, wData);
    if (game->pawn_promotion)
    {
        render_pawn_promotion_menu(renderer, pieces, game, side, wData);
    }
}

int render_game(Game *game, Side side)
{

    WindowData wData;
    wData.pieceHeld = 0;
    wData.validMoves = 0ULL;
    wData.heldPieceIndex = -1;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 336, 336, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Mix_Chunk **sounds = (Mix_Chunk **)malloc(sizeof(Mix_Chunk *) * 2);

    Mix_Chunk *moveHit = Mix_LoadWAV("assets/move-self.mp3");
    if (moveHit == NULL)
    {
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    Mix_Chunk *captureHit = Mix_LoadWAV("assets/capture.mp3");
    if (captureHit == NULL)
    {
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    sounds[0] = moveHit;
    sounds[1] = captureHit;

    // Load image
    SDL_Surface *loadedSurface = IMG_Load("assets/pieces.png");
    if (!loadedSurface)
    {
        printf("Unable to load image! IMG_Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *icon = IMG_Load("assets/icon.png");
    if (!loadedSurface)
    {
        printf("Unable to load image! IMG_Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowIcon(window, icon);

    // Create texture from surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!texture)
    {
        printf("Unable to create texture! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_FreeSurface(loadedSurface);

    // Main loop
    int quit = 0;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
            handle_mouse_events(&e, game, &wData, sounds);
        }

        // Clear screen
        SDL_RenderClear(renderer);

        // Render texture
        render_board(renderer, texture, game, side, &wData);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    Mix_FreeChunk(moveHit);
    Mix_FreeChunk(captureHit);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
