#include "game.h"
#include <stdbool.h>

static const uint16_t maze_path[MAZE_HEIGHT * MAZE_WIDTH] = { 
    1, 2, 3, 4, 5, 6, 13,
    0, 0, 0, 0, 0, 0, 20,
    0, 0, 0, 0, 0, 0, 27,
    0, 0, 0, 0, 0, 0, 34,
    0, 0, 0, 0, 0, 0, -1
};

static bool move_correct(const uint16_t *maze_path, int8_t x_move, int8_t y_move, game_state_t *game) {
    // CHECK FOR BAD INPUTS
    if (x_move * y_move != 0)   return false;   // simultaneous x and y moves
    if (x_move * x_move > 1)    return false;   // x_move more than one space
    if (y_move * y_move > 1)    return false;   // y_move more than one space

    // convert 2D index to 1D index
    uint8_t move = x_move + (game->width * y_move);

    // if the user's movement matches the current node's edge, the move is correct
    if (maze_path[maze_index(game->x_pos, game->y_pos, game->width)] 
     == maze_index(game->x_pos, game->y_pos, game->width) + move)
        return true;
    
    // wrong move
    return false;
}

int try_move(int8_t x_move, int8_t y_move, game_state_t *game) {
    if (move_correct(maze_path, x_move, y_move, game->width)) {
        // one is verified to be 0
        // the magnitude of the other is <= 1
        game->x_pos += x_move;
        game->y_pos += y_move;

        // test for win condition
        if (maze_path[maze_index(game->x_pos, game->y_pos, game->width)] == 0xFFFF) {
            reset_game();
            return WIN_STATE;
        }

        // x-index = x/8 [bytes] + x%8 [bits]
        // y-index = width/8 [row width in bytes] * y [current row]
        // 1D-index = y-index + x-index
        uint16_t display_byte_index = (game->width / 8 * game->y_pos) + (game->x_pos / 8);
        uint8_t display_bit_index = game->x_pos % 8;

        // set the bit corresponding to the new position to 1
        game->maze[display_byte_index] |= 0x80 >> display_bit_index;

        return CONTINUE_STATE;
    }

    // wrong move
    reset_game();
    return LOSE_STATE;
}

void new_game() {}

static void reset_game() {}

