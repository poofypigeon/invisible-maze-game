#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "macro.h"
#include "maze_gen.h"

static bool move_is_correct(int8_t x_move, int8_t y_move, game_state_t *game) {
    // CHECK FOR BAD INPUTS
    if (x_move * y_move != 0)   return false;   // simultaneous x and y moves
    if (x_move * x_move > 1)    return false;   // x_move more than one space
    if (y_move * y_move > 1)    return false;   // y_move more than one space

    // convert 2D index to 1D index
    int8_t move = cto1d(x_move, y_move, game->width);

    // if the user's movement matches the current node's edge, the move is correct
    if (game->maze[cto1d(game->x_pos, game->y_pos, game->width)] 
     == cto1d(game->x_pos, game->y_pos, game->width) + move)
        return true;
    
    // wrong move
    return false;
}

static void reset_game(game_state_t *game) {
    game->x_pos = 0;
    game->y_pos = 0;
    memset(game->path, '\0', game->height * ((game->width / 8) + 1));
    game->path[0] = 0x80;
}

int make_move(int8_t x_move, int8_t y_move, game_state_t *game) {
    if (move_is_correct(x_move, y_move, game)) {
        // one is verified to be 0
        // the magnitude of the other is <= 1
        game->x_pos += x_move;
        game->y_pos += y_move;

        // test for win condition
        if (game->maze[cto1d(game->x_pos, game->y_pos, game->width)] == -1) {
            // reset_game(game);
            return WIN_STATE;
        }

        // x-index = (x / 8) [bytes] + (x % 8) [bits]
        // y-index = (width / 8) [row width in bytes] * y [current row]
        // 1D-index = y-index + x-index
        uint32_t display_byte_index = (((game->width / 8) + 1) * game->y_pos) + (game->x_pos / 8);
        
        uint8_t display_bit_index = game->x_pos % 8;

        // set the bit corresponding to the new position to 1
        game->path[display_byte_index] |= 0x80 >> display_bit_index;

        return CONTINUE_STATE;
    }

    // wrong move
    reset_game(game);
    return LOSE_STATE;
}

game_state_t new_game(uint32_t height, uint32_t width, uint32_t seed) {
    game_state_t game = { height, width, 0, 0, 0, 0 };

    game.path = malloc(height * (width / 8));
    memset(game.path, '\0', height * ((width / 8) + 1));
    game.path[0] = 0x80;

    game.maze = generate_maze(height, width, seed);
    return game;
}

void end_game(game_state_t game) {
    free(game.path);
    free(game.maze);
}
