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
    const uint16_t height;
    const uint16_t width;
    uint16_t x_pos;
    uint16_t y_pos;
    uint8_t *maze;
} game_state_t;

#endif // GAME_H