#ifndef MAZE_GEN_H
#define MAZE_GEN_H

#include <stdint.h>

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
} direction_t;

int32_t *generate_maze(uint32_t height, uint32_t width, uint32_t seed);

#endif // MAZE_GEN_H