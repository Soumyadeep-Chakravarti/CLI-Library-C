// ~/src/core/runtime.c

#include "core/runtime.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static volatile bool exit_requested = false;

void tui_core_request_exit(void) {
    exit_requested = true;
}

bool tui_core_should_exit(void) {
    return exit_requested;
}

static void handle_sigint(int signo) {
    (void)signo;
    tui_core_request_exit();
}

void tui_core_init(void) {
    signal(SIGINT, handle_sigint);
    // TODO: init timer, logger, or screen
}

void tui_core_shutdown(void) {
    // TODO: cleanup screen, state, etc.
}

void tui_core_run(RenderCallback render) {
    while (!tui_core_should_exit()) {
        render();
        usleep(100000); // 10 FPS default
    }
}
