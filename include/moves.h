#include "game.h"

U64 generate_pawn_moves(U64 square, Board *board, Side side);
U64 generate_knight_moves(U64 square, Board *board, Side side);
U64 generate_diagonal_moves(U64 square, Board *board, Side side);
U64 generate_horizontal_moves(U64 square, Board *board, Side side);
U64 generate_bishop_moves(U64 square, Board *board, Side side);
U64 generate_rook_moves(U64 square, Board *board, Side side);
U64 generate_queen_moves(U64 square, Board *board, Side side);
U64 generate_king_moves(U64 square, Board *board, Side side);
U64 get_piece_bit(char *square);
U64 *get_piece_bitboard(Board *board, Piece piece, Side side);

Piece get_piece_type(U64 square, Board *board);
U64 get_valid_moves(U64 square, Board *board, Side side);
int promote_pawn(Game *game, Piece type);
int capture(Game *game, U64 from, U64 to, Piece type);
int move(Game *game, U64 from, U64 to, Piece type);