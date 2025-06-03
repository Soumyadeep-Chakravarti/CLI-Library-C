// ~/src/cli_core.c

#include "cli_core.h"
#include "screen.h"
#include "input.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static volatile int stop_requested = 0;

void cli_core_request_stop(void) {
    stop_requested = 1;
}

int cli_core_should_stop(void) {
    return stop_requested;
}

// Signal handler
static void handle_sigint(int signum) {
    (void)signum;
    cli_core_request_stop();
}

int cli_core_init(void) {
    if (cli_screen_init() != 0) return -1;
    if (cli_input_init() != 0) return -1;

    signal(SIGINT, handle_sigint);  // Handle Ctrl+C
    return 0;
}

void cli_core_shutdown(void) {
    cli_input_shutdown();
    cli_screen_shutdown();
}

void cli_core_run(void (*render_func)(void)) {
    while (!cli_core_should_stop()) {
        cli_input_poll();   // updates keyboard state
        if (cli_input_key_pressed('q')) {
            cli_core_request_stop();
        }

        render_func();
        cli_screen_present();
        usleep(16000);  // ~60 FPS
    }
}
