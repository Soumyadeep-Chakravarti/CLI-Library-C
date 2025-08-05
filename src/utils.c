#include "termiui.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void clear_screen() {
    printf("\033[2J");
}

void move_cursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void enable_raw_mode(Conductor* conductor) {
    if (tcgetattr(STDIN_FILENO, &conductor->original_termios) == -1) {
        perror("tcgetattr");
        exit(1);
    }

    struct termios raw = conductor->original_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

void disable_raw_mode(Conductor* conductor) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &conductor->original_termios) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}
