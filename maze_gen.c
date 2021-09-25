#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze_gen.h"
#include "macro.h"

// #define DEBUG

int32_t *generate_maze(uint32_t height, uint32_t width, uint32_t seed) {
    srandom(seed);          // establish seed for RNG

    uint32_t x_pos = 0;
    uint32_t y_pos = 0;
    uint32_t step  = 0;     // how many spaces moved (current path index)

    int8_t *visited = malloc(height * width); // keep track of visited spaces
    uint32_t *path  = malloc(height * width * sizeof(uint32_t));
    memset(visited, '\0', height * width * sizeof(int8_t));

    // use a maze gen algorithm
    // - mark seen squares as visited
    // - keep track of the path we've taken in current_path
    // - if you reach a dead end, backtrack and remove from current_path
    // - when exit square is reached, take current_path as full path
    while ((path[step] = cto1d(x_pos, y_pos, width)) != height * width - 1) {
        path[step] = cto1d(x_pos, y_pos, width);

        visited[path[step]] = 1;

        bool neighbors[4] = { true, true, true, true };

        // identify non-visited neighbor sqaures
        if (y_pos == 0 || visited[cto1d(x_pos, y_pos - 1, width)] != 0)
            neighbors[UP] = false;

        if (x_pos == width - 1 || visited[cto1d(x_pos + 1, y_pos, width)] != 0) 
            neighbors[RIGHT] = false;

        if (y_pos == height - 1 || visited[cto1d(x_pos, y_pos + 1, width)] != 0)
            neighbors[DOWN] = false;

        if (x_pos == 0 || visited[cto1d(x_pos - 1, y_pos, width)] != 0) 
            neighbors[LEFT] = false;

        // count free neightbors
        uint8_t free_neighbors = 0;
        for (int dir = UP; dir <= LEFT; dir++)
            if (neighbors[dir]) free_neighbors++;

        // backtrack if no neighbors available
        if (free_neighbors == 0) {
            visited[path[step]] = -1; // mark as backtracked
            step--;
            x_pos = ito2dx(path[step], width);
            y_pos = ito2dy(path[step], width);
            continue;
        }

        // generate a random number
        uint8_t nth_free_direction = random() % free_neighbors;

        // choose a direction using the random number
        direction_t move_direction;
        for (uint8_t dir = UP; dir <= LEFT; dir++) {
            if (neighbors[dir] == true) {
                if (nth_free_direction <= 0) {
                    move_direction = dir;
                    break;
                }
                nth_free_direction--;
            }
        }

        // update coords
        switch (move_direction) {
            case UP:
                y_pos--;
                break;
            case RIGHT:
                x_pos++;
                break;
            case DOWN:
                y_pos++;
                break;
            case LEFT:
                x_pos--;
                break;
        }

        step++;
    }

    // use path to generate maze graph
    int32_t *maze = malloc(height * width * sizeof(int32_t));
    memset(maze, 0, height * width * sizeof(int32_t));

    for (uint32_t i = 0; i < step; i++)
        maze[path[i]] = path[i + 1];

    // set maze exit space to -1
    maze[width * height - 1] = -1;

    #ifdef DEBUG
        // print maze in graph form
        printf("SEED %d\n\r", seed);
        for (uint32_t i = 0; i < height; i++) {
            for (uint32_t j = 0; j < width; j++) {
                printf("%4d", maze[cto1d(j, i, width)]);
            }
            printf("\n\r");
        }
    #endif

    return maze;
}