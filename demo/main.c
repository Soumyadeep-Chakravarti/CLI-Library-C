// ~/demo/main.c

#include "cli_core.h"
#include "screen.h"
#include <stdio.h>

static int frame = 0;

void render(void) {
    cli_screen_clear();

    int w = cli_screen_width();
    int h = cli_screen_height();

    char msg[] = "Hello from CLI TUI Framework!";
    int msg_len = sizeof(msg) - 1;

    int x = (w - msg_len) / 2;
    int y = h / 2;

    for (int i = 0; i < msg_len; i++) {
        cli_screen_put_char(x + i, y, msg[i]);
    }

    // simple animation
    char spinner[] = "-\\|/";
    cli_screen_put_char(w - 2, h - 1, spinner[frame % 4]);
    frame++;
}

int main(void) {
    if (cli_core_init() != 0) {
        fprintf(stderr, "Failed to initialize CLI core\n");
        return 1;
    }

    cli_core_run(render);

    cli_core_shutdown();
    return 0;
}
