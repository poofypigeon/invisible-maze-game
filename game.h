#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define MAZE_HEIGHT 5
#define MAZE_WIDTH 7

#define LOSE_STATE -1
#define CONTINUE_STATE 0
#define WIN_STATE 1

// 2D to 1D index
#define maze_index(x, y, w) (y * w + x)

typedef struct {
    const uint32_t height;
    const uint32_t width;
    uint32_t x_pos;
    uint32_t y_pos;
    uint8_t *path;
    int32_t *maze;
} game_state_t;

game_state_t new_game(uint32_t height, uint32_t width, uint32_t seed);

void end_game(game_state_t game);

int make_move(int8_t x_move, int8_t y_move, game_state_t *game);

#endif // GAME_H