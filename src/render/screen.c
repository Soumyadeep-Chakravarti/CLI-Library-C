// ~/src/render/screen.c

#include "render/screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

static struct termios orig_termios;
static size_t term_width = 0;
static size_t term_height = 0;
static Cell *buffer = NULL;

static void enable_raw_mode(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

static void query_terminal_size(void) {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    term_width = ws.ws_col;
    term_height = ws.ws_row;
}

void screen_init(void) {
    enable_raw_mode();
    query_terminal_size();
    buffer = calloc(term_width * term_height, sizeof(Cell));
    printf("\x1b[?25l"); // hide cursor
    printf("\x1b[2J");   // clear screen
}

void screen_shutdown(void) {
    printf("\x1b[?25h"); // show cursor
    printf("\x1b[2J");
    printf("\x1b[H");
    disable_raw_mode();
    free(buffer);
    buffer = NULL;
}

void screen_clear(void) {
    memset(buffer, 0, term_width * term_height * sizeof(Cell));
}

void screen_draw(int x, int y, char ch, uint8_t fg, uint8_t bg) {
    if (x < 0 || y < 0 || x >= (int)term_width || y >= (int)term_height) return;
    buffer[y * term_width + x] = (Cell){ ch, fg, bg };
}

void screen_flush(void) {
    printf("\x1b[H"); // move cursor to home
    for (size_t y = 0; y < term_height; ++y) {
        for (size_t x = 0; x < term_width; ++x) {
            putchar(buffer[y * term_width + x].ch ? buffer[y * term_width + x].ch : ' ');
        }
        putchar('\n');
    }
    fflush(stdout);
}

size_t screen_width(void) {
    return term_width;
}

size_t screen_height(void) {
    return term_height;
}
