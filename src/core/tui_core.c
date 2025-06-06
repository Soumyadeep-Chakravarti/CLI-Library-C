// ~/src/core/tui_core.c

#include "core/tui_core.h"

#include <unistd.h>   // for usleep
#include <stdio.h>    // for printf (debug/logging)
#include <termios.h>  // for terminal input settings
#include <fcntl.h>    // for fcntl
#include <unistd.h>   // for read
#include <signal.h>   // for signal handling
#include <stdbool.h>
#include <stdlib.h>

static volatile int tui_running = 0;
static int exit_key = 'q'; // default exit key

// Terminal settings backup
static struct termios orig_termios;

// Restore terminal settings on exit
static void reset_terminal_mode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

// Enable non-blocking raw input mode
static void set_terminal_mode(void) {
    struct termios new_termios;

    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;

    // Disable canonical mode, echo and signals
    new_termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    // Disable Ctrl-S and Ctrl-Q flow control
    new_termios.c_iflag &= ~(IXON);
    // Minimum number of characters for noncanonical read
    new_termios.c_cc[VMIN] = 0;
    // Timeout (tenths of seconds) for noncanonical read
    new_termios.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    // Set stdin non-blocking
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    atexit(reset_terminal_mode);
}

// Handle Ctrl+C to request exit
static void sigint_handler(int signo) {
    (void)signo;
    tui_running = 0;
}

void tui_core_set_exit_key(int key) {
    exit_key = key;
}

static void handle_input(void) {
    char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);
    if (n > 0) {
        printf("Got char: '%c' (%d)\n", ch, (int)ch); // ← debug output
        fflush(stdout); // ensure it's printed
        if ((int)ch == exit_key) {
            printf("[TUI] Exit key pressed.\n");
            fflush(stdout);
            tui_running = 0;
        }
    }
}

void tui_core_init(void) {
    tui_running = 1;
    set_terminal_mode();
    signal(SIGINT, sigint_handler);
    printf("[TUI] Initialized core system.\n");
}

void tui_core_run(tui_render_fn render) {
    if (!render) return;
    while (tui_running) {
        handle_input();
        render();
        usleep(100000);// 10 FPS placeholder
    }
}

void tui_core_shutdown(void) {
    reset_terminal_mode();
    printf("[TUI] Shutting down.\n");
}
