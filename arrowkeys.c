#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>

#define UP_ARROW 'A'
#define DOWN_ARROW 'B'
#define RIGHT_ARROW 'C'
#define LEFT_ARROW 'D'

#define MATRIX_WIDTH 7
#define MATRIX_HEIGHT 5

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

/*
 * Reset the display and cursor to starting state
 */
void reset_maze(unsigned int *x_pos, unsigned int *y_pos) {
    *x_pos = 0;
    *y_pos = 0;

    printf("\033[2J\033[3;1H");
    printf("  > * . . . . . .\n\r");
    printf("    . . . . . . .\n\r");
    printf("    . . . . . . .\n\r");
    printf("    . . . . . . .\n\r");
    printf("    . . . . . . . > ");
    puts("\033[2;0H");
    fflush(stdout);
}



int main() {
    enable_raw_mode();

    unsigned int x_pos = 0;
    unsigned int y_pos = 0;

    reset_maze(&x_pos, &y_pos);

    while (1) {
        char dir = read_arrow_input();
        switch(dir) {
            case UP_ARROW:
                printf("UP\n\r");
                fflush(stdout);
                break;
            case DOWN_ARROW:
                printf("DOWN\n\r");
                fflush(stdout);
                break;
            case RIGHT_ARROW:
                printf("RIGHT\n\r");
                fflush(stdout);
                break;
            case LEFT_ARROW:
                printf("LEFT\n\r");
                fflush(stdout);
                break;
        }
    }

    return 0;
}