
#include "moves.h"
#include <stdio.h>

const U64 KNIGHT_MOVES[64] = {
    0x0000000000020400ULL, 0x0000000000050800ULL, 0x00000000000A1100ULL, 0x0000000000142200ULL,
    0x0000000000284400ULL, 0x0000000000508800ULL, 0x0000000000A01000ULL, 0x0000000000402000ULL,
    0x0000000002040004ULL, 0x0000000005080008ULL, 0x000000000A110011ULL, 0x0000000014220022ULL,
    0x0000000028440044ULL, 0x0000000050880088ULL, 0x00000000A0100010ULL, 0x0000000040200020ULL,
    0x0000000204000402ULL, 0x0000000508000805ULL, 0x0000000A1100110AULL, 0x0000001422002214ULL,
    0x0000002844004428ULL, 0x0000005088008850ULL, 0x000000A0100010A0ULL, 0x0000004020002040ULL,
    0x0000020400040200ULL, 0x0000050800080500ULL, 0x00000A1100110A00ULL, 0x0000142200221400ULL,
    0x0000284400442800ULL, 0x0000508800885000ULL, 0x0000A0100010A000ULL, 0x0000402000204000ULL,
    0x0002040004020000ULL, 0x0005080008050000ULL, 0x000A1100110A0000ULL, 0x0014220022140000ULL,
    0x0028440044280000ULL, 0x0050880088500000ULL, 0x00A0100010A00000ULL, 0x0040200020400000ULL,
    0x0204000402000000ULL, 0x0508000805000000ULL, 0x0A1100110A000000ULL, 0x1422002214000000ULL,
    0x2844004428000000ULL, 0x5088008850000000ULL, 0xA0100010A0000000ULL, 0x4020002040000000ULL,
    0x0400040200000000ULL, 0x0800080500000000ULL, 0x1100110A00000000ULL, 0x2200221400000000ULL,
    0x4400442800000000ULL, 0x8800885000000000ULL, 0x100010A000000000ULL, 0x2000204000000000ULL,
    0x0004020000000000ULL, 0x0008050000000000ULL, 0x00110A0000000000ULL, 0x0022140000000000ULL,
    0x0044280000000000ULL, 0x0088500000000000ULL, 0x0010A00000000000ULL, 0x0020400000000000ULL};

const U64 ROOK_MOVES[64] = {
    0x01010101010101FEULL, 0x02020202020202FDULL, 0x04040404040404FBULL, 0x08080808080808F7ULL, 0x10101010101010EFULL, 0x20202020202020DFULL, 0x40404040404040BFULL, 0x808080808080807FULL,
    0x010101010101FE01ULL, 0x020202020202FD02ULL, 0x040404040404FB04ULL, 0x080808080808F708ULL, 0x101010101010EF10ULL, 0x202020202020DF20ULL, 0x404040404040BF40ULL, 0x8080808080807F80ULL,
    0x0101010101FE0101ULL, 0x0202020202FD0202ULL, 0x0404040404FB0404ULL, 0x0808080808F70808ULL, 0x1010101010EF1010ULL, 0x2020202020DF2020ULL, 0x4040404040BF4040ULL, 0x80808080807F8080ULL,
    0x01010101FE010101ULL, 0x02020202FD020202ULL, 0x04040404FB040404ULL, 0x08080808F7080808ULL, 0x10101010EF101010ULL, 0x20202020DF202020ULL, 0x40404040BF404040ULL, 0x808080807F808080ULL,
    0x010101FE01010101ULL, 0x020202FD02020202ULL, 0x040404FB04040404ULL, 0x080808F708080808ULL, 0x101010EF10101010ULL, 0x202020DF20202020ULL, 0x404040BF40404040ULL, 0x8080807F80808080ULL,
    0x0101FE0101010101ULL, 0x0202FD0202020202ULL, 0x0404FB0404040404ULL, 0x0808F70808080808ULL, 0x1010EF1010101010ULL, 0x2020DF2020202020ULL, 0x4040BF4040404040ULL, 0x80807F8080808080ULL,
    0x01FE010101010101ULL, 0x02FD020202020202ULL, 0x04FB040404040404ULL, 0x08F7080808080808ULL, 0x10EF101010101010ULL, 0x20DF202020202020ULL, 0x40BF404040404040ULL, 0x807F808080808080ULL,
    0xFE01010101010101ULL, 0xFD02020202020202ULL, 0xFB04040404040404ULL, 0xF708080808080808ULL, 0xEF10101010101010ULL, 0xDF20202020202020ULL, 0xBF40404040404040ULL, 0x7F80808080808080ULL};

// TODO: check for blocking check, en passant
U64 generate_pawn_moves(U64 square, Board *board, Side side)
{
    U64 empty_squares = ~(board->wPieces | board->bPieces);
    U64 opponent_squares = side ? board->bPieces : board->wPieces;

    U64 moves = 0ULL;
    if (side)
    {
        // Single move forward
        moves |= (square << 8) & empty_squares;
        // Double move forward from initial rank
        moves |= ((square & 0x000000000000FF00ULL) << 16) & empty_squares & (empty_squares << 8);
        // Captures
        moves |= (square << 7) & opponent_squares & 0x7F7F7F7F7F7F7F7FULL; // Capture right
        moves |= (square << 9) & opponent_squares & 0xFEFEFEFEFEFEFEFEULL; // Capture left
    }
    else
    {
        // Single move forward
        moves |= (square >> 8) & empty_squares;
        // Double move forward from initial rank
        moves |= ((square & 0x00FF000000000000ULL) >> 16) & empty_squares & (empty_squares >> 8);
        // Captures
        moves |= (square >> 7) & opponent_squares & 0xFEFEFEFEFEFEFEFEULL; // Capture left
        moves |= (square >> 9) & opponent_squares & 0x7F7F7F7F7F7F7F7FULL; // Capture right
    }
    return moves;
}

U64 generate_knight_moves(U64 square, Board *board, Side side)
{
    U64 moves = 0ULL;
    int knight_idx = __builtin_ctzll(square);

    U64 empty_squares = ~(board->wPieces | board->bPieces);
    U64 opponent_squares = side ? board->bPieces ^ board->bKing : board->wPieces ^ board->wKing;

    moves = KNIGHT_MOVES[knight_idx] & (empty_squares | opponent_squares);

    return moves;
}

U64 generate_diagonal_moves(U64 square, Board *board, Side side)
{
    U64 moves = 0;
    U64 opponent_pieces = side ? board->bPieces ^ board->bKing : board->wPieces ^ board->wKing;
    U64 player_pieces = side ? board->wPieces : board->bPieces;
    // Masks for board boundaries
    const U64 leftEdge = 0x8080808080808080ULL;
    const U64 rightEdge = 0x0101010101010101ULL;

    // Iterate over each direction
    for (int d = 0; d < 4; ++d)
    {
        U64 current = square;

        while (1)
        {
            // Move in the current direction
            U64 next = current;

            if (d == 0)
            { // bottom-left
                next = (current >> 9) & ~leftEdge;
            }
            else if (d == 1)
            { // bottom-right
                next = (current >> 7) & ~rightEdge;
            }
            else if (d == 2)
            { // top-left
                next = (current << 7) & ~leftEdge;
            }
            else if (d == 3)
            { // top-right
                next = (current << 9) & ~rightEdge;
            }

            if (next == 0)
                break; // Exit loop if out of bounds

            if ((player_pieces & next) != 0)
            {
                break; // Stop if the square is occupied by a piece of the same side
            }
            else if ((opponent_pieces & next) != 0)
            {
                moves |= next; // Add move for capture
                break;         // Stop after capturing
            }
            else
            {
                moves |= next; // Add non-capturing move
            }

            current = next; // Continue in the same direction
        }
    }
    // print_u64(moves);
    return moves;
}

U64 generate_vertical_moves(U64 square, Board *board, Side side)
{
    U64 moves = 0ULL;
    U64 current = square;

    U64 opponent_pieces = side == WHITE ? board->bPieces : board->wPieces;
    U64 player_pieces = side == WHITE ? board->wPieces : board->bPieces;

    // Move up (north)
    while (current && !(current & 0xFF00000000000000ULL)) // Check if not on the 8th rank
    {
        current <<= 8;
        if (player_pieces & current)
            break;
        moves |= current;
        if (opponent_pieces & current)
            break;
    }

    // Move down (south)
    current = square;
    while (current && !(current & 0x00000000000000FFULL)) // Check if not on the 1st rank
    {
        current >>= 8;
        if (player_pieces & current)
            break;
        moves |= current;
        if (opponent_pieces & current)
            break;
    }

    return moves;
}

U64 generate_horizontal_moves(U64 square, Board *board, Side side)
{
    U64 moves = 0;
    U64 current;

    U64 opponent_pieces = side == WHITE ? board->bPieces : board->wPieces;
    U64 player_pieces = side == WHITE ? board->wPieces : board->bPieces;

    // Masks for board boundaries
    const U64 leftEdge = 0x0101010101010101ULL;
    const U64 rightEdge = 0x8080808080808080ULL;

    // Move left (west)
    current = square;
    while (current & ~leftEdge)
    {
        current >>= 1;
        if (current & leftEdge)
            break; // Ensure we don't wrap around
        if (player_pieces & current)
            break;
        moves |= current;
        if (opponent_pieces & current)
            break;
    }

    // Move right (east)
    current = square;
    while (current & ~rightEdge)
    {
        current <<= 1;
        if (current & rightEdge)
            break; // Ensure we don't wrap around
        if (player_pieces & current)
            break;
        moves |= current;
        if (opponent_pieces & current)
            break;
    }

    return moves;
}

U64 generate_bishop_moves(U64 square, Board *board, Side side)
{
    U64 moves = 0ULL;
    moves = generate_diagonal_moves(square, board, side);
    return moves;
}

U64 generate_rook_moves(U64 square, Board *board, Side side)
{
    U64 horizontal_moves = generate_horizontal_moves(square, board, side);
    U64 vertical_moves = generate_vertical_moves(square, board, side);

    return horizontal_moves | vertical_moves;
}

U64 generate_queen_moves(U64 square, Board *board, Side side)
{
    U64 moves = 0ULL;
    moves = generate_rook_moves(square, board, side);
    moves |= generate_diagonal_moves(square, board, side);
    return moves;
}

U64 generate_king_moves(U64 square, Board *board, Side side)
{
    U64 moves = 0ULL;

    // Define directions for king moves
    int directions[8] = {
        1,  // East
        -1, // West
        8,  // North
        -8, // South
        9,  // North-East
        7,  // North-West
        -7, // South-East
        -9  // South-West
    };

    // Determine opponent's pieces
    U64 opponent_pieces = (side == WHITE) ? (board->bPieces ^ board->bKing) : (board->wPieces ^ board->wKing);

    // Determine friendly pieces
    U64 friendly_pieces = (side == WHITE) ? (board->wPawns | board->wKnights | board->wBishops | board->wRooks | board->wQueens | board->wKing) : (board->bPawns | board->bKnights | board->bBishops | board->bRooks | board->bQueens | board->bKing);

    for (int i = 0; i < 8; i++)
    {
        int shift = directions[i];
        U64 target_square = 0;

        // Check if the move is within board boundaries
        if (shift == 1 && (square & 0x0101010101010101ULL))
            continue; // Moving east from a file
        if (shift == -1 && (square & 0x8080808080808080ULL))
            continue; // Moving west from h file
        if (shift == 8 && (square & 0xFF00000000000000ULL))
            continue; // Moving north from 8th rank
        if (shift == -8 && (square & 0x00000000000000FFULL))
            continue; // Moving south from 1st rank
        if (shift == 9 && (square & 0xFF01010101010101ULL))
            continue; // Moving north-east from 8th rank or a file
        if (shift == 7 && (square & 0xFF80808080808080ULL))
            continue; // Moving north-west from 8th rank or h file
        if (shift == -7 && (square & 0x01010101010101FFULL))
            continue; // Moving south-east from 1st rank or a file
        if (shift == -9 && (square & 0x80808080808080FFULL))
            continue; // Moving south-west from 1st rank or h file

        if (shift > 0)
            target_square = square << shift;
        else
            target_square = square >> -shift;

        // Add the move if it's not occupied by a friendly piece
        if (!(target_square & friendly_pieces))
        {
            moves |= target_square;
        }
    }

    return moves;
}

U64 get_piece_bit(char *square)
{

    if (square[0] < 'a' || square[0] > 'h' || square[1] < '1' || square[1] > '8')
    {
        return 0ULL; // Invalid input
    }

    int file = square[0] - 'a';
    int rank = square[1] - '1';
    int position = rank * 8 + file;

    return 1ULL << position;
}

Piece get_piece_type(U64 square, Board *board)
{

    if ((square & (board->wPawns | board->bPawns)) > 0)
    {
        return Pawn;
    }
    else if ((square & (board->wKnights | board->bKnights)) > 0)
    {
        return Knight;
    }
    else if ((square & (board->wBishops | board->bBishops)) > 0)
    {
        return Bishop;
    }
    else if ((square & (board->wRooks | board->bRooks)) > 0)
    {
        return Rook;
    }
    else if ((square & (board->wQueens | board->bQueens)) > 0)
    {
        return Queen;
    }
    else if ((square & (board->wKing | board->bKing)) > 0)
    {
        return King;
    }

    return Empty;
}

int move(Game *game, U64 from, U64 to, Piece type)
{
    int captured = 0;
    Side current_side = game->turn;
    Side opponent_side = (current_side == WHITE) ? BLACK : WHITE;

    // Check for capture
    if ((current_side == WHITE && (game->board.bPieces & to)) ||
        (current_side == BLACK && (game->board.wPieces & to)))
    {
        if (!capture(game, from, to, type))
        {
            return -1;
        }
        captured = 1;
    }

    // Move the piece
    U64 *piece_bitboard = get_piece_bitboard(&game->board, type, current_side);
    *piece_bitboard &= ~from;
    *piece_bitboard |= to;

    // Update the side's overall bitboard
    U64 *side_bitboard = (current_side == WHITE) ? &game->board.wPieces : &game->board.bPieces;
    *side_bitboard &= ~from;
    *side_bitboard |= to;

    // Handle pawn promotion
    if (type == Pawn)
    {
        U64 promotion_rank = (current_side == WHITE) ? 0xFF00000000000000ULL : 0x00000000000000FFULL;
        if (to & promotion_rank)
        {
            // Default promotion to Queen, can be changed based on user input
            Piece promotion_piece = Queen;
            promote_pawn(game, promotion_piece);
        }
    }

    // Switch turn if no promotion occurred
    if (!game->pawn_promotion)
    {
        game->turn = opponent_side;
    }

    print_board(&game->board);
    printf("-----------\n");
    return captured;
}

int capture(Game *game, U64 from, U64 to, Piece type)
{
    if ((game->turn == WHITE && (game->board.bKing & to)) ||
        (game->turn == BLACK && (game->board.wKing & to)))
    {
        fprintf(stderr, "Error: King capture attempted. Invalid game state.\n");
        return 0;
    }

    int captured_side = !game->turn;

    if (captured_side)
    {
        game->board.wPawns &= ~to;
        game->board.wKnights &= ~to;
        game->board.wBishops &= ~to;
        game->board.wRooks &= ~to;
        game->board.wQueens &= ~to;

        switch (type)
        {
        case Pawn:
            game->board.bPawns &= ~from;
            game->board.bPawns |= to;
            break;
        case Knight:
            game->board.bKnights &= ~from;
            game->board.bKnights |= to;
            break;
        case Bishop:
            game->board.bBishops &= ~from;
            game->board.bBishops |= to;
            break;
        case Rook:
            game->board.bRooks &= ~from;
            game->board.bRooks |= to;
            break;
        case Queen:
            game->board.bQueens &= ~from;
            game->board.bQueens |= to;
            break;
        }
    }
    else
    {
        game->board.bPawns &= ~to;
        game->board.bKnights &= ~to;
        game->board.bBishops &= ~to;
        game->board.bRooks &= ~to;
        game->board.bQueens &= ~to;

        switch (type)
        {
        case Pawn:
            game->board.wPawns &= ~from;
            game->board.wPawns |= to;
            break;
        case Knight:
            game->board.wKnights &= ~from;
            game->board.wKnights |= to;
            break;
        case Bishop:
            game->board.wBishops &= ~from;
            game->board.wBishops |= to;
            break;
        case Rook:
            game->board.wRooks &= ~from;
            game->board.wRooks |= to;
            break;
        case Queen:
            game->board.wQueens &= ~from;
            game->board.wQueens |= to;
            break;
        }
    }

    return 1;
}

int promote_pawn(Game *game, Piece type)
{
    U64 promotion_mask;

    if (game->turn == WHITE)
    {
        promotion_mask = 0xFF00000000000000ULL; // 8th rank for white pawns
    }
    else
    {
        promotion_mask = 0x00000000000000FFULL; // 1st rank for black pawns
    }

    switch (type)
    {
    case Queen:
        if (game->turn == WHITE)
        {
            game->board.wQueens |= (game->board.wPawns & promotion_mask);
            game->board.wPawns &= ~promotion_mask;
        }
        else
        {
            game->board.bQueens |= (game->board.bPawns & promotion_mask);
            game->board.bPawns &= ~promotion_mask;
        }
        return 1;
    case Bishop:
        if (game->turn == WHITE)
        {
            game->board.wBishops |= (game->board.wPawns & promotion_mask);
            game->board.wPawns &= ~promotion_mask;
        }
        else
        {
            game->board.bBishops |= (game->board.bPawns & promotion_mask);
            game->board.bPawns &= ~promotion_mask;
        }
        return 1;
    case Rook:
        if (game->turn == WHITE)
        {
            game->board.wRooks |= (game->board.wPawns & promotion_mask);
            game->board.wPawns &= ~promotion_mask;
        }
        else
        {
            game->board.bRooks |= (game->board.bPawns & promotion_mask);
            game->board.bPawns &= ~promotion_mask;
        }
        return 1;
    case Knight:
        if (game->turn == WHITE)
        {
            game->board.wKnights |= (game->board.wPawns & promotion_mask);
            game->board.wPawns &= ~promotion_mask;
        }
        else
        {
            game->board.bKnights |= (game->board.bPawns & promotion_mask);
            game->board.bPawns &= ~promotion_mask;
        }
        game->pawn_promotion = 0;
        return 1;
    default:
        return 0; // Invalid promotion type
    }
}

U64 get_valid_moves(U64 square, Board *board, Side side)
{
    Piece piece = get_piece_type(square, board);

    switch (piece)
    {
    case Pawn:
        return generate_pawn_moves(square, board, side);
    case Knight:
        return generate_knight_moves(square, board, side);
    case Bishop:
        return generate_bishop_moves(square, board, side);
    case Rook:
        return generate_rook_moves(square, board, side);
    case Queen:
        return generate_queen_moves(square, board, side);
    case King:
        return generate_king_moves(square, board, side);
    default:
        return 0ULL;
    }
}