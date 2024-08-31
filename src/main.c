#include "gui/main_window.h"
#include "game.h"
#include "moves.h"

int main(int argc, char **argv)
{
    Game game;
    init_board(&game.board);
    // print_board(&board);

    Side side = WHITE;
    game.side = side;
    game.turn = side;
    // U64 square = get_piece_bit("a5");
    // U64 moves = generate_queen_moves(square, &board, side);

    // U64 knight_moves = 0x1ULL;

    // print_u64(moves);
    // print_u64(pawn_moves);
    // fprintf(stdout, "---------\n");

    render_game(&game, side);
    return 0;
}