#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "game.h"

#define UP_ARROW 'A'
#define DOWN_ARROW 'B'
#define RIGHT_ARROW 'C'
#define LEFT_ARROW 'D'

struct termios orig_termios;

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char read_arrow_input() {
    char c;
    while (read(STDIN_FILENO, &c, 1) != -1) {
        if (c == 'q') exit(0);
        if (c == 27) {
            char cmd[2];
            read(STDIN_FILENO, &cmd, 2);
            if (cmd[0] != '[') continue;
            if (cmd[1] >= UP_ARROW && cmd [1] <= LEFT_ARROW) return cmd[1];
        }
    }
    exit(1);
}

int main() {
    enable_raw_mode();

    uint32_t iteration = 0;

    game_state_t game = new_game(MAZE_HEIGHT, MAZE_WIDTH, iteration);

    int status;

    while (1) {
        printf("\033[1J\033[0;0H");
        for (int row = 0; row < game.height; row++) {
            for (int byte = 0; byte <= game.width / 8; byte++) {
                for (int bit = 0; bit < game.width; bit++) {
                    printf("%c", (game.path[row * (game.width / 8 +1) + byte] & (0x80 >> bit)) ? '1' : '0');
                }
                printf("\n\r");
            }
        }
        printf("\033[%d;%dH", game.y_pos + 1, game.x_pos + 1);
        fflush(stdout);

        char dir = read_arrow_input();
        switch(dir) {
            case UP_ARROW:
                status = make_move(0, -1, &game);
                break;
            case DOWN_ARROW:
                status = make_move(0, 1, &game);
                break;
            case RIGHT_ARROW:
                status = make_move(1, 0, &game);
                break;
            case LEFT_ARROW:
                status = make_move(-1, 0, &game);
                break;
        }

        if (status == 1) {
            iteration++;
            printf("\033[2J\033[0;0HYOU WIN!");
            fflush(stdout);
            sleep(1);
            end_game(game);
            game_state_t next_game = new_game(MAZE_HEIGHT, MAZE_WIDTH, iteration);
            memcpy(&game, &next_game, sizeof(game_state_t));
        }
    }

    return 0;
}