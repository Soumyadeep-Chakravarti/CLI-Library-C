// ~/src/screen.c

#include "screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

static int width, height;
static char *buffer_front = NULL;
static char *buffer_back = NULL;
static struct termios orig_termios;

static void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

static void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int cli_screen_init(void) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        width = 80; height = 24;
    } else {
        width = ws.ws_col;
        height = ws.ws_row;
    }

    buffer_front = calloc(width * height, sizeof(char));
    buffer_back = calloc(width * height, sizeof(char));
    if (!buffer_front || !buffer_back) return -1;

    enable_raw_mode();

    printf("\033[2J");  // clear screen
    printf("\033[?25l"); // hide cursor
    fflush(stdout);

    return 0;
}

void cli_screen_shutdown(void) {
    printf("\033[?25h"); // show cursor
    printf("\033[0m");   // reset colors
    printf("\033[2J");   // clear screen
    printf("\033[H");    // cursor home
    fflush(stdout);

    free(buffer_front);
    free(buffer_back);
    buffer_front = buffer_back = NULL;

    disable_raw_mode();
}

void cli_screen_clear(void) {
    memset(buffer_back, ' ', width * height);
}

void cli_screen_put_char(int x, int y, char c) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    buffer_back[y * width + x] = c;
}

void cli_screen_present(void) {
    // diff and render
    for (int i = 0; i < width * height; ++i) {
        if (buffer_back[i] != buffer_front[i]) {
            int x = i % width;
            int y = i / width;
            printf("\033[%d;%dH%c", y + 1, x + 1, buffer_back[i]);
            buffer_front[i] = buffer_back[i];
        }
    }
    fflush(stdout);
}

int cli_screen_width(void) { return width; }
int cli_screen_height(void) { return height; }
