// ~/src/core/tui_core.c

#include "core/tui_core.h"
#include <unistd.h>   // for usleep
#include <stdio.h>    // for printf (debug/logging)

static int tui_running = 0;

void tui_core_init(void) {
    tui_running = 1;
    // TODO: initialize screen, input, and timing systems
    printf("[TUI] Initialized core system.\n");
}

void tui_core_run(tui_render_fn render) {
    if (!render) return;

    while (tui_running) {
        render();
        usleep(100000); // 10 FPS placeholder
    }
}

void tui_core_shutdown(void) {
    // TODO: cleanup screen, input, state
    printf("[TUI] Shutting down.\n");
}
