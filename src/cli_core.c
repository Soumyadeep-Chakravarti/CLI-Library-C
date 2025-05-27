// ~/src/cli_core.c

#include "cli_core.h"
#include "screen.h"
#include "input.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static int running = 1;

static void handle_sigint(int signum) {
    (void)signum;
    running = 0;
}

int cli_core_init(void) {
    if (cli_screen_init() != 0) return -1;
    if (cli_input_init() != 0) return -1;

    signal(SIGINT, handle_sigint);
    return 0;
}

void cli_core_shutdown(void) {
    cli_input_shutdown();
    cli_screen_shutdown();
}

void cli_core_run(void (*render_func)(void)) {
    while (running) {
        cli_input_poll();   // update input state
        render_func();      // user-defined render callback
        cli_screen_present();
        usleep(16000);      // ~60fps
    }
}
